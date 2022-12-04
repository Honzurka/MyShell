%option header-file="scanner.h"

%{
    #include "parser.tab.h"
    #include <stdio.h>
%}

%%
    /* TODO # comments*/

[ \t]+	/* whitespace */

";"             { printf("semic\n"); return SEMIC; }
[^ \t\n;]+      { printf("str\n"); return STR; }

.   { fprintf(stderr, "unknown char: %s\n", yytext); }

%%
