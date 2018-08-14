
%option noyywrap yylineno


EXP ([Ee][-+]?[0-9]+)
UCN (\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})
CHS (\xE9\xBE[\x80-\xA5]|(\xE4[\xB8-\xBF]|\xE9[\x80-\xBD]|[\xE5-\xE8][\x80-\xBF])[\x80-\xBF])

%{
#include "stdafx.h"
#include <stdio.h> 

#include "dpd.h"
#include "DPD.tab.h"


%}

%%

"#".*\n					{ yylval.commentlist = new_comment(yytext); return COMMENT; }

\'([^'\\]|\\['"?\\abfnrtv]|\\[0-7]{1,3}|\\[xX][0-9a-fA-F]+|{UCN})*\'	{  return VALUE_STRING; }

"Protocol"				{ return PROTOCOL; }
"Segment"				{ return SEGMENT; }
"="						{ return EQUAL; }
"==" |
">=" |
"<=" |
">" |
"<"						{ return CMP; }

"StandardUInt8"			{ return SEGMENT_TYPE_U8; }
"StandardUInt16"		{ return SEGMENT_TYPE_U16; }
"StandardUInt32"		{ return SEGMENT_TYPE_U32; }
"StandardInt8"			{ return SEGMENT_TYPE_I8; }
"StandardInt16"			{ return SEGMENT_TYPE_I16; }
"StandardInt32"			{ return SEGMENT_TYPE_I32; }
"RandomInt"				{ return SEGMENT_TYPE_RI; }
"StandardDouble"		{ return SEGMENT_TYPE_DOUBLE; }
"StandardFloat"			{ return SEGMENT_TYPE_FLOAT; }
"Boolean"				{ return SEGMENT_TYPE_BOOLEAN; }
"CRC"					{ return SEGMENT_TYPE_CRC; }
"Array"					{ return SEGMENT_TYPE_ARRAY; }
"String"				{ return SEGMENT_TYPE_STRING; }
"Block"					{ return SEGMENT_TYPE_BLOCK; }
"Buffer"				{ return SEGMENT_TYPE_BUFFER; }

"ByteOrder" |
"Format" |
"Tail" |
"Code" |
"Count" |
"Child" |
"Algorithm" |
"BitCount" |
"Range"					{ yylval.propertyname = _strdup(yytext); return SEGMENT_PROPERTY; }

"True" |
"False"	|
"Small" |
"Big" |
"Primitive" |
"Inversion" |
"Complement"			{ yylval.propertyvalue = _strdup(yytext); yylval.lineno = yylineno; return VALUE_PROPERTY; }


"Switch"				{ yylval.propertyname = _strdup(yytext); return SWITCH; }
"Case"					{ yylval.propertyname = _strdup(yytext); return CASE; }
"Default"				{ yylval.propertyname = _strdup(yytext); return DEFAULT; }

"End"					{ return END; }

0[0-7]* |
[1-9][0-9]* |
0[Xx][0-9a-fA-F]+		{ yylval.propertyvalue = _strdup(yytext); yylval.lineno = yylineno; return VALUE_INT; }

-?([0-9]*\.[0-9]+|[0-9]+\.){EXP}? |
[0-9]+{EXP}				{ yylval.propertyvalue = _strdup(yytext); yylval.lineno = yylineno; return VALUE_FLOAT; }

"("[ \t]*(0|[1-9][0-9]*)[ \t]*,[ \t]*(0|[1-9][0-9]*)[ \t]*")"	{ yylval.propertyvalue = _strdup(yytext); yylval.lineno = yylineno; return VALUE_RANGE; }

([a-zA-Z_]|{CHS})([a-zA-Z0-9_]|{CHS})*	{ yylval.id = _strdup(yytext); yylval.lineno = yylineno; return IDENTIFIER; }

\r\n					{ ; }
\n						{ ; }
[ \t]					{ ;	}

[\xEF\xBB\xBF]			{ ; }
.						{ outerror(1, yylineno, yytext); }
%%