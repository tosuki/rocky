#include "logger.h"

#include <stdarg.h>
#include <stdio.h>

void print_colored(const char *color, const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("%s", color);
    vprintf(format, args);
    printf("\033[0m\n");
}

void logger_info(const char* message, ...) {
    print_colored("\033[1;32m", message);
}

void logger_error(const char* message, ...) {
    print_colored("\033[1;31m", message);
}

void logger_warn(const char* message, ...) {
    print_colored("\033[1;33m", message);
}

