%define api.value.type {char*}

%{
    #include <stdio.h>
    #include <string.h>

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
                    // TODO: process command
                    printf("name: %s args: %s\n", $1, $2);
                    
                    free($1);
                    free($2);
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
                    if ($1 == NULL) {
                        $$ = $2;
                    }
                    else {
                        int len = 0;
                        len += strlen($1);
                        len += strlen($2);

                        char* result = malloc(len + 2);
                        strcat(result, $1);
                        strcat(result, " ");
                        strcat(result, $2);

                        free($1);
                        free($2);

                        $$ = result;
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