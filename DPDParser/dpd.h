#pragma once
void outerror(int errcode, int lineno, const char *s);
void yyerror(const char *s);
extern int yylineno;


struct comment *new_comment(char* v);
struct comment *union_comment(struct comment* list, struct comment* line);

struct property *new_property(enum valuetype vt, char* pname, char* pvalue, int lno);
struct property *union_property(struct property* list, struct property* p);

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
	v_case_int,
	v_case_str,
	v_case_bool,
	v_switch_default
};

enum segmenttype {
	StandardUInt8 = 1,
	StandardUInt16,
	StandardUInt32,
	StandardInt8,
	StandardInt16,
	StandardInt32,
	StandardIntRandom,
	StandardDouble,
	StandardFloat,
	StandardBoolean,
	StandardCRC,
	StandardArray,
	StandardString,
	StandardBlock,
	StatndartBuffer,
	StatndartSwitch
};


