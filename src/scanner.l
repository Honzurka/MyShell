
%{
    #include "parser.tab.h"
    #include <stdio.h>
    #include <string.h>
%}

%option yylineno

%%
\n  { /*increments yylineno*/ }

#.* { /* comment */ }

[ \t]+	/* whitespace */

";"             {
                    return SEMIC;
                }
[^ \t\n;]+      {
                    yylval = strdup(yytext);
                    if (yylval == NULL)
                    {
                        fprintf(stderr, "out of memory\n");
                        exit(1);
                    }

                    return STR;
                }

.   { fprintf(stderr, "unknown char: %s\n", yytext); }

%%
