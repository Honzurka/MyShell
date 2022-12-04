%define api.value.type {char*}

%{
    #include <stdio.h>
    int yylex();
    void yyerror(char *s);
%}

%token  SEMIC   ";"
%token  STR     //"string without whitespace"

// %type<char*> name args

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
        printf("name: %s args: %s\n", $1, $2);
        
        //free strduped strings
        free($1);
        free($2);
        }
    ;

name:
    STR
    ;

args:
    %empty
    | args STR  {
                    //TODO: strcat + free
                    printf("new arg: %s\n", $2);
                    $$ = $2;
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