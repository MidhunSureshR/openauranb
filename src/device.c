//
// Created by rmidhunsuresh on 10/11/20.
//

#include "device.h"
#include "error.h"
#include <stdbool.h>

const uint16_t KEYBOARD_PID  = 6228;
const uint16_t KEYBOARD_VID =  2821;

typedef struct{
    uint16_t pid;
    uint16_t vid;
} device_data;

typedef struct{
    uint8_t bInterfaceNumber;
    libusb_device_handle *dev_handle;
}device;

device device_properties;

static libusb_device *get_device();

static device_data get_device_data(libusb_device *device){
    struct libusb_device_descriptor descriptor;
    int error_code = libusb_get_device_descriptor(device, &descriptor);
    handle_error(error_code);
    device_data data;
    data.pid = descriptor.idProduct;
    data.vid = descriptor.idVendor;
    return data;
}

static bool is_aura_keyboard(libusb_device *device){
    device_data dev_data = get_device_data(device);
    return dev_data.vid == KEYBOARD_VID && dev_data.pid == KEYBOARD_PID;
}

typedef struct libusb_interface_descriptor interface_descriptor;
typedef struct libusb_config_descriptor config_descriptor;

static const interface_descriptor* get_interface_descriptor(config_descriptor *configDescriptor){
    const interface_descriptor *id;
    id = &configDescriptor->interface[0].altsetting[0];
    return id;
}

static config_descriptor* get_config_descriptor(struct libusb_device *device){
    config_descriptor *configDescriptor;
    int error_code = libusb_get_active_config_descriptor(device, &configDescriptor);
    handle_error(error_code);
    return configDescriptor;
}

static uint8_t get_interface_number(libusb_device *device){
    config_descriptor *configDescriptor = get_config_descriptor(device);
    const interface_descriptor *id = get_interface_descriptor(configDescriptor);
    const uint8_t bInterfaceNumber = id->bInterfaceNumber;
    libusb_free_config_descriptor(configDescriptor);
    return bInterfaceNumber;
}

void acquire_device(){
    int error_code = libusb_init(NULL);
    handle_error(error_code);

    libusb_device *dev = get_device();
    assert(dev, exit_with_message, "Error: Failed at get_device()");

    error_code = libusb_open(dev, &device_properties.dev_handle);
    handle_error(error_code);

    libusb_set_auto_detach_kernel_driver(device_properties.dev_handle,1);
    device_properties.bInterfaceNumber = get_interface_number(dev);
    error_code = libusb_claim_interface(device_properties.dev_handle, device_properties.bInterfaceNumber);
    handle_error(error_code);
}

static libusb_device* get_device() {
    libusb_device **list;
    const ssize_t num_device = libusb_get_device_list(NULL, &list);
    assert(num_device > 0, handle_error, num_device);
    for (ssize_t i = 0; i < num_device; ++i) {
        libusb_device *device = list[i];
        if (is_aura_keyboard(device)) {
            libusb_free_device_list(list, 1);
            return device;
        }
    }
    return NULL;
}

void release_device(){
    assert(&device_properties.dev_handle != NULL, exit_with_message, "Error: release_device() called before acquire_device()");
    libusb_release_interface(device_properties.dev_handle, device_properties.bInterfaceNumber);
    libusb_close(device_properties.dev_handle);
    libusb_exit(NULL);
}

void send_packet(unsigned char* packet){
    assert(&device_properties.dev_handle != NULL, exit_with_message, "Error: send_packet() called before acquire_device()");
    uint8_t       bmReqType = 0x21;
    uint8_t            bReq = 0x09;
    uint16_t           wVal = 0x035D;
    uint16_t         wIndex = 0;
    uint16_t           wLen = 17;
    unsigned int     to = 0;
    int error_code = libusb_control_transfer(device_properties.dev_handle,bmReqType,bReq,wVal,wIndex,packet,wLen,to);
    assert(error_code > 0, handle_error, error_code);
}