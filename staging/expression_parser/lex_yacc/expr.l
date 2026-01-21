%{
#include <stdlib.h>
#include "expr.tab.h"
%}

%%
[0-9]+      { yylval.ival = strtoull(yytext, NULL, 10); return INT; }
[+*()]     { return yytext[0]; }
[ \t\r\n]+    ;   /* ignore whitespace */
.           ;
%%
