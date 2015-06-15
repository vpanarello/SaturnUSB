/*
Interface de controles de Sega Saturn para USB Sega Saturn to USB controller Interface
Ver. 1.01 - 28/04/2015

(c) Vagner Panarello
*/

#include <TimerOne.h>

// Address for controllers and External multiplexers
// Usually the S2 and S3 belong for internal controllers addressament and S1 and S0 choose 1 of 4 controllers to scan in time.

#define S0 21
#define S1 20
#define S2 19
#define S3 18

// Input pins for controller 1

#define Y0 1
#define Y1 0
#define Y2 2
#define Y3 3

#define scanFrequency 976 //976 // 512 Hz - 128 Hz per controller

#define maxEndereco 15

// ===============================================

uint8_t  endereco = 0;
uint8_t  controle = 0;
uint8_t  multiplex = 0;


uint8_t controllerMapping[4][4];

// ===============================================

void changeMuxSeletor(void) {
  
          endereco++;
          if (endereco > maxEndereco) {endereco = 0;}
          
          multiplex = (endereco & B00001100) >> 2;
          controle = (endereco & B00000011);
          
          PORTF = (endereco << 4) & B11110000;
}

void scanKeys(void) {
  
          uint8_t keysSample = ~PIND & B00001111;
         
          if (multiplex == 0)
              {
                controllerMapping[controle][0] |= (keysSample << 4);
                controllerMapping[controle][0] &= ((keysSample << 4) | B00001111);
              
              }
          else if (multiplex == 1) 
              {
                controllerMapping[controle][0] |= keysSample ;
                controllerMapping[controle][0] &= (keysSample | B11110000) ;
              
              }
          else if (multiplex == 2)
              {  
                boolean esquerda = (keysSample & B00000001);
                boolean baixo = (keysSample & B00000010) >> 1;
                boolean direita = (keysSample & B00000100) >> 2;
                boolean cima = (keysSample & B00001000) >> 3;
                
                if (cima) {
                    controllerMapping[controle][2] = 1;
                }
                else if (baixo) {
                    controllerMapping[controle][2] = - 1;
                }
                else {
                    controllerMapping[controle][2] = 0;
                }
                
                if (direita) {
                    controllerMapping[controle][3] = 1;
                }
                else if (esquerda) {
                    controllerMapping[controle][3] = - 1;
                
                }
                else {
                    controllerMapping[controle][3] = 0;
                }
              }
          else if (multiplex == 3)
              {
                    controllerMapping[controle][1] = (keysSample >> 3) & B00000001;
              }
}
    
    
    
          
void printKeyPad(void) {
      Serial.println("");
      for (byte i = 0; i < 4; i++) {
        Serial.println(controllerMapping[0][i],BIN);
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
   delay(100);
   // printKeyPad();
   Gamepad.sendData(controllerMapping[0]);
}
