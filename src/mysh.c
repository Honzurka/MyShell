#include "parser.tab.h"
#include "scanner.h"
#include <err.h>

int main(int argc, char** argv)
{
    char* line = "a;b;";

    // yy_scan_string(line);
    YY_BUFFER_STATE buf = yy_scan_string(line);

    if (yyparse() != 0)
    {
        exit(1);
        // error is already caught in parser --- exiting might be enough
        // err(1, "yyparse error\n"); //not sure about status code
    }

    yy_delete_buffer(buf);
}