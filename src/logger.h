#include <stdio.h>

void print_colored(const char *color, const char *format, va_list args); 
void logger_error(const char* message, ...);
void logger_warn(const char* message, ...);
void logger_info(const char* message, ...);
