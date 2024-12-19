/******************************************************************* 
write an appropriate header comment, see the assignment
*******************************************************************/
%option yylineno
%option noyywrap
%{
#include "lexer.h"

%}

%%
 /* Keyword */
BEGIN     { return TOK_BEGIN; }
BREAK     { return TOK_BREAK; }
CONTINUE  { return TOK_CONTINUE; }
DOWNTO    { return TOK_DOWNTO; }
ELSE      { return TOK_ELSE; }
END       { return TOK_END; }
FOR       { return TOK_FOR; }
IF        { return TOK_IF; }
LET       { return TOK_LET; }
PROGRAM   { return TOK_PROGRAM; }
READ      { return TOK_READ; }
THEN      { return TOK_THEN; }
TO        { return TOK_TO; }
VAR       { return TOK_VAR; }
WHILE     { return TOK_WHILE; }
WRITE     { return TOK_WRITE; }

 /* DataType */
INTEGER   { return TOK_INTEGER; }
REAL      { return TOK_REAL; }

 /* Punctuation */ 
[;]      { return TOK_SEMICOLON; }
[:"]       { return TOK_COLON; }
[(]      { return TOK_OPENPAREN; }
[)]       { return TOK_CLOSEPAREN; }

 /* Operators */
[+]       { return TOK_PLUS; }
[-]       { return TOK_MINUS; }
[*]       { return TOK_MULTIPLY; }
[/]       { return TOK_DIVIDE; }
":="      { return TOK_ASSIGN; }
[=]       { return TOK_EQUALTO; }
[<]       { return TOK_LESSTHAN; }
[>]       { return TOK_GREATERTHAN; }
"<>"      { return TOK_NOTEQUALTO; }
"MOD"     { return TOK_MOD; }
"NOT"     { return TOK_NOT; }
"OR"      { return TOK_OR; }
"AND"     { return TOK_AND; }



'[^'\n]+{80,200}'   {return TOK_UNKNOWN;}


'[^'\n]{0,80}' { return TOK_STRINGLIT; }


[A-Z][A-Z0-9]{0,7} { return TOK_IDENT; }


[0-9]+    { return TOK_INTLIT; }

[0-9]+"."[0-9]+ { return TOK_FLOATLIT; }

[ \t]+              
[\r\n]  


 /*https://stackoverflow.com/questions/25395251/detecting-and-skipping-line-comments-with-flex*/

"{"[^}]*"}"        { /* Ignore comments */ }       { /* Ignore comments */ }

.         { return TOK_UNKNOWN; }

[A-Z]*  {return TOK_UNKNOWN;}


 /* Found an unknown character */

.         { return TOK_UNKNOWN; }

 /* Recognize end of file */

<<EOF>>   { return TOK_EOF; }
