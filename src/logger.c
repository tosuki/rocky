#include "logger.h"


#include <stdio.h>

void print_colored(const char *message, const char *color) {
    printf("%s%s\033[0m\n", color, message);
}

void logger_info(char* message) {
    print_colored(message, "\033[1;32m");
}

void logger_error(char* message) {
    print_colored(message, "\033[1;31m");
}

void logger_warn(char* message) {
    print_colored(message, "\033[1;33m");
}

