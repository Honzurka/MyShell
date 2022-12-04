%option header-file="scanner.h"

%{
    #include "parser.tab.h"
    #include <stdio.h>
%}

%%
    /* TODO: ignore {whitespace} */
    /* TODO # comments*/

";"             { printf("semic\n"); return SEMIC; }
[^ \t\n;]+      { printf("str\n"); return STR; }

.   { fprintf(stderr, "unknown char: %s\n", yytext); }

%%
