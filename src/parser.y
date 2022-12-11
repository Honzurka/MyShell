%define api.value.type {char*}

%{
    #include <stdio.h> //dbg
    #include "parserFunctions.h"

    int yylex();
    void yyerror(char *s);
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
    fprintf(stderr, "error: %s\n", s); //syntax error - TODO: format
}