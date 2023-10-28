#include <LiquidCrystal.h>
#include <Servo.h>
#include <dht.h>

#define DHT22_PIN 2
#define LED_PIN 5
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
dht DHT;
Servo myservo;  // create servo object to control a servo
int ppros = 56;
int aangl = 70;
int ttemp = 8;
int potpin = 2;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

struct {
  uint32_t total;
  uint32_t ok;
  uint32_t crc_error;
  uint32_t time_out;
  uint32_t connect;
  uint32_t ack_l;
  uint32_t ack_h;
  uint32_t unknown;
} stat = { 0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  lcd.begin(16, 2);
  myservo.attach(6);
  pinMode(LED_PIN, OUTPUT);
  myservo.write(0);
  Serial.begin(115200); 
}

void loop() {
  if (analogRead(potpin) > ppros){
    myservo.write(aangl);
  }
  if (DHT.temperature < ttemp){
    digitalWrite(LED_PIN, HIGH);  
    delay(100);
    digitalWrite(LED_PIN, LOW);
  }
  val = analogRead(potpin);
  lcd.println(val);
  delay(2000);
  lcd.println(DHT.temperature);
  uint32_t start = micros();
  int chk = DHT.read22(DHT22_PIN);
  uint32_t stop = micros();

  stat.total++;
  switch (chk)
  {
    case DHTLIB_OK:
      stat.ok++;
      Serial.print("OK,\t");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      stat.crc_error++;
      Serial.print("Checksum error,\t");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      stat.time_out++;
      Serial.print("Time out error,\t");
      break;
    case DHTLIB_ERROR_CONNECT:
      stat.connect++;
      Serial.print("Connect error,\t");
      break;
    case DHTLIB_ERROR_ACK_L:
      stat.ack_l++;
      Serial.print("Ack Low error,\t");
      break;
    case DHTLIB_ERROR_ACK_H:
      stat.ack_h++;
      Serial.print("Ack High error,\t");
      break;
    default:
      stat.unknown++;
      Serial.print("Unknown error,\t");
      break;
  }
  Serial.print(DHT.temperature, 1);
  Serial.print(",\t");
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.print(val);
  Serial.print(",\t");
  Serial.println();

}
