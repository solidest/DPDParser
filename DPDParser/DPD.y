
%{
#include <stdio.h>
#include <stdlib.h>
#include "stdafx.h"
#include "dpd.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;
extern char* yytext;

%}

%union {
	char* s;
	enum segmenttype stype;
	struct comment* commentlist;
	struct protocol *protocollist;
	struct segment *segmentlist;
	struct property *propertylist;
}

%token PROTOCOL
%token SEGMENT SEGMENT_TYPE_U8 SEGMENT_TYPE_U16 SEGMENT_TYPE_U32 SEGMENT_TYPE_I8 SEGMENT_TYPE_I16 SEGMENT_TYPE_I32 SEGMENT_TYPE_IR
%token SEGMENT_TYPE_FLOAT SEGMENT_TYPE_DOUBLE SEGMENT_TYPE_BOOLEAN SEGMENT_TYPE_CRC SEGMENT_TYPE_ARRAY SEGMENT_TYPE_SWITCH
%token SEGMENT_TYPE_STRING SEGMENT_TYPE_BLOCK SEGMENT_TYPE_BUFFER EQUAL CMP

%token <s> SEGMENT_PROPERTY
%token <s> VALUE_PROPERTY
%token <s> VALUE_INT
%token <s> VALUE_FLOAT
%token <s> VALUE_STRING
%token <s> VALUE_RANGE
%token <s> DEFAULT
%token <s> CASE
%token <s> VALUE_BOOL
%token <s> IDENTIFIER
%token <commentlist> COMMENT
%token END

%type <stype> segment_type
%type <commentlist>  commentlist
%type <protocollist>  protocol protocollist
%type <segmentlist> segment segmentlist
%type <propertylist> property propertylist

%start protocollist

%%

protocollist: 
	| protocollist error protocol
	| protocollist protocol
;

protocol:  commentlist PROTOCOL IDENTIFIER segmentlist END{ ; }
	| commentlist PROTOCOL IDENTIFIER error END{ ; }
;

commentlist:							{ $$ = NULL; }
	| commentlist COMMENT				{ $$ = union_comment($1, $2); }
;

segmentlist: 
	| segmentlist segment
;

segment: commentlist SEGMENT IDENTIFIER segment_type propertylist { ; }
;


propertylist:					{ $$ = NULL; }
	| propertylist error property
	| propertylist property		{ $$ = union_property($1, $2); }
;


segment_type: SEGMENT_TYPE_U8	{ $$ = StandardUInt8; }
	| SEGMENT_TYPE_U16			{ $$ = StandardUInt16; }
	| SEGMENT_TYPE_U32			{ $$ = StandardUInt32; }
	| SEGMENT_TYPE_I8			{ $$ = StandardInt8; }
	| SEGMENT_TYPE_I16			{ $$ = StandardInt16; }
	| SEGMENT_TYPE_I32			{ $$ = StandardInt32; }
	| SEGMENT_TYPE_IR			{ $$ = StandardIntRandom; }
	| SEGMENT_TYPE_DOUBLE		{ $$ = StandardDouble; }
	| SEGMENT_TYPE_FLOAT		{ $$ = StandardFloat; }
	| SEGMENT_TYPE_BOOLEAN		{ $$ = StandardBoolean; }
	| SEGMENT_TYPE_CRC			{ $$ = StandardCRC; }
	| SEGMENT_TYPE_ARRAY		{ $$ = StandardArray; }
	| SEGMENT_TYPE_STRING		{ $$ = StandardString; }
	| SEGMENT_TYPE_BLOCK		{ $$ = StandardBlock; }
	| SEGMENT_TYPE_BUFFER		{ $$ = StatndartBuffer; }
	| SEGMENT_TYPE_SWITCH		{ $$ = StatndartSwitch; }
;

property: SEGMENT_PROPERTY EQUAL VALUE_PROPERTY		{ $$ = new_property(v_property, $1, $3, @3.first_line); }
	| SEGMENT_PROPERTY EQUAL VALUE_INT				{ $$ = new_property(v_int, $1, $3, @3.first_line); }
	| SEGMENT_PROPERTY EQUAL VALUE_BOOL				{ $$ = new_property(v_bool, $1, $3, @3.first_line); }
	| SEGMENT_PROPERTY EQUAL VALUE_FLOAT			{ $$ = new_property(v_float, $1, $3, @3.first_line); }
	| SEGMENT_PROPERTY EQUAL VALUE_STRING			{ $$ = new_property(v_str, $1, $3, @3.first_line); }
	| SEGMENT_PROPERTY EQUAL VALUE_RANGE			{ $$ = new_property(v_range, $1, $3, @3.first_line); }
	| DEFAULT EQUAL VALUE_INT						{ $$ = new_property(v_int, $1, $3, @3.first_line); }
	| DEFAULT EQUAL VALUE_FLOAT						{ $$ = new_property(v_float, $1, $3, @3.first_line); }
	| DEFAULT EQUAL VALUE_STRING					{ $$ = new_property(v_str, $1, $3, @3.first_line); }
	| DEFAULT EQUAL VALUE_BOOL						{ $$ = new_property(v_bool, $1, $3, @3.first_line); }
	| IDENTIFIER CASE VALUE_INT IDENTIFIER			{ $$ = new_property(v_case_int, $2, $3, @3.first_line); }
	| IDENTIFIER CASE VALUE_STRING IDENTIFIER		{ $$ = new_property(v_case_str, $2, $3, @3.first_line); }
	| IDENTIFIER CASE VALUE_BOOL IDENTIFIER			{ $$ = new_property(v_case_bool, $2, $3, @3.first_line); }
	| CASE VALUE_INT IDENTIFIER						{ $$ = new_property(v_case_int, $2, $3, @3.first_line); }
	| CASE VALUE_STRING IDENTIFIER					{ $$ = new_property(v_case_str, $2, $3, @3.first_line); }
	| CASE VALUE_BOOL IDENTIFIER					{ $$ = new_property(v_case_bool, $2, $3, @3.first_line); }
	| DEFAULT IDENTIFIER							{ $$ = new_property(v_switch_default, $1, $2, @2.first_line); }
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
	printf( "Error%d on line(%d) %s (%s)\n", errcode, lineno, s,yytext);
	//exit(1);
}

void yyerror(const char* s) {
	outerror(-1, yylineno, s);
}
