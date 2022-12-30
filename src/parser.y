%code requires { // emitted to header file
    #include <stdio.h> //dbg
    #include <string.h>
    #include "parserFunctions.h"
    #include "globalError.h"
}

%code { // emitted source file
    int yylex();
    
    void yyerror(char *s);
    extern int yylineno;
    extern char* yytext;
}

%union {
    char* str;
    redirect_t redir;
    command_with_redirects_t cwr;
}

%token      LANGLE  "<"
%token      RANGLE  ">"
%token      RANGLE2 ">>"
%token      PIPE    "|"
%token      SEMIC   ";"
%token<str> STR     "string without whitespace"

%type<str> name args
%type<redir> redirect redirect_list
%type<cwr> command_with_redirects

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
    %empty                      {
                                    redirect_t result = {0};
                                    $$ = result;
                                }
    | redirect_list redirect    { $$ = combineRedirects($1, $2); }
    ;

redirect:
    LANGLE STR      { $$ = createRedirect($2, INPUT); }
    | RANGLE STR    { $$ = createRedirect($2, OUTPUT); }
    | RANGLE2 STR   { $$ = createRedirect($2, APPEND); }
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

command_with_redirects: //TODO-NEXT: chain of commands with redirects------- linked-list vs ??? 
    redirect_list name redirect_list args redirect_list
    {
        // combine redirs - last one is most important
        redirect_t finalRedir = $1;
        finalRedir = combineRedirects(finalRedir, $3);
        finalRedir = combineRedirects(finalRedir, $5);

        // create command
        command_t cmd = createCommand($2, $4); //will be run later

        $$ = createCommandWithRedirects(cmd, finalRedir);

        // ...
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
                        $$ = concatArgs($1, $2); // maybe todo: could be improved with types
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