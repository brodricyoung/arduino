#include <IRremote.h>

int RECV_PIN = 11; // Define the pin the IR receiver is connected to
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600); // Initialize serial communication
  irrecv.enableIRIn(); // Start the IR receiver
}

void loop() {
  if (irrecv.decode(&results)) { // If IR code is received
    Serial.println(results.value, HEX); // Print the received IR code in hexadecimal format
    delay(500);
    irrecv.resume(); // Receive the next value
  }
}
