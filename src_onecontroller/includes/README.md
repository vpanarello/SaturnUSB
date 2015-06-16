The follow piece of codes have do be included inside "USBAPI.h" and "HID.cpp" files. 
==


### File "HID.cpp"


1) Add this header to the file (approximately in the line 30)
		
		// Gamepad 1
		
2) Add the descriptor of the report of the Joystick device inside the vector "const u8 _hidReportDescriptor[]" (approximately in the line 47)
		
		0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
		0x09, 0x05,                    // USAGE (Game Pad)
		0xa1, 0x01,                    // COLLECTION (Application)
		0xa1, 0x00,                    //   COLLECTION (Physical)
		0x85, 0x03,                    //     REPORT_ID (3)
		0x05, 0x09,                    //     USAGE_PAGE (Button)
		0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
		0x19, 0x09,                    //     USAGE_MINIMUM (Button 9)
		0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
		0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
		0x95, 0x09,                    //     REPORT_COUNT (9)
		0x75, 0x01,                    //     REPORT_SIZE (1)
		0x81, 0x02,                    //     INPUT (Data,Var,Abs)
		0x95, 0x01,                    //     REPORT_COUNT (1)
		0x75, 0x07,                    //     REPORT_SIZE (7)
		0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
		0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
		0x09, 0x30,                    //     USAGE (X)
		0x09, 0x31,                    //     USAGE (Y)
		0x15, 0xff,                    //     LOGICAL_MINIMUM (-1)
		0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
		0x95, 0x02,                    //     REPORT_COUNT (2)
		0x75, 0x08,                    //     REPORT_SIZE (8)
		0x81, 0x02,                    //     INPUT (Data,Var,Abs)
		0xc0,                          //   END_COLLECTION
		0xc0,                         // END_COLLECTION    
	
	
	
	
	
### File "USBAPI.h"