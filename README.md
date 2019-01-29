How do I change my backlight colour?
Download asus_rog.o from the obj folder and execute it with the command
	sudo ./asus_rog color_format color_code 

ASUS ROG GL553VE Backlight reverse engineering:

Observations:

	1)In windows you will find the device as a vendor defined device with the product id 1854. There will be multiple such devices of which only one corresponds
	to the backlight control. You can find this by disabling each device progressively and trying to use the ROG Aura softare to control the light. When the light 
	cannot be changed you have found your device. 
	
	2)The procedure to change the backlight colour is as follows:
	
		You need to send a control signal with the following data buffer:
		  
		char packet_bytes_color[] = {
			0x5d, 0xb3, 0x00, 0x00,
			//Color goes next
			//default color #ffffff -> white
			0xff, 0xff, 0xff,
			0x00,0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00
		};
	
		Now you need to send another control signal as with the following data buffer:
		
		 char packet_bytes_set[] = {
			0x5d, 0xb5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00
		};
		
	3)Thats about it.

Driver attempt:
	A libusb based program to control the LEDs can be found on my github.
