#include "error.h"
#include <string.h>
#include <libusb-1.0/libusb.h>


 void print_error(const int error_code){
    const char* error_string = libusb_error_name(error_code);
    char pretty_error[50];
    strcpy(pretty_error,"\nProgram encountered an error : ");
    strncat(pretty_error, error_string, strlen(error_string));
    puts(pretty_error);
}
