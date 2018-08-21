#pragma once
#include "sqlite3.h"
#include "dpd.h"

class DpdParserDB
{
private:
	int m_taskid;
	int m_tasktype;
	char* m_taskcode;


	sqlite3 * m_pDB;
	sqlite3_stmt *m_predpd_notes_stmt;
	sqlite3_stmt *m_predpd_protocol_stmt;
	sqlite3_stmt *m_predpd_property_stmt;
	sqlite3_stmt *m_predpd_error_stmt;

public:
	DpdParserDB();
	~DpdParserDB();


	int SaveProtocol(struct protocol* proto);
	int SaveSegment(struct segment* seg, int protolid);
	int SaveProperty(struct property* proper, int segid);
	int SaveSymbol(const char* symbol, int lineno, int firstcolumn, int lastcolumn);

	void SaveError(int errcode, int firstsymbol, int endsymbol);
	void UpdateTaskState(int state);


	bool LoadTask(int taskid);
	int getTaskId() { return m_taskid; }
	char* getTaskCode() { return m_taskcode;  }


};

