#include "logger.h"

#include <stdarg.h>
#include <stdio.h>

void print_colored(const char *color, const char *format, va_list args) {
    printf("%s", color);
    vprintf(format, args);
    printf("\033[0m\n");
}

void logger_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    print_colored("\033[1;32m", format, args);
    va_end(args);
}

void logger_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    print_colored("\033[1;31m", format, args);
    va_end(args);
}

void logger_warn(const char *format, ...) {
    va_list args;
    va_start(args, format);
    print_colored("\033[1;33m", format, args);
    va_end(args);
}

