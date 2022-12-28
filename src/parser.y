%define api.value.type {char*}

%{
    #include <stdio.h> //dbg
    #include <string.h>
    #include "parserFunctions.h"
    #include "globalError.h"

    int yylex();
    
    void yyerror(char *s);
    extern int yylineno;
    extern char* yytext;
%}

%token  SEMIC   ";"
%token  STR     "string without whitespace"

%%

commands:
    command_list semic_opt
    ;

command_list:
    %empty
    | command
    | command_list SEMIC command
    ;

command:
    name args   {
                    runCommand($1, $2);
                }
    ;

name:
    STR
    ;

args:
    %empty      {
                    $$ = NULL;
                }
    | args STR  {
                    if ($1 == NULL)
                    {
                        $$ = $2;
                    }
                    else
                    {
                        $$ = concatArgs($1, $2);
                    }
                }
    ;

semic_opt:
    %empty
    | SEMIC
    ;

%%

void yyerror(char *s)
{
    // printf("yytext: %s\n", yytext); //dbg


    char* msg = (char*)malloc(strlen(s) + strlen(yytext) + 24);
    sprintf(msg, "%s near unexpected token %s", s, yytext);

    setError(SYNTAX_ERROR, msg, yylineno);
}