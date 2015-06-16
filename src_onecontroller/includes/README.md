Instruction to update "USBAPI.h" and "HID.cpp" files. 
==

The follow steps are necessary to implement new descriptors for the USB Gamepad device, Arduino IDE has no native descriptors or libraries for Leonardo that recognises as Joystick USB device unlike of Keyboards or Mouse devices that you have not to implement extra codes to use them.

You have do include the follow pieces of code like specified in the steps. Pay attention to maintain the original codes of file just adding the new pieces of code at the file. Is strongly recommended make copies from original files before modifying. If some compilation problem occurred after editing the files, you can easily restore de original ones and solve the problem.

Search the two files inside the Arduino IDE installation folder at this path:
"Arduino\hardware\arduino\avr\cores\arduino"

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
		
		
		
		
3) Last add the class "Gamepad", this class has the methods that send the reports (packet of bytes) of controllers data. We will use only the  "sendData()" method on this class. (approximately in the line 579)
		
		
		// Gamepad

		Gamepad_::Gamepad_(void) : _buttons(0)
				{
				}

		void Gamepad_::begin(void)
				{
				}

		void Gamepad_::end(void)
				{
				}
		void Gamepad_::sendData(uint8_t b[4], uint8_t reportID)
				{
					
				 HID_SendReport(reportID, b, 4);
				}
		void Gamepad_::press(uint8_t b)
				{
				 _buttons |= b;
				 HID_SendReport(3,&_buttons,1);
				}

		void Gamepad_::release(uint8_t b)
				{
				 _buttons &= ~b;
				 HID_SendReport(3,&_buttons,1);
				}

		bool Gamepad_::isPressed(uint8_t b)
				{
				 if ((b & _buttons) > 0) 
					return true;
				 return false;
		}
	
	
	
### File "USBAPI.h"


		class Gamepad_
				{
				private:
					uint8_t _buttons;
				public:
					 Gamepad_(void);
					 void begin(void);
					 void end(void);
					 void sendData(uint8_t b[4], uint8_t reportID);
					 void press(uint8_t b);
					 void release(uint8_t b);
					 bool isPressed(uint8_t b);
				};
		extern Gamepad_ Gamepad;