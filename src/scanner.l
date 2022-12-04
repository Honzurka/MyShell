%option header-file="scanner.h"

%{
    #include "parser.tab.h"
    #include <stdio.h>
    #include <string.h>
%}

%%
    /* TODO # comments*/

[ \t]+	/* whitespace */

";"             {
                    // printf("semic: %s\n", yytext);
                    return SEMIC;
                }
[^ \t\n;]+      {
                    printf("str: %s\n", yytext);
                    printf("strPtr: %p\n", &yylval);
                    // printf("test: %d\n", malloc_usable_size(yylval));
                    // strcpy(yylval, yytext);
                    yylval = strdup(yytext); //new string must be created --------------------------- where do i delete it??----------------
                    return STR;
                }

.   { fprintf(stderr, "unknown char: %s\n", yytext); }

%%
