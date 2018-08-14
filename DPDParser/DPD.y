%{
#include <stdio.h>
#include <stdlib.h>
#include "stdafx.h"
#include "dpd.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;

%}

%union {
	int lineno;
	char* propertyname;
	char* propertyvalue;
	char* id;
	enum segmenttype stype;
	struct comment* commentlist;
	struct protocol *protocollist;
	struct segment *segmentlist;
	struct property *propertylist;
}

%token PROTOCOL
%token SEGMENT
%token SEGMENT_TYPE_U8
%token SEGMENT_TYPE_U16
%token SEGMENT_TYPE_U32
%token SEGMENT_TYPE_I8
%token SEGMENT_TYPE_I16
%token SEGMENT_TYPE_I32
%token SEGMENT_TYPE_RI
%token SEGMENT_TYPE_DOUBLE
%token SEGMENT_TYPE_FLOAT
%token SEGMENT_TYPE_BOOLEAN
%token SEGMENT_TYPE_CRC
%token SEGMENT_TYPE_ARRAY
%token SEGMENT_TYPE_STRING
%token SEGMENT_TYPE_BLOCK
%token SEGMENT_TYPE_BUFFER
%token EQUAL
%token CMP
%token <propertyname> SEGMENT_PROPERTY
%token <propertyvalue> VALUE_PROPERTY
%token <propertyvalue> VALUE_INT
%token <propertyvalue> VALUE_FLOAT
%token <propertyvalue> VALUE_STRING
%token <propertyvalue> VALUE_RANGE
%token <propertyname> DEFAULT
%token <propertyname> SWITCH
%token <propertyname> CASE
%token <id> IDENTIFIER
%token <commentlist> COMMENT
%token END

%type <stype> segment_type
%type <commentlist> comment commentlist
%type <protocollist>  protocol protocollist
%type <segmentlist> segment segmentlist
%type <propertylist> property propertylist

%start protocollist

%%

protocollist: 
	| protocollist protocol
;

protocol:  commentlist PROTOCOL IDENTIFIER segmentlist END { ; }
;

segmentlist: 
	| segmentlist segment
;

segment: commentlist SEGMENT IDENTIFIER segment_type propertylist { ; }
;

commentlist:					{ $$ = NULL; }
	| commentlist COMMENT		{ $$ = union_comment($1, $2); }
;


propertylist:					{ $$ = NULL; }
	| propertylist property		{ $$ = union_property($1, $2); }
;


property: SEGMENT_PROPERTY EQUAL VALUE_PROPERTY		{ $$ = new_property(v_property, $1, $3, yylval.lineno); }
	| SEGMENT_PROPERTY EQUAL VALUE_INT				{ $$ = new_property(v_int, $1, $3, yylval.lineno); }
	| SEGMENT_PROPERTY EQUAL VALUE_FLOAT			{ $$ = new_property(v_float, $1, $3, yylval.lineno); }
	| SEGMENT_PROPERTY EQUAL VALUE_STRING			{ $$ = new_property(v_str, $1, $3, yylval.lineno); }
	| SEGMENT_PROPERTY EQUAL VALUE_RANGE			{ $$ = new_property(v_range, $1, $3, yylval.lineno); }
	| DEFAULT EQUAL VALUE_INT						{ $$ = new_property(v_int, $1, $3, yylval.lineno); }
	| DEFAULT EQUAL VALUE_FLOAT						{ $$ = new_property(v_float, $1, $3, yylval.lineno); }
	| DEFAULT EQUAL VALUE_STRING					{ $$ = new_property(v_str, $1, $3, yylval.lineno); }
	| CASE VALUE_INT IDENTIFIER						{ $$ = new_property(v_case_int, $2, $3, yylval.lineno); }
	| CASE VALUE_STRING IDENTIFIER					{ $$ = new_property(v_case_str, $2, $3, yylval.lineno); }
	| DEFAULT IDENTIFIER							{ $$ = new_property(v_switch_default, $1, $2, yylval.lineno); }
;

segment_type: SEGMENT_TYPE_U8	{ $$ = StandardUInt8; }
	| SEGMENT_TYPE_U16			{ $$ = StandardUInt16; }
	| SEGMENT_TYPE_U32			{ $$ = StandardUInt32; }
	| SEGMENT_TYPE_I8			{ $$ = StandardInt8; }
	| SEGMENT_TYPE_I16			{ $$ = StandardInt16; }
	| SEGMENT_TYPE_I32			{ $$ = StandardInt32; }
	| SEGMENT_TYPE_DOUBLE		{ $$ = StandardDouble; }
	| SEGMENT_TYPE_FLOAT		{ $$ = StandardFloat; }
	| SEGMENT_TYPE_BOOLEAN		{ $$ = StandardBoolean; }
	| SEGMENT_TYPE_CRC			{ $$ = StandardCRC; }
	| SEGMENT_TYPE_ARRAY		{ $$ = StandardArray; }
	| SEGMENT_TYPE_STRING		{ $$ = StandardString; }
	| SEGMENT_TYPE_BLOCK		{ $$ = StandardBlock; }
	| SEGMENT_TYPE_BUFFER		{ $$ = StatndartBuffer; }
;

%%
int main(int argc, char **argv) {
	FILE* fs=0;
	if(argc >1)
	{
		errno_t err;
		err = fopen_s(&fs, argv[1], "r");
		if (err != 0)
		{
			perror(argv[1]);
		}
		else
		{
			yyin = fs;
		}
	}
	else
	{
		yyin = stdin;
	}

	do { 
		yyparse();
	} while(!feof(yyin));

	if(fs) 
	{
		fclose (fs);
		yyin = stdin;
		do { 
		yyparse();
	} while(!feof(yyin));
	}

	return 0;
}

void outerror(int errcode, int lineno, const char *s) {
	printf( "Error%d on line(%d) %s\n", errcode, lineno, s);
	//exit(1);
}

void yyerror(const char* s) {
	outerror(-1, yylineno, s);
}
