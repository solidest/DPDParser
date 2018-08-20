#include "stdafx.h"
#include "DpdParserDB.h"


DpdParserDB::DpdParserDB()
{
	int res = sqlite3_open("file:memdb1?mode=memory&cache=shared", &m_pDB);
	if (res != SQLITE_OK) 
	{
		sqlite3_close(m_pDB);
		m_pDB = NULL;
		return;
	}

	res = sqlite3_prepare_v2(m_pDB, "insert into pre_protocol values(:name, :lineno);", -1, &m_protocol_stmt, NULL);
	if (res != SQLITE_OK)
	{
		m_protocol_stmt = NULL;
		return;
	}
}


DpdParserDB::~DpdParserDB()
{
	if (m_pDB)
	{
		sqlite3_close(m_pDB);
		m_pDB = NULL;
	}

	if (m_protocol_stmt)
	{
		sqlite3_finalize(m_protocol_stmt);
		m_protocol_stmt = NULL;
	}
}

//记录错误信息
void DpdParserDB::SaveError(int errcode, int lineno, const char *s)
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

	sqlite3_bind_text(m_protocol_stmt, 1, proto->name, -1, SQLITE_STATIC);
	sqlite3_bind_int(m_protocol_stmt, 2, proto->lineno);
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


int DpdParserDB::SaveSymbol(char* symbol, int lineno, int firstcolumn, int lastcolumn)
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
