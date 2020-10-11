/*
 * Openauranb
 * Open Source Driver to control ROG GL553ve keyboard backlight
 * <rmidhunsuresh@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>

void printLogoArt(){
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

void showHelpMessage(){
    puts("\nUsage\nsudo openauranb <color_code_in_hex>\nEg: sudo openauranb #ff0000");
}

void checkArgumentPresence(const int numberArgs){
    if(numberArgs < 2){
        showHelpMessage();
        exit(0);
    }
}

int main(int argc,char* argv[]){
    printLogoArt();
    checkArgumentPresence(argc);
    return 0;
}
