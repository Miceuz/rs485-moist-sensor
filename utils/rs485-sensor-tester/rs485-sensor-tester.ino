#include <Bounce2.h>
#include <ModbusMaster.h>
#include <SoftwareSerial.h>
#include <SdFat.h>

SoftwareSerial softSerial(2, 3); // RX, TX
Bounce button = Bounce();
ModbusMaster sensor;
SdFat sd;


#define DRIVER_ENABLE 7
#define SENSOR_PWR 8
#define LED1 6
#define LED2 A1

const int chipSelect = 4;

void preTransmission() {
  digitalWrite(DRIVER_ENABLE, HIGH);
}

void postTransmission() {
  digitalWrite(DRIVER_ENABLE, LOW);
}

uint16_t data[6];

void setup() {
  Serial.begin(9600);
  softSerial.begin(19200);
  
  pinMode(A0, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(DRIVER_ENABLE, OUTPUT);
  pinMode(SENSOR_PWR, OUTPUT);
  digitalWrite(SENSOR_PWR, HIGH);
  
  button.attach(A0);
  button.interval(5);

  digitalWrite(LED2, HIGH);
  digitalWrite(LED1, HIGH);
  delay(1000);
  digitalWrite(LED2, LOW);
  digitalWrite(LED1, LOW);

  sensor.begin(1, softSerial);
  sensor.preTransmission(preTransmission);
  sensor.postTransmission(postTransmission);

  Serial.begin(9600);
  Serial.println("#Begins...");

  if(sd.begin (chipSelect, SPI_HALF_SPEED)) {
    Serial.println("SD card ok");
  }
}

void loop() {
  if(button.fell()) {
    
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      delay(100);
      uint8_t tests = 0;
      uint16_t moisture=0;
      int16_t temperature=0;
      uint16_t ver=0;
      uint8_t err = 0;
      uint8_t r;
      
      while(tests < 10 && err == 0) {
        r = sensor.readInputRegisters(0, 3);
  
        if(0 != r) {
          Serial.print("#Error: ");
          Serial.println(r);
          
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, HIGH);
          delay(100);
          err = 1;
        } else {
          moisture = sensor.getResponseBuffer(0);
          temperature = sensor.getResponseBuffer(1);
          ver = sensor.getResponseBuffer(2);
          
          tests++;
          if(moisture < 200 || moisture > 450) {
//            Serial.println(moisture);
            Serial.println("#MOISTURE OUT OF BOUNDS!");
            err = 1;
          }
  
          if(temperature < 150 || temperature > 800) {
//            Serial.println(temperature);
            Serial.println("#TEMPERATURE OUT OF BOUNDS!");
            err = 1;
          }
  
          if(err) {
            digitalWrite(LED1, LOW);
            digitalWrite(LED2, HIGH);
            delay(200);
          } else {
            digitalWrite(LED1, HIGH);
            digitalWrite(LED2, LOW);
            delay(50);
          }
        }
      }

      if(0 == r) {
        Serial.print(moisture);
        Serial.print(", ");
        Serial.println(temperature);
        File results = sd.open("results.txt", FILE_WRITE);
        results.print(moisture);
        results.print(",");
        results.println(temperature);
        results.close();
      }      
      while(LOW == button.read()){
        button.update();
      }
      delay(100);
      digitalWrite(LED2, LOW);
      digitalWrite(LED1, LOW);

  } else if(button.rose()){
    digitalWrite(LED2, LOW);
    digitalWrite(LED1, LOW);
  }
  
  button.update();

  
}
