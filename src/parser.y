%{
    #include <stdio.h>
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
    name args
    ;

name:
    STR
    ;

args:
    %empty
    | args STR
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