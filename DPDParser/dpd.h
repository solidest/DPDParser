#pragma once
void outerror(int errcode, int lineno, const char *s);
void yyerror(const char *s);
extern int yylineno;


struct comment *new_comment(char* v);
struct comment *union_comment(struct comment* list, struct comment* line);

struct protocol *new_protocol(char* pname, struct segment* seglist, struct comment* notes, int lino);
struct protocol *union_protocol(struct protocol* list, struct protocol* p);

struct segment *new_segment(char* pname, enum segmenttype stype, struct property* seglist, struct comment* notes, int lino);
struct segment *union_segment(struct segment* list, struct segment* seg);

struct property *new_switchproperty(char* switchseg, int switchlno, struct property * caselist, char* defaultvalue, int defaultlno);
struct property *new_ifproperty(char* ifid, int iflno, char* cmp, enum valuetype vtype, char* cmpvalue, int cmplino, char* thenvalue, int thenlno, char* elsevalue, int elselno);
struct property *new_property(enum valuetype vt, char* pname, char* pvalue, int lno);
struct property *union_property(struct property* list, struct property* p);

void free_protocol(struct protocol* list);

struct comment {
	char* line;
	struct comment* nextline;
};

struct protocol {
	int lineno;
	char *name;
	char *notes;
	struct protocol *next;
	struct segment *seglist;
};

struct segmeng {
	enum segmenttype segtype;
	int lineno;
	char *name;
	char *notes;
	struct property *properlist;
	struct segment *next;
};

struct property {
	enum valuetype vtype;
	char *name;
	char *value;
	int lineno;
	struct property *next;
};


enum valuetype {
	v_int = 1,
	v_float,
	v_str,
	v_range,
	v_property,
	v_id,
	v_bool,
	v_caseint,
	v_casestr
};

enum segmenttype {
	DPDUInt8 = 1,
	DPDUInt16,
	DPDUInt32,
	DPDInt8,
	DPDInt16,
	DPDInt32,
	DPDIntRandom,
	DPDUIntRandom,
	DPDDouble,
	DPDFloat,
	DPDBoolean,
	DPDCRC,
	DPDArray,
	DPDString,
	DPDBlock,
	DPDBuffer,
	DPDSwitch,
	DPDIfElse
};


