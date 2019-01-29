#include <stdio.h>
#include <libusb.h>

//Product ID is coded in hexadecimal
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

void sendControlTransfer(libusb_device_handle *handle){
    char packet_bytes_color[] = {
        0x5d, 0xb3, 0x00, 0x00,
        //Color goes next
        //default color #66ff33
        0xff, 0xff, 0xff,

        0x00,0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00
    };
    char packet_bytes_set[] = {
        0x5d, 0xb5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00
    };
    int isError;
    //Control transfer arugments are so forth defined:
    uint8_t       bmReqType = 0x21;
    uint8_t            bReq = 0x09;
    uint16_t           wVal = 0x035D;
    uint16_t         wIndex = 0;
    uint16_t           wLen = 17;
    unsigned int     to = 0;

    //Attempt to send three packets

    isError = libusb_control_transfer(handle,bmReqType,bReq,wVal,wIndex,packet_bytes_color,wLen,to);
    printf("Successfully send color data urb.\n");

    //isError = libusb_interrupt_transfer(handle,0x00,packet_bytes_white,17,NULL,1000);
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
    else{
        printf("Number of transferred bytes = %d\n",isError);
        printf("Or error is %s\n",libusb_error_name(isError));
    }
    //Send next packet
    isError = libusb_control_transfer(handle,bmReqType,bReq,wVal,wIndex,packet_bytes_set,wLen,to);

    //isError = libusb_interrupt_transfer(handle,0x00,packet_bytes_white,17,NULL,1000);
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
    else{
        printf("Number of transferred bytes = %d\n",isError);
        printf("Or error is %s\n",libusb_error_name(isError));
    }


}

void handleDevice(libusb_device *device, uint8_t interfaceNumber){



    libusb_device_handle *handle;
    int isError;

    handle = libusb_open_device_with_vid_pid(NULL,rogAuraHID_VID,rogAuraHID_PID);
    /*
    Following code does nto work on windows
    if(libusb_kernel_driver_active(handle,interfaceNumber) == 1){
        printf("\tKernel drivers are attached to this device.\n");
    }
    libusb_detach
    ...
    See documentation


    if(isError != 0){
        printf("ERROR:Could not establish a handle with device.\n");
        printf("Corresponding error code is %d\n",isError);
        printf("Corresponding error in ASCII => %s\n",libusb_error_name(isError));
        //TODO: Detect all errors based on error code and give proper prompts
        return;
    }
    */
    printf("Sucessfull in creating a handle for device...\n");
    //Attempt to dettach kernel driver
    //printf("Deattaching kernel driver to ROG AURA\n");
    //See above
    //libusb_set_auto_detach_kernel_driver(device, 1);
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
        printf("ERROR:Another program or driver has cliamed the interface.\n");
        return;
    }
    else if(isError == LIBUSB_ERROR_NO_DEVICE){
        printf("ERROR:Device has been disconnected.\n");
        return;
    }
    printf("Successfully claimed an interface for I/O operations.\n");
    printf("Attempting to send a control transfer...\n");

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
    struct libusb_device_descriptor deviceInfo;
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
    printf("Sucessfully found all usb devices.\n");
    for(i=0;i<number_of_devices;i++){
        device = all_usb_devices[i];

        if(isROGAura(device)){
            printf("Found ROG AURA HID-compliant vendor-defined device.\n");
            asusHIDdevice = device;
        }


    }


    printf("Device address = %d\n",libusb_get_device_address(asusHIDdevice));
    libusb_get_active_config_descriptor(asusHIDdevice,&configDescriptor);
    libinterface = &configDescriptor->interface[0];
    id = &libinterface->altsetting[0];


    printf("Starting packet manipulation.\n");
    //Deinitialize the library before exit
    printf("Number of interfaces available on this device = %d\n",configDescriptor->bNumInterfaces);
    printf("Interface number is found as %d\n",id->bInterfaceNumber);

    handleDevice(asusHIDdevice,id->bInterfaceNumber);


    printf("Deinitializing libusb list.\n");
    libusb_free_device_list(all_usb_devices,1);
    libusb_exit(NULL);
    return 0;
}
