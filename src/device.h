#ifndef OPENAURANB_DEVICE_H
#define OPENAURANB_DEVICE_H
#include <libusb-1.0/libusb.h>
#include <stdio.h>

extern const uint16_t KEYBOARD_PID;
extern const uint16_t KEYBOARD_VID;

void send_packet(unsigned char* packet);

void acquire_device();

void release_device();

#endif //OPENAURANB_DEVICE_H
