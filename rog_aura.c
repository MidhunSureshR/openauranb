#include <stdio.h>
#include <libusb-1.0/libusb.h>

#define rogAuraHID_PID 6228
#define rogAuraHID_VID 2821

int isROGAura(libusb_device *device){
    struct libusb_device_descriptor hardwareINFO;
    libusb_get_device_descriptor(device,&hardwareINFO);
    printf("\tChecking device with product_id = %04x\n and device address = %d\n",hardwareINFO.idProduct,libusb_get_device_address(device));
    if(hardwareINFO.idProduct == rogAuraHID_PID && hardwareINFO.idVendor == rogAuraHID_VID){
        return 1;
    }
    else
        return 0;
}

void sendBytes(char packet[],libusb_device_handle *handle){
    int isError;
    //Control transfer arguments are so forth defined:
    uint8_t       bmReqType = 0x21;
    uint8_t            bReq = 0x09;
    uint16_t           wVal = 0x035D;
    uint16_t         wIndex = 0;
    uint16_t           wLen = 17;
    unsigned int     to = 0;

    isError = libusb_control_transfer(handle,bmReqType,bReq,wVal,wIndex,packet,wLen,to);
    if(isError == LIBUSB_ERROR_TIMEOUT ){
        printf("ERROR: Transfer Timed Out.\n");
    }
    else if(isError == LIBUSB_ERROR_PIPE){
        printf("ERROR: Control request was not supported by the device.\n");
    }
    else if(isError == LIBUSB_ERROR_NO_DEVICE){
        printf("ERROR: Device has been disconnected.\n");
    }
    else if(isError == LIBUSB_ERROR_BUSY){
        printf("ERROR: Called from event handling context.\n");
    }
    else if(isError == LIBUSB_ERROR_INVALID_PARAM){
        printf("ERROR: Transer size larger than hardware can support.\n");
    }
    else if(isError == LIBUSB_ERROR_OTHER){
        printf("ERROR: An uncommon error has occurred.");
    }
    else{
        printf("Number of transferred bytes = %d\n",isError);
    }
}
void sendControlTransfer(libusb_device_handle *handle){
    char packet_bytes_color[] = {
            0x5d, 0xb3, 0x00, 0x00,
            //Color goes next
            //default color #ffffff
            0xff, 0xff, 0xff,

            0x00,0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00
    };
    char packet_bytes_set[] = {
            //This packet need to be send next to set the color.
            0x5d, 0xb5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00
    };
    sendBytes(packet_bytes_color,handle);
    sendBytes(packet_bytes_set,handle);

}

void handleDevice(libusb_device *device, uint8_t interfaceNumber){

    libusb_device_handle *handle;
    int isError;

    // We want to procedurally find and open the device.
    isError = libusb_open(device,&handle);
    if(isError !=0 ){
        printf("ERROR:%s\n",libusb_error_name(isError));
    }
    printf("Successful in creating a handle for device...\n");

    //De-attach kernel drivers before USB communication
    if(libusb_set_auto_detach_kernel_driver(handle,1) == LIBUSB_SUCCESS){
        printf("Auto detach kernel mode set.\n");
    }

    //Claim the interface through which communication will occur.
    isError = libusb_claim_interface(handle,interfaceNumber);
    if(isError != 0){
        printf("Detected error. Will call libusb_close.\n");
        libusb_close(handle);
        // return;
    }

    if(isError == LIBUSB_ERROR_NOT_FOUND){
        printf("ERROR: Requested interface does not exist.\n");
        return;
    }
    else if(isError == LIBUSB_ERROR_BUSY){
        printf("ERROR: Another program or driver has claimed the interface.\n");
        return;
    }
    else if(isError == LIBUSB_ERROR_NO_DEVICE){
        printf("ERROR: Device has been disconnected.\n");
        return;
    }
    printf("Successfully claimed an interface for I/O operations.\n");
    printf("Attempting to send a control transfer...\n");
    printf("Starting URB manipulation.\n");
    sendControlTransfer(handle);

    libusb_release_interface(handle,interfaceNumber);

    //Close handle
    libusb_close(handle);

}

int main()
{
    int isError;
    libusb_device **all_usb_devices;
    libusb_device *asusHIDdevice;
    libusb_device *device;
    const struct libusb_interface *libinterface;
    const struct libusb_interface_descriptor *id;
    struct libusb_config_descriptor *configDescriptor;
    ssize_t number_of_devices;
    ssize_t i;

    printf("Asus ROG AURA USB Tester\n");
    printf("Press any key to start.\n");
    getchar();

    //Try to initialize the libusb library
    printf("Initializing libusb.\n");
    isError = libusb_init(NULL);
    if(isError != 0){
        printf("ERROR:Could not initialize libusb library.\n");
        return 0;
    }
    printf("Successfully initialized libusb library.\n");

    //Lets try to find our HID device that controls backlight LEDs.
    number_of_devices = libusb_get_device_list(NULL,&all_usb_devices);
    printf("Number of usb devices found = %d\n",number_of_devices);
    if(number_of_devices < 0){
        printf("ERROR: Could not fetch usb device list.\n");
        return 0;
    }
    printf("Successfully found all USB devices.\n");
    for(i=0;i<number_of_devices;i++){
        device = all_usb_devices[i];

        if(isROGAura(device)){
            printf("Found ROG AURA HID-compliant vendor-defined device.\n");
            asusHIDdevice = device;
            break;
        }


    }


    printf("Device address = %d\n",libusb_get_device_address(asusHIDdevice));
    libusb_get_active_config_descriptor(asusHIDdevice,&configDescriptor);
    libinterface = &configDescriptor->interface[0];
    id = &libinterface->altsetting[0];




    printf("Number of interfaces available on this device = %d\n",configDescriptor->bNumInterfaces);
    printf("Interface number is found as %d\n",id->bInterfaceNumber);

    handleDevice(asusHIDdevice,id->bInterfaceNumber);

    //Deinitialize the library before exit
    printf("Deinitializing libusb list.\n");
    libusb_free_device_list(all_usb_devices,1);
    libusb_exit(NULL);

    return 0;
}