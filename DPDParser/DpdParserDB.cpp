#include "stdafx.h"
#include "DpdParserDB.h"

#pragma region --New & ~--

DpdParserDB::DpdParserDB()
{
	sqlite3_config(SQLITE_CONFIG_URI, 1);
	int res = sqlite3_open("file::memory:?cache=shared", &m_pDB);
	//int res = sqlite3_open("file:C:\\Users\\solidest\\Desktop\\parser.db", &m_pDB);
	if (res != SQLITE_OK)
	{
		sqlite3_close(m_pDB);
		m_pDB = NULL;
		return;
	}

	m_taskcode = NULL;

	res = sqlite3_prepare_v2(m_pDB, "SELECT tasktype, dpdcode FROM predpd_task WHERE rowid = :taskid and taskstate = :state", -1, &m_predpd_task_stmt, NULL);
	if (res != SQLITE_OK)
	{
		m_predpd_task_stmt = NULL;
	}

	res = sqlite3_prepare_v2(m_pDB, "INSERT INTO predpd_property (segment, propertyname, propertyvalue, propertytype) VALUES(:segment, :propertyname, :propertyvalue, :propertytype);", -1, &m_predpd_property_stmt, NULL);
	if (res != SQLITE_OK)
	{
		m_predpd_property_stmt = NULL;
	}

	res = sqlite3_prepare_v2(m_pDB, "INSERT INTO predpd_error (taskid, errorcode, firstsymbol, lastsymbol) VALUES(:taskid, :errorcode, :firstsymbol, :lastsymbol);", -1, &m_predpd_error_stmt, NULL);
	if (res != SQLITE_OK)
	{
		m_predpd_error_stmt = NULL;
	}

	res = sqlite3_prepare_v2(m_pDB, "INSERT INTO predpd_symbol (taskid, symbol, lineno, firstcolumn, lastcolumn) VALUES(:taskid, :symbol, :lineno, :firstcolumn, :lastcolumn);", -1, &m_predpd_symbol_stmt, NULL);
	if (res != SQLITE_OK)
	{
		m_predpd_symbol_stmt = NULL;
	}

	res = sqlite3_prepare_v2(m_pDB, "INSERT INTO predpd_notes (notesymbol, aftersymbol) VALUES(:notesymbol, :aftersymbol);", -1, &m_predpd_notes_stmt, NULL);
	if (res != SQLITE_OK)
	{
		m_predpd_notes_stmt = NULL;
	}

	res = sqlite3_prepare_v2(m_pDB, "INSERT INTO predpd_protocol (taskid, protocolname) VALUES(:taskid, :protocolname);", -1, &m_predpd_protocol_stmt, NULL);
	if (res != SQLITE_OK)
	{
		m_predpd_protocol_stmt = NULL;
	}

	res = sqlite3_prepare_v2(m_pDB, "INSERT INTO predpd_segment (protocol, segmentname, segmenttype) VALUES(:protocol, :segmentname, :segmenttype);", -1, &m_predpd_segment_stmt, NULL);
	if (res != SQLITE_OK)
	{
		m_predpd_segment_stmt = NULL;
	}

}


DpdParserDB::~DpdParserDB()
{
	if (m_pDB)
	{
		sqlite3_close(m_pDB);
		m_pDB = NULL;
	}

	if (m_taskcode)
	{
		delete(m_taskcode);
	}

	if (m_predpd_task_stmt)
	{
		sqlite3_finalize(m_predpd_task_stmt);
		m_predpd_task_stmt = NULL;
	}

	if (m_predpd_segment_stmt)
	{
		sqlite3_finalize(m_predpd_segment_stmt);
		m_predpd_segment_stmt = NULL;
	}

	if (m_predpd_error_stmt)
	{
		sqlite3_finalize(m_predpd_error_stmt);
		m_predpd_error_stmt = NULL;
	}

	if (m_predpd_symbol_stmt)
	{
		sqlite3_finalize(m_predpd_symbol_stmt);
		m_predpd_symbol_stmt = NULL;
	}

	if (m_predpd_notes_stmt)
	{
		sqlite3_finalize(m_predpd_notes_stmt);
		m_predpd_notes_stmt = NULL;
	}

	if (m_predpd_property_stmt)
	{
		sqlite3_finalize(m_predpd_property_stmt);
		m_predpd_property_stmt = NULL;
	}

	if (m_predpd_protocol_stmt)
	{
		sqlite3_finalize(m_predpd_protocol_stmt);
		m_predpd_protocol_stmt = NULL;
	}
}

#pragma endregion


//加载分析任务
bool DpdParserDB::LoadTask(int taskid)
{
	sqlite3_bind_int(m_predpd_task_stmt, 1, taskid);
	sqlite3_bind_int(m_predpd_task_stmt, 2, TASK_STATE_READY);
	int res = sqlite3_step(m_predpd_task_stmt);
	if ((res != SQLITE_DONE) && (res != SQLITE_ROW))
	{
		return false;
	}
	m_taskid = taskid;
	m_tasktype = sqlite3_column_int(m_predpd_task_stmt, 0);
	const char* code = (const char*)sqlite3_column_text(m_predpd_task_stmt, 1);

	if (!code) return false;
	int len = (int)strlen(code);
	m_codesize = len + 2;
	m_taskcode = new char[m_codesize];
	m_taskcode[m_codesize-1] = '\0';
	strcpy_s(m_taskcode, len+1, code);


	sqlite3_reset(m_predpd_task_stmt);
	return true;
}

//更新任务状态
void DpdParserDB::UpdateTaskState(int state)
{
	char str[80];
	char *err_msg;
	int ret = 0;
	sprintf_s(str, "UPDATE predpd_task SET taskstate = %d WHERE rowid = %d", state, m_taskid);
	ret = sqlite3_exec(m_pDB, str, NULL, NULL, &err_msg);
	if (0 != ret)
	{
		;//log
	}
}

//记录错误信息
int DpdParserDB::SaveError(int errcode, int firstsymbol, int lastsymbol)
{
	sqlite3_bind_int(m_predpd_error_stmt, 1, m_taskid);
	sqlite3_bind_int(m_predpd_error_stmt, 2, errcode);
	sqlite3_bind_int(m_predpd_error_stmt, 3, firstsymbol);
	sqlite3_bind_int(m_predpd_error_stmt, 4, lastsymbol);
	int rc = sqlite3_step(m_predpd_error_stmt);
	if ((rc != SQLITE_DONE) && (rc != SQLITE_ROW))
	{
		return -1;
	}
	sqlite3_reset(m_predpd_error_stmt);
	return (int)sqlite3_last_insert_rowid(m_pDB);
}

//保存注释
int DpdParserDB::SaveComment(struct comment* notes, int symbol)
{
	if (!notes) return -1;
	sqlite3_bind_int(m_predpd_notes_stmt, 1, notes->line);
	sqlite3_bind_int(m_predpd_notes_stmt, 2, symbol);
	int rc = sqlite3_step(m_predpd_notes_stmt);
	if ((rc != SQLITE_DONE) && (rc != SQLITE_ROW))
	{
		return -1;
	}
	sqlite3_reset(m_predpd_notes_stmt);
	return (int)sqlite3_last_insert_rowid(m_pDB);
}

//保存一条协议结果到数据库
int DpdParserDB::SaveProtocol(struct protocol* proto)
{
	if (!proto) return -1;
	sqlite3_bind_int(m_predpd_protocol_stmt, 1, m_taskid);
	sqlite3_bind_int(m_predpd_protocol_stmt, 2, proto->name);
	int rc = sqlite3_step(m_predpd_protocol_stmt);
	if ((rc != SQLITE_DONE) && (rc != SQLITE_ROW))
	{
		return -1;
	}
	sqlite3_reset(m_predpd_protocol_stmt);
	int idx = (int)sqlite3_last_insert_rowid(m_pDB);

	SaveComment(proto->notes, proto->name);
	SaveSegment(proto->seglist, idx);
	if (proto->next)
	{
		idx = SaveProtocol(proto->next);
	}
	return idx;
}

int DpdParserDB::SaveSegment(struct segment* seg, int protolid)
{
	if (!seg) return -1;
	sqlite3_bind_int(m_predpd_segment_stmt, 1, protolid);
	sqlite3_bind_int(m_predpd_segment_stmt, 2, seg->name);
	sqlite3_bind_int(m_predpd_segment_stmt, 3, seg->segtype);
	int rc = sqlite3_step(m_predpd_segment_stmt);
	if ((rc != SQLITE_DONE) && (rc != SQLITE_ROW))
	{
		return -1;
	}
	sqlite3_reset(m_predpd_segment_stmt);
	int idx = (int)sqlite3_last_insert_rowid(m_pDB);

	SaveComment(seg->notes, seg->name);
	SaveProperty(seg->properlist, idx);
	if (seg->next)
	{
		idx = SaveSegment(seg->next, protolid);
	}
	return idx;
}

int DpdParserDB::SaveProperty(struct property* proper, int segid)
{
	if (!proper) return -1;
	sqlite3_bind_int(m_predpd_property_stmt, 1, segid);
	sqlite3_bind_int(m_predpd_property_stmt, 2, proper->name);
	sqlite3_bind_int(m_predpd_property_stmt, 3, proper->value);
	sqlite3_bind_int(m_predpd_property_stmt, 4, proper->vtype);
	int rc = sqlite3_step(m_predpd_property_stmt);
	if ((rc != SQLITE_DONE) && (rc != SQLITE_ROW))
	{
		return -1;
	}
	sqlite3_reset(m_predpd_property_stmt);
	int idx = (int)sqlite3_last_insert_rowid(m_pDB);

	if (proper->next)
	{
		idx = SaveProperty(proper->next, segid);
	}
	return idx;
}

int DpdParserDB::SaveSymbol(const char* symbol, int lineno, int firstcolumn, int lastcolumn)
{
	if (!symbol) return -1;
	sqlite3_bind_int(m_predpd_symbol_stmt, 1, m_taskid);
	sqlite3_bind_text(m_predpd_symbol_stmt, 2, symbol, -1, SQLITE_STATIC);
	sqlite3_bind_int(m_predpd_symbol_stmt, 3, lineno);
	sqlite3_bind_int(m_predpd_symbol_stmt, 4, firstcolumn);
	sqlite3_bind_int(m_predpd_symbol_stmt, 5, lastcolumn);
	int rc = sqlite3_step(m_predpd_symbol_stmt);
	if ((rc != SQLITE_DONE) && (rc != SQLITE_ROW))
	{
		return -1;
	}
	sqlite3_reset(m_predpd_symbol_stmt);
	int idx = (int)sqlite3_last_insert_rowid(m_pDB);
	return idx;
}

