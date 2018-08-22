#pragma once
#include "sqlite3.h"
#include "dpd.h"

class DpdParserDB
{
private:
	int m_taskid;
	int m_tasktype;
	char* m_taskcode;
	int m_codesize;


	sqlite3 * m_pDB;

	sqlite3_stmt *m_predpd_task_stmt;
	sqlite3_stmt *m_predpd_segment_stmt;
	sqlite3_stmt *m_predpd_error_stmt;
	sqlite3_stmt *m_predpd_symbol_stmt;
	sqlite3_stmt *m_predpd_notes_stmt;
	sqlite3_stmt *m_predpd_property_stmt;
	sqlite3_stmt *m_predpd_protocol_stmt;

public:
	DpdParserDB();
	~DpdParserDB();


	int SaveProtocol(struct protocol* proto);

	int SaveSymbol(const char* symbol, int lineno, int firstcolumn, int lastcolumn);
	int SaveError(int errcode, int firstsymbol, int endsymbol);

	void UpdateTaskState(int state);
	bool LoadTask(int taskid);
	int getTaskId() { return m_taskid; }
	int getCodeSize() { return m_codesize; }
	char* getTaskCode() { return m_taskcode; }
	int getTaskType() { return m_tasktype; }

private:
	int SaveComment(struct comment* notes, int symbol);
	int SaveSegment(struct segment* seg, int protolid);
	int SaveProperty(struct property* proper, int segid);

};

