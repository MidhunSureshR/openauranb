/*
 * Openauranb
 * Open Source Driver to control ROG GL553ve keyboard backlight
 * <rmidhunsuresh@gmail.com>
 */

#include "backlight.h"
#include "input.h"

int main(int argc,char* argv[]){
    print_ascii_art();
    check_arguments(argc);
    color key_color = hex_to_color(argv[1]);
    set_color(key_color);
    return 0;
}
