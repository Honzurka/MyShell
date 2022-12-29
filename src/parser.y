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

%token  LANGLE  "<"
%token  RANGLE  ">"
%token  RANGLE2 ">>"
%token  PIPE    "|"
%token  SEMIC   ";"
%token  STR     "string without whitespace"

%%

start:
    pipe_list commands
    ;

pipe_list:
    %empty
    | command_list_req PIPE
    | pipe_list command_list_req PIPE
    ;

redirect_list:
    %empty
    | redirect_list redirect
    ;

redirect:
    LANGLE STR
    | RANGLE STR
    | RANGLE2 STR
    ;

commands:
    command_list_opt semic_opt
    ;

command_list_opt:
    %empty
    | command_list_req
    ;

command_list_req:
    command_with_redirects
    | command_list_opt SEMIC command_with_redirects
    ;

command_with_redirects:
    redirect_list name redirect_list args redirect_list
    {
        runCommand($2, $4); //wont be directly executed with pipes------------------------instead it will be passed up to pipe chain
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

void yyerror(char *s) {
    char* msg = (char*)malloc(strlen(s) + strlen(yytext) + 24);
    sprintf(msg, "%s near unexpected token %s", s, yytext);

    setError(SYNTAX_ERROR, msg, yylineno);
}