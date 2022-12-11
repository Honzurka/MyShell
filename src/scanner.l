/*%option header-file="scanner.h"*/

%{
    #include "parser.tab.h"
    #include <stdio.h>
    #include <string.h>
%}

%%
    /* TODO # comments*/

[ \t]+	/* whitespace */

";"             {
                    return SEMIC;
                }
[^ \t\n;]+      {
                    yylval = strdup(yytext);
                    return STR;
                }

.   { fprintf(stderr, "unknown char: %s\n", yytext); }

%%
