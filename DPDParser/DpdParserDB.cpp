#include "stdafx.h"
#include "DpdParserDB.h"


DpdParserDB::DpdParserDB()
{
	int res = sqlite3_open("file::memory:?cache=shared", &m_pDB);
	if (res != SQLITE_OK) 
	{
		sqlite3_close(m_pDB);
		m_pDB = NULL;
		return;
	}

	res = sqlite3_prepare_v2(m_pDB, "INSERT INTO predpd_notes (note, lineno, tokentype, tokenid) VALUES(:note, :lineno, :tokentype, :tokenid);", -1, &m_predpd_notes_stmt, NULL);
	if (res != SQLITE_OK)
	{
		m_predpd_notes_stmt = NULL;
	}

	res = sqlite3_prepare_v2(m_pDB, "INSERT INTO predpd_protocol (pretaskid, name, lineno) VALUES(:pretaskid, :name, :lineno);", -1, &m_predpd_protocol_stmt, NULL);
	if (res != SQLITE_OK)
	{
		m_predpd_protocol_stmt = NULL;
	}

	res = sqlite3_prepare_v2(m_pDB, "INSERT INTO predpd_property (segmentid, propertyname) VALUES(:segmentid, :propertyname);", -1, &m_predpd_property_stmt, NULL);
	if (res != SQLITE_OK)
	{
		m_predpd_property_stmt = NULL;
	}

	res = sqlite3_prepare_v2(m_pDB, "INSERT INTO predpd_error (taskid, errorcode, firstsymbol, lastsymbol) VALUES(:taskid, :errorcode, :firstsymbol, :lastsymbol);", -1, &m_predpd_error_stmt, NULL);
	if (res != SQLITE_OK)
	{
		m_predpd_error_stmt = NULL;
	}


}


DpdParserDB::~DpdParserDB()
{
	if (m_pDB)
	{
		sqlite3_close(m_pDB);
		m_pDB = NULL;
	}

	if (m_predpd_notes_stmt)
	{
		sqlite3_finalize(m_predpd_notes_stmt);
		m_predpd_notes_stmt = NULL;
	}

	if (m_predpd_protocol_stmt)
	{
		sqlite3_finalize(m_predpd_protocol_stmt);
		m_predpd_protocol_stmt = NULL;
	}

	if (m_predpd_property_stmt)
	{
		sqlite3_finalize(m_predpd_property_stmt);
		m_predpd_property_stmt = NULL;
	}

	if (m_predpd_error_stmt)
	{
		sqlite3_finalize(m_predpd_error_stmt);
		m_predpd_error_stmt = NULL;
	}

}

//记录错误信息
void DpdParserDB::SaveError(int errcode, int firstsymbol, int endsymbol)
{

}

//加载分析任务
bool DpdParserDB::LoadTask(int taskid)
{
	return false;
}

//保存一条协议结果到数据库
int DpdParserDB::SaveProtocol(struct protocol* proto)
{

	//sqlite3_bind_text(m_protocol_stmt, 1, proto->name, -1, SQLITE_STATIC);
	//sqlite3_bind_int(m_protocol_stmt, 2, proto->lineno);
	int rc = sqlite3_step(m_protocol_stmt);
	if ((rc != SQLITE_DONE) && (rc != SQLITE_ROW))
	{
		return -1;
	}
	sqlite3_reset(m_protocol_stmt);


	return 0;
	
}

int DpdParserDB::SaveSegment(struct segment* seg, int protolid)
{
	return -1;
}

int DpdParserDB::SaveProperty(struct property* proper, int segid)
{
	return -1;
}


int DpdParserDB::SaveSymbol(const char* symbol, int lineno, int firstcolumn, int lastcolumn)
{
	return -1;
}

void DpdParserDB::UpdateTaskState(int state)
{

}

int SaveSegment(struct segment* seg)
{
	return 0;
}
