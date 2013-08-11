#include "dbg.h"

#include <stdarg.h>
#include <stdio.h>

void
_dbg (char *format, ...)
{
    va_list args;

    va_start (args, format);
        vprintf (format, args);
    va_end (args);
}
