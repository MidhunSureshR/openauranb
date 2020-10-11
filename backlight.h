/*
 * Libusb code to control backlight
 */

#ifndef OPENAURANB_BACKLIGHT_H
#define OPENAURANB_BACKLIGHT_H

#include <stdio.h>
#include <libusb-1.0/libusb.h>

const int keyboardHID_PID  = 6228;
const int keyboardHID_VID =  2821;

void setColor();

#endif //OPENAURANB_BACKLIGHT_H
