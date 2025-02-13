
#include "IRremote.h"

/*----- Variables, Pins -----*/
#define ENABLE 5
#define DIRA 3
#define DIRB 4
int i;
int receiver = 12; // Signal Pin of IR receiver to Arduino Digital Pin 6

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);    // create instance of 'irrecv'
decode_results results;     // create instance of 'decode_results'

void setup()
{ 
  irrecv.enableIRIn(); // Start the receiver
    //---set pin direction
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
if (irrecv.decode(&results)) // have we received an IR signal?
  {
    Serial.println("IF start");
    switch(results.value)
    {
      Serial.println("SWITCH start");

      case 0xFF629D: // "UP" button pressed
                      Serial.println("UP start");
                      digitalWrite(ENABLE,LOW);
                      delay(250);
                      analogWrite(ENABLE,400); // enable on
                      digitalWrite(DIRA,HIGH); //one way
                      digitalWrite(DIRB,LOW);
                      delay(2000); 
                      digitalWrite(ENABLE,LOW);
                      Serial.println("UP");
                      break;

      case 0xFFA857: // "DOWN" button pressed
                      Serial.println("DOWN start");
                      digitalWrite(ENABLE,LOW);
                      delay(250);
                      analogWrite(ENABLE,400);
                      digitalWrite(DIRA,LOW);  //reverse
                      digitalWrite(DIRB,HIGH); 
                      delay(2000); 
                      digitalWrite(ENABLE,LOW);
                      Serial.println("DOWN end");
                      break;

      case 0xFF02FD: // "OK" button pressed
                      Serial.println("OK start");
                      digitalWrite(ENABLE, LOW); // Turns it off
                      delay(2000); 
                      Serial.println("OK end");
                      break;
                
    }
    
    Serial.println("RESUME start");
    irrecv.resume(); // receive the next value   
    Serial.println("RESUME/IF end");
    Serial.println("");
  }  


}/* --end main loop -- */
