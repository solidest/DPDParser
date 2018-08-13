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
	char * s;
	struct value* v;
	struct ast *a;
	struct protocol *pro;
	struct segment *seg;
	struct property *proper;
}

%token PROTOCOL
%token SEGMENT
%token SEGMENT_TYPE
%token EQUAL
%token SEGMENT_PROPERTY
%token <s> VALUE_PROPERTY
%token VALUE_INT
%token VALUE_FLOAT
%token VALUE_STRING
%token VALUE_RANGE
%token DEFAULT
%token SWITCH
%token CASE
%token IDENTIFIER
%token COMMENT
%token END

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

segment: commentlist SEGMENT IDENTIFIER SEGMENT_TYPE propertylist { ; }
;

commentlist:
	| commentlist COMMENT
;

propertylist: 
	| propertylist property
;

property: SEGMENT_PROPERTY EQUAL VALUE_PROPERTY { printf("%s", $3); }
	| SEGMENT_PROPERTY EQUAL VALUE_INT 
	| SEGMENT_PROPERTY EQUAL VALUE_FLOAT 
	| SEGMENT_PROPERTY EQUAL VALUE_STRING 
	| SEGMENT_PROPERTY EQUAL VALUE_RANGE 
	| DEFAULT EQUAL VALUE_INT
	| DEFAULT EQUAL VALUE_FLOAT
	| DEFAULT EQUAL VALUE_STRING
	| CASE VALUE_INT IDENTIFIER 
	| CASE VALUE_STRING IDENTIFIER
	| DEFAULT IDENTIFIER
;


%%
int main() {
	yyin = stdin;
	do { 
		yyparse();
	} while(!feof(yyin));
	return 0;
}

void outerror(int errcode, int lineno, const char *s) {
	printf( "Error%d on line(%d) %s\n", errcode, lineno, s);
	//exit(1);
}

void yyerror(const char* s) {
	outerror(-1, yylineno, s);
}
