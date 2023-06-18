#include <Wire.h> // include the I2C library
#include <LiquidCrystal_I2C.h> // include the I2C LCD library
#include <DHT.h> // include the DHT library

#define DHTPIN 2 // define the DHT11 data pin
#define DHTTYPE DHT11 // define the DHT11 sensor type
#define TRIGGER_PIN 3 // define the HC-SR04 trigger pin
#define ECHO_PIN 4 // define the HC-SR04 echo pin

DHT dht(DHTPIN, DHTTYPE); // initialize the DHT11 sensor
LiquidCrystal_I2C lcd(0x27, 16, 2); // initialize the I2C LCD

const int motorPin = 10;

const int redPin = 5; // define the red LED pin
const int greenPin = 6; // define the green LED pin
const int bluePin = 7; // define the blue LED pin

int fanCount = 0; // sets a counter for fan control

void setup() {
  pinMode(TRIGGER_PIN, OUTPUT); // set the trigger pin as output
  pinMode(ECHO_PIN, INPUT); // set the echo pin as input
  pinMode(redPin, OUTPUT); // set the red LED pin as output
  pinMode(greenPin, OUTPUT); // set the green LED pin as output
  pinMode(bluePin, OUTPUT); // set the blue LED pin as output
  pinMode(motorPin, OUTPUT); // set the motor pin as output

  dht.begin(); // start the DHT11 sensor
  lcd.begin(); // initialize the LCD
  lcd.backlight(); // turn on the LCD backlight

}

void loop() {
  float humidity = dht.readHumidity(); // read the humidity value
  float temperature = dht.readTemperature(); // read the temperature value
  float temperature_f = temperature * 1.8 + 32; // convert the temperature to Fahrenheit 
 
  float distance = getDistance(); // read the distance value

  int speed = 127;
  
  lcd.setCursor(0,0); // set the cursor to the first column of the first row
  lcd.print("Temp: "); // display the temperature label
  lcd.print(temperature_f); // display the temperature value
  lcd.print("F "); // display the temperature unit
  
  
  lcd.setCursor(0,1); // set the cursor to the first column of the second row
  lcd.print("Humidty: "); // display the humidity label
  lcd.print(humidity); // display the humidity value
  lcd.print("% "); // display the humidity unit
  
  /*lcd.print("Dis: "); // display the distance label
  lcd.print(distance); // display the distance value
  lcd.print("cm "); // display the distance unit00
  lcd.print("fanCount "); // display the fan counter label
  lcd.print(fanCount); // display the fan counter value*/
  
  // increase fan counter by one using ultrasonic sensor
  if (distance < 50) {
    fanCount = fanCount + 3;
  }
  
  if (((fanCount % 2) != 0) ||  (temperature_f >= 73)) {
    analogWrite(motorPin, speed);
  } else {
    analogWrite(motorPin, 0);
  }

  // map the temperature to an RGB color
  int r, g, b;
  if (temperature_f < 67) {
    r = 0;
    g = 0;
    b = 255;
  } else if (temperature_f >= 67 && temperature_f <= 73) {
    r = 0;
    g = 255;
    b = 0;
  } else {
    r = 255;
    g = 0;
    b = 0;
    
  }

// set the RGB LED color
analogWrite(redPin, r);
analogWrite(greenPin, g);
analogWrite(bluePin, b);
  
  delay(100); // wait for a tenth of a second
}

float getDistance() {
  // send a 1ms pulse to the trigger pin
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10000);
  digitalWrite(TRIGGER_PIN, LOW);
  
  // calculate the duration of the pulse from the echo pin
  float duration = pulseIn(ECHO_PIN, HIGH);
  
  // calculate the distance in centimeters
  float distance = duration * 0.034 / 2.0;
  
  return distance;
}
 
