%option header-file="scanner.h"

%{
    #include "parser.tab.h"
    #include <stdio.h> //debug print
%}

%%
    /* TODO: ignore {whitespace} */
    /* TODO # comments*/

";" { printf("semic\n"); return SEMIC; }

    /* . { printf("default: %c\n", yytext); } */

%%
