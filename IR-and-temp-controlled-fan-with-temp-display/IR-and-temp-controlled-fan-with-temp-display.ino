#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RECEIVER_PIN 12   // Signal Pin of IR receiver to Arduino Digital Pin 12
#define OUTPUT_PIN 13     // Output Pin connected to your breadboard

#define ENABLE_PIN 5
#define DIRA_PIN 3
#define DIRB_PIN 4

#define DHTPIN A0 // Analog pin for temperature sensor

IRrecv irrecv(RECEIVER_PIN);
decode_results results;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display
bool hot = HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(OUTPUT_PIN, OUTPUT);
  irrecv.enableIRIn(); // Enable the IR receiver
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(DIRA_PIN, OUTPUT);
  pinMode(DIRB_PIN, OUTPUT);
  lcd.init();                      // Initialize the LCD
  lcd.backlight();
}

void loop() {
  handleIRTask();     // IR receiver task
  displayTempTask();  // Temperature sensor task
  runMotorTask();     // DC motor task
}

void handleIRTask() {
  if (irrecv.decode(&results)) {  // If an IR signal is received
    bool outputState = !digitalRead(OUTPUT_PIN); // Toggle the output state
    digitalWrite(OUTPUT_PIN, outputState); // Apply the new output state to the output pin
    Serial.print("IR Signal Received. Output state changed to ");
    Serial.println(outputState == HIGH ? "HIGH" : "LOW");
    irrecv.resume(); // Receive the next value
  }
}


void displayTempTask() {
  int tempReading = analogRead(DHTPIN);
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celsius
  float tempF = (tempC * 9.0) / 5.0 + 32.0; // Convert Celsius to Fahrenheit

  lcd.setCursor(0, 0);
  lcd.print("Temp  F:");
  lcd.setCursor(8, 0);
  lcd.print(tempF);

  delay(500); // Adjust as needed
}


void runMotorTask() {
  int tempReading = analogRead(DHTPIN);
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celsius
  float tempF = (tempC * 9.0) / 5.0 + 32.0; // Convert Celsius to Fahrenheit

  int iteration = 0;
  float test_temp = 70;
  if (tempF > test_temp) {
    while (hot == HIGH) {
      iteration += 1;
      if (iteration < 2) {
        Serial.print("Output state changed to HIGH, Temp over ");
        Serial.println(test_temp);
      }
      digitalWrite(OUTPUT_PIN, HIGH);

        analogWrite(ENABLE_PIN, 250); // Enable the motor
        digitalWrite(DIRA_PIN, HIGH); // Set motor direction or any other motor control logic
        digitalWrite(DIRB_PIN, LOW);
      
      
      if (irrecv.decode(&results)) {  // If an IR signal is received
        digitalWrite(OUTPUT_PIN, LOW); // Apply the new output state to the output pin
        hot = LOW;
        analogWrite(ENABLE_PIN, 0);
        Serial.print("IR Signal Received. Output state changed to LOW");
        irrecv.resume(); // Receive the next value
      }

      displayTempTask();  // Temperature sensor task
    
      int tempReading = analogRead(DHTPIN);
      double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
      tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
      float tempC = tempK - 273.15;            // Convert Kelvin to Celsius
      float tempF = (tempC * 9.0) / 5.0 + 32.0; // Convert Celsius to Fahrenheit

      if (tempF < (test_temp - 0.5)) {
        Serial.print("Output state changed to LOW, Temp under ");
        Serial.println(test_temp);
        hot = LOW;
        analogWrite(ENABLE_PIN, 0);
        digitalWrite(OUTPUT_PIN, LOW);
      }
    }
    
  }
  bool irState = digitalRead(OUTPUT_PIN); // Read the state of the IR receiver

  if (irState == HIGH) {  // Check if the IR receiver state is high
    analogWrite(ENABLE_PIN, 250); // Enable the motor
    digitalWrite(DIRA_PIN, HIGH); // Set motor direction or any other motor control logic
    digitalWrite(DIRB_PIN, LOW);
  } 
    if (irState == LOW) {  // Check if the IR receiver state is low
    analogWrite(ENABLE_PIN, 0); // Disable the motor
    digitalWrite(DIRA_PIN, HIGH); // Set motor direction or any other motor control logic
    digitalWrite(DIRB_PIN, LOW);
    }
}
