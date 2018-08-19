#pragma once
#include "sqlite3.h"
#include "dpd.h"

class DpdParserDB
{
private:
	sqlite3 * m_pDB;
	sqlite3_stmt *m_protocol_stmt;
	sqlite3_stmt *m_segment_stmt;
	//sqlite3_stmt *m_property_stmt;

public:
	DpdParserDB();
	~DpdParserDB();
	bool SaveProtocol(struct protocol* proto);
};

