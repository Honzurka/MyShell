%code requires { // emitted to header file
    #include "parserFunctions.h"
    #include "commandQueue.h"
    #include "pipeQueue.h"
}

%code { // emitted source file
    #include <stdio.h>
    #include <string.h>
    #include "globalError.h"
    
    int yylex();
    
    void yyerror(char *s);
    extern int yylineno;
    extern char* yytext;
}

%union {
    char* str;
    redirect_t redir;
    command_with_redirects_t cwr;
    command_head_t* commandHead;
    pipe_head_t* pipeHead;
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
%type<commandHead> command_list_req command_list_opt commands
%type<pipeHead> pipe_list

%%

start:
    pipe_list commands  {
                            pipe_node_t* node = createPipeNode($2);
                            addPipeNode($1, node);
                            runPipesInQueue($1);
                        }
    ;

pipe_list:
    %empty                              { $$ = createPipeHead(); }
    | pipe_list command_list_req PIPE   {
                                            pipe_node_t* node = createPipeNode($2);
                                            addPipeNode($1, node);
                                            $$ = $1;
                                        }
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
    command_list_opt semic_opt  { $$ = $1; }
    ;

command_list_opt:
    %empty              { $$ = createCommandHead(); }
    | command_list_req  { $$ = $1; }
    ;

command_list_req:
    command_with_redirects  {
                                command_head_t* head = createCommandHead();
                                command_node_t* node = createCommandNode($1);
                                addCommandNode(head, node);
                                $$ = head;
                            }
    | command_list_opt SEMIC command_with_redirects {
                                                        command_node_t* node = createCommandNode($3);
                                                        addCommandNode($1, node);
                                                        $$ = $1;
                                                    }
    ;

command_with_redirects:
    redirect_list name redirect_list
    {
        redirect_t finalRedir = $1;
        finalRedir = combineRedirects(finalRedir, $3);

        command_t cmd = createCommand($2, NULL); // NULL args

        $$ = createCommandWithRedirects(cmd, finalRedir);
    }
    | redirect_list name redirect_list args redirect_list
    {
        // combine redirs - last one is most important
        redirect_t finalRedir = $1;
        finalRedir = combineRedirects(finalRedir, $3);
        finalRedir = combineRedirects(finalRedir, $5);

        command_t cmd = createCommand($2, $4);

        $$ = createCommandWithRedirects(cmd, finalRedir);
    }
    ;

name:
    STR
    ;

args:
    STR         { $$ = $1; }
    | args STR  { $$ = concatArgs($1, $2); }
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