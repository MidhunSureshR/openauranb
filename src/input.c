//
// Created by rmidhunsuresh on 10/12/20.
//

#include "input.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

color hex_to_color(const char* hex_color){
    color color_representation;
    char color_part[5];

    const char *i = hex_color; // Pointer to start of red component
    strncpy(color_part, i, 2);
    color_representation.red = strtol(color_part, NULL, 16);

    i += 2; // Pointer to start of green component
    strncpy(color_part, i, 2);
    color_representation.green = strtol(color_part, NULL, 16);

    i += 2; // Pointer to start of blue component
    strncpy(color_part, i, 2);
    color_representation.blue = strtol(color_part, NULL, 16);

    return color_representation;
}

void print_ascii_art(){
    printf("\n"
           "\n"
           " _____                                              _     \n"
           "|  _  |                                            | |    \n"
           "| | | |_ __   ___ _ __   __ _ _   _ _ __ __ _ _ __ | |__  \n"
           "| | | | '_ \\ / _ \\ '_ \\ / _` | | | | '__/ _` | '_ \\| '_ \\ \n"
           "\\ \\_/ / |_) |  __/ | | | (_| | |_| | | | (_| | | | | |_) |\n"
           " \\___/| .__/ \\___|_| |_|\\__,_|\\__,_|_|  \\__,_|_| |_|_.__/ \n"
           "      | |                                                 \n"
           "      |_|                                                 \n"
           "");
    puts("__________________________________________________________");
    puts("Open source driver to control keyboard backlight\nCompatible with ROG GL553ve models.");
}

void show_help_message(){
    puts("\nUsage\nsudo openauranb <color_code_in_hex>\nEg: sudo openauranb ff0000\n");
    puts("Note:\tMake sure you do not include the '#' in your color code.");
    puts("\t'#' symbol probably has special meaning in your shell.");
}

void check_arguments(const int numberArgs){
    if(numberArgs < 2){
        show_help_message();
        exit(0);
    }
}