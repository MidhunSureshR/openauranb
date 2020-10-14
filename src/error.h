#ifndef OPENAURANB_ERROR_H
#define OPENAURANB_ERROR_H

// Macro to simplify error checking
// If condition is false, call callback(arg)
#define assert(condition, callback, arg) if(!(condition)) { callback(arg); exit(1); }

#include <stdlib.h>
#include <stdio.h>

void print_error(int error_code);

static inline void handle_error(const int error_code){
    assert(error_code == 0, print_error, error_code);
}

static inline void exit_with_message(const char *message){
    puts(message);
    exit(0);
}

#endif //OPENAURANB_ERROR_H