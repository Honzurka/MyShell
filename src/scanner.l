
%{
    #include "parser.tab.h"
    #include <stdio.h>
    #include <string.h>
%}

%option yylineno
%option nounput
%option noinput

%%
\n  { /*increments yylineno*/ }

#.* { /* comment */ }

[ \t]+	/* whitespace */

"<"             {
                    return LANGLE;
                }
">"             {
                    return RANGLE;
                }
">>"            {
                    return RANGLE2;
                }
"|"             {
                    return PIPE;
                }
";"             {
                    return SEMIC;
                }
[^ \t\n;<>|]+      {
                    yylval.str = strdup(yytext);
                    if (yylval.str == NULL)
                    {
                        fprintf(stderr, "out of memory\n");
                        exit(1);
                    }

                    return STR;
                }

    /*.   { fprintf(stderr, "unknown char: %s\n", yytext); } --- rule not matched */

%%
