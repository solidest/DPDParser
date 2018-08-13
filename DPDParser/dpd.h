#pragma once
void outerror(int errcode, int lineno, const char *s);
void yyerror(const char *s);
extern int yylineno;

struct ast {
	int nodetype;
	struct ast *l;
	struct ast *r;
};

struct dpd {
	int nodetype;
	struct protocol *prolist;
};

struct protocol {
	int nodetype;
	int lineno;
	char *name;
	char *notes;
	struct protocol *next;
	struct segment *seglist;
};

struct segmeng {
	int nodetype;
	int lineno;
	char *name;
	char *notes;
	char *segtype;
	struct property *properlist;
	struct segment *next;
};

struct property {
	int nodetype;
	int lineno;
	char *name;
	char *value;
	enum valuetype vtype;
	struct property *next;
};

enum valuetype {
	value_int = 1,
	value_float,
	value_str,
	value_range,
	value_property,
	value_switch_case,
	value_switch_default
};



