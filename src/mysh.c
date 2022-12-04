#include "parser.tab.h"
#include "scanner.h"
#include <err.h>

int main(int argc, char** argv)
{
    char* line = "cd";

    YY_BUFFER_STATE buf = yy_scan_string(line);
    if (yyparse() != 0)
    {
        exit(1);
    }
    yy_delete_buffer(buf);
}