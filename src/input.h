#ifndef OPENAURANB_INPUT_H
#define OPENAURANB_INPUT_H

#include <stdbool.h>

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
}color;

color hex_to_color(const char* hex_color);
void show_help_message();
void check_arguments(int numberArgs);
void print_ascii_art();

#endif //OPENAURANB_INPUT_H