%{
#include <stdio.h>
#include <stdlib.h>
int res = 0;
int yylex();
int yyparse();
void yyerror(const char *s);
%}

%union {
    int ival;
}

%define parse.error verbose
%token <ival> INT
%type <ival> expr

%left '+'
%left '*'

%start expr

%%
expr:
      expr '+' expr   { $$ = $1 + $3; res = $$;}
    | expr '*' expr   { $$ = $1 * $3; res = $$;}
    | '(' expr ')'    { $$ = $2; }
    | INT             { $$ = $1; }
    ;

%%

int main() {
    if (yyparse() == 0)
        printf("%d\n", res);
}

void yyerror(const char *s) {
    printf("Error %s\n", s);
	exit(-1);
}
