%{
    #include <stdio.h>
    int yylex();
    void yyerror(char *s);
%}

%token  SEMIC   ";"
%token  ARGS    "program arguments"
%token  NAME    "program name"

%%

commands:
    command_list semic_opt
    ;

command_list:
    %empty
    | command_list SEMIC command
    ;

command:
    NAME ARGS
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