
%option noyywrap yylineno


EXP ([Ee][-+]?[0-9]+)
UCN (\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})

%{
#include "stdafx.h"
#include <stdio.h> 

#include "dpd.h"
#include "DPD.tab.h"


%}

%%

"#".*\n					{ yylval.s = _strdup(yytext); return COMMENT; }

\'([^'\\]|\\['"?\\abfnrtv]|\\[0-7]{1,3}|\\[xX][0-9a-fA-F]+|{UCN})*\'	{ yylval.s = _strdup(yytext); return VALUE_STRING; }

"Protocol"				{ return PROTOCOL; }
"Segment"				{ return SEGMENT; }
"="						{ return EQUAL; }

"Array" |
"StandardUInt8" |
"StandardUInt16" |
"StandardUInt32" |
"StandardInt8" |
"StandardInt16" |
"StandardDouble" |
"StandardFloat" |
"Boolean" |
"CRC" |
"Array"	|
"String" |
"Block"					{ return SEGMENT_TYPE; }

"ByteOrder" |
"Format" |
"Tail" |
"Code" |
"Count" |
"Child" |
"Algorithm" |
"BitCount" |
"Range"					{ return SEGMENT_PROPERTY; }

"True" |
"False"	|
"Small" |
"Big" |
"Primitive" |
"Inversion" |
"Complement"			{ return VALUE_PROPERTY; }


"Switch"				{ return SWITCH; }
"Case"					{ return CASE; }
"Default"				{ return DEFAULT; }

"End"					{ return END; }

0[0-7]* |
[1-9][0-9]* |
0[Xx][0-9a-fA-F]+		{ yylval.s = _strdup(yytext); return VALUE_INT; }

-?([0-9]*\.[0-9]+|[0-9]+\.){EXP}? |
[0-9]+{EXP}				{ yylval.s = _strdup(yytext); return VALUE_FLOAT; }

"("[ \t]*(0|[1-9][0-9]*)[ \t]*,[ \t]*(0|[1-9][0-9]*)[ \t]*")"	{ yylval.s = _strdup(yytext); return VALUE_RANGE; }

[a-zA-Z_][a-zA-Z0-9_]*	{ yylval.s = _strdup(yytext); return IDENTIFIER; }

\r\n					{ ; }
\n						{ ; }
[ \t]					{ ;	}

[\xEF\xBB\xBF]			{ ; }
.						{ outerror(1, yylineno, yytext); }
%%