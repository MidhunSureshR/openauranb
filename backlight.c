//
// Created by rmidhunsuresh on 10/11/20.
//

#include "backlight.h"
#include "device.h"

static void set_color_coded_packet(unsigned char *packet, color *keyboard_color) {
    packet[4] = keyboard_color->red;
    packet[5] = keyboard_color->green;
    packet[6] = keyboard_color->blue;
}

static void send_color_packet(color *keyboard_color){
    // This packet informs the device about the color
    unsigned char packet_color[] = {
            0x5d, 0xb3, 0x00, 0x00,
            //Color goes next
            0xff /*red component*/, 0x00 /*green component*/, 0x00 /*blue component*/,
            0x00,0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00
    };
    set_color_coded_packet(packet_color, keyboard_color);
    send_packet(packet_color);
}

static void send_set_packet(){
    // This packet sets the color
    unsigned char packet_set[] = {
            //This packet needs to be sent next to set the color.
            0x5d, 0xb5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00
    };
    send_packet(packet_set);
}

static void send_apply_packet(){
    // This packet must be sent to apply the keyboard color
    // Failure to send this packet will mean that the color
    // does not retain after reboot.
    unsigned char packet_apply[] = {
            0x5d, 0xb4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00
    };
    send_packet(packet_apply);
}

void set_color(color keyboard_color){
    // Prep the device for communication
    acquire_device();
    // Send the packets needed to change the color
    send_color_packet(&keyboard_color);
    send_set_packet();
    send_apply_packet();
    // Deallocate resources used for communication
    release_device();
}