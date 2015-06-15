/*
Interface de controles de Sega Saturn para USB Sega Saturn to USB controller Interface
Ver. 1.01 - 28/04/2015

(c) Vagner Panarello
*/

#include <TimerOne.h>

#define S0 21
#define S1 20
#define S2 19
#define S3 18

#define Y0 1
#define Y1 0
#define Y2 2
#define Y3 3

#define scanFrequency 976 // 512 Hz - 128 Hz per controller

#define maxEndereco 15

// ===============================================

uint8_t  endereco = 0;

boolean keyPad[4][16] = {
  {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},  //  Z   B   cima  0
  {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},  //  X   A   esqu  1
  {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},  //  Y   C   baix  0
  {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}   //  R   St  dir   L
};

// ===============================================

void changeMuxSeletor(void) {
  
          endereco++;
          if (endereco > maxEndereco) {endereco = 0;}
          PORTF = (endereco << 4) & B11110000;
}

void scanKeys(void) {
  
          uint8_t keysSample = PIND & B00001111;
              for (byte i = 0; i < 4; i++) {
                 keyPad[i][endereco] = (keysSample >> i) & B00000001;
              }
}
          
void printKeyPad(void) {
  
      Serial.println("");

      for (byte i = 0; i < 4; i++) {
            Serial.println("");
            for (byte j = 0; j < 16; j++) {
                 Serial.print(keyPad[i][j]);      
                 if (j == 3 || j == 7 || j == 11) {
                     Serial.print(" - ");
               
                 }
            
            }
      }
      Serial.println("");
}           

void pulse(void) {        
        scanKeys();
        changeMuxSeletor();       
}




// ================ Main Routines ==================



void setup() {
  
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  pinMode(Y0, INPUT_PULLUP);
  pinMode(Y1, INPUT_PULLUP);
  pinMode(Y2, INPUT_PULLUP);
  pinMode(Y3, INPUT_PULLUP);
  digitalWrite(Y0, HIGH);
  digitalWrite(Y1, HIGH);
  digitalWrite(Y2, HIGH);
  digitalWrite(Y3, HIGH);
  
  Timer1.initialize(scanFrequency);
  Timer1.attachInterrupt(pulse); // call timing controller scanner routines
  
  Serial.begin(9600);
  
  Gamepad.begin();
 
}

void loop() {
  

  delay(2000);



  
  delay(100);
  printKeyPad();
  

 

}
