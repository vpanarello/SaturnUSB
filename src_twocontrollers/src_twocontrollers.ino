/* Copyright (c) 2015, Vagner Panarello
*
*  Classic VideoGame Controller interface to USB
*
*  Modified 28th April 2015 by Vagner Panarello for Suport ONE Sega Saturn Controller
*  Modified 30th April 2015 by Vagner Panarello for Suport TWO Sega Saturn Controllers
*
*
*  This program is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/license>
*/

#include <TimerOne.h> // Arduino Timer 1 control Library is required

// Address for controllers and External multiplexers
// Usually the S2 and S3 belong for internal controllers addressament and S1 and S0 choose 1 of 4 controllers to scan in time.

#define S0 21
#define S1 20
#define S2 19
#define S3 18

// Input pins for controller 1

#define Y0_1 1
#define Y1_1 0
#define Y2_1 2
#define Y3_1 3

// Input pins for controller 2

#define Y0_2 8
#define Y1_2 14
#define Y2_2 16
#define Y3_2 15

#define scanFrequency 976 //976 // 512 Hz - 128 Hz per controller
#define maxEndereco 15

// ===============================================

uint8_t  endereco = 0;
uint8_t  controle = 0;
uint8_t  multiplex = 0;

uint8_t keysSample[4];
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
  
         
          keysSample[0] = ~PIND & B00001111;         // Controller 1 scan
          keysSample[1] = (~PINB & B00011111) >> 1;  // Controller 2 scan
          keysSample[2] = 0;                         // Controller 3 NOT allocaded
          keysSample[3] = 0;                         // Controller 3 NOT allocaded
         
          if (multiplex == 0)
              {
                controllerMapping[controle][0] |= (keysSample[controle] << 4);
                controllerMapping[controle][0] &= ((keysSample[controle] << 4) | B00001111);
              
              }
          else if (multiplex == 1) 
              {
                controllerMapping[controle][0] |= keysSample[controle] ;
                controllerMapping[controle][0] &= (keysSample[controle] | B11110000) ;
              
              }
          else if (multiplex == 2)
              {  
                boolean esquerda = (keysSample[controle] & B00000001);
                boolean baixo = (keysSample[controle] & B00000010) >> 1;
                boolean direita = (keysSample[controle] & B00000100) >> 2;
                boolean cima = (keysSample[controle] & B00001000) >> 3;
                
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
                    controllerMapping[controle][1] = (keysSample[controle] >> 3) & B00000001;
              }
}
    
    
    
          
void printKeyPad(void) {
      Serial.println("Controle 1");
      for (byte i = 0; i < 4; i++) {
              Serial.println(controllerMapping[0][i],BIN);
            }
      Serial.println("");
      
      Serial.println("Controle 2");
      for (byte i = 0; i < 4; i++) {
              Serial.println(controllerMapping[1][i],BIN);
            }
      Serial.println("");
}


void pulse(void) {        
        scanKeys();
        changeMuxSeletor();       
}




// ================ Main Routines ==================


void setup() {
  
  // Config pins for Addresses outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Config pins for controller 1
  pinMode(Y0_1, INPUT_PULLUP);
  pinMode(Y1_1, INPUT_PULLUP);
  pinMode(Y2_1, INPUT_PULLUP);
  pinMode(Y3_1, INPUT_PULLUP);
  digitalWrite(Y0_1, HIGH);
  digitalWrite(Y1_1, HIGH);
  digitalWrite(Y2_1, HIGH);
  digitalWrite(Y3_1, HIGH);
  
  // Config pins for controller 2
  pinMode(Y0_2, INPUT_PULLUP);
  pinMode(Y1_2, INPUT_PULLUP);
  pinMode(Y2_2, INPUT_PULLUP);
  pinMode(Y3_2, INPUT_PULLUP);
  digitalWrite(Y0_2, HIGH);
  digitalWrite(Y1_2, HIGH);
  digitalWrite(Y2_2, HIGH);
  digitalWrite(Y3_2, HIGH);
  
  
  Timer1.initialize(scanFrequency);
  Timer1.attachInterrupt(pulse); // call timing controller scanner routines
 
  Serial.begin(9600);
  Gamepad.begin();
}






void loop() {
   delay(50);
    printKeyPad();
    Gamepad.sendData(controllerMapping[0], 3);
    delay(50);
    Gamepad.sendData(controllerMapping[1], 4);
}
