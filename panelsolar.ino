#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHT11_PIN 7
#define TX_PIN 3
#define RX_PIN 2
#include <SoftwareSerial.h>
SoftwareSerial espSerial(TX_PIN, RX_PIN);

#include <BH1750.h>
BH1750 lightMeter;

#include <dht11.h>
dht11 DHT;

#include <Servo.h>
Servo lr_servo;
Servo ud_servo;

const byte interruptPin = 2;

int lr_angle = 90;
int ud_angle = 10;
int l_state = A0;
int r_state = A1;
int u_state = A2;
int d_state = A3;
const byte buzzer = 6;
const byte lr_servopin = 9;
const byte ud_servopin = 10;

unsigned int light;
byte error = 15;
byte m_speed = 10;
byte resolution = 1;
int temperature;
int humidity;

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);

  Wire.begin();
  lightMeter.begin();

  lr_servo.attach(lr_servopin);
  ud_servo.attach(ud_servopin);
  pinMode(l_state, INPUT);
  pinMode(r_state, INPUT);
  pinMode(u_state, INPUT);
  pinMode(d_state, INPUT);

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), adjust_resolution, FALLING);

  lcd.init();
  lcd.backlight();

  lr_servo.write(lr_angle);
  delay(1000);
  ud_servo.write(ud_angle);
  delay(1000);
}

void loop() {
  enviarDatosESP8266();
  delay(2000);
  ServoAction();
  read_light();
  read_dht11();
  LcdShowValue();
}

void ServoAction() {
  int L = analogRead(l_state);
  int R = analogRead(r_state);
  int U = analogRead(u_state);
  int D = analogRead(d_state);

  if (abs(L - R) > error && L > R) {
    lr_angle -= resolution;
    if (lr_angle < 0) lr_angle = 0;
    lr_servo.write(lr_angle);
    delay(m_speed);
  }
  else if (abs(L - R) > error && L < R) {
    lr_angle += resolution;
    if (lr_angle > 180) lr_angle = 180;
    lr_servo.write(lr_angle);
    delay(m_speed);
  }
  else {
    lr_servo.write(lr_angle);
  }

  if (abs(U - D) > error && U >= D) {
    ud_angle -= resolution;
    if (ud_angle < 10) ud_angle = 10;
    ud_servo.write(ud_angle);
    delay(m_speed);
  }
  else if (abs(U - D) > error && U < D) {
    ud_angle += resolution;
    if (ud_angle > 90) ud_angle = 90;
    ud_servo.write(ud_angle);
    delay(m_speed);
  }
  else {
    ud_servo.write(ud_angle);
  }
}

void LcdShowValue() {
  char str1[5];
  char str2[2];
  char str3[2];

  dtostrf(light, -5, 0, str1);
  dtostrf(temperature, -2, 0, str2);
  dtostrf(humidity, -2, 0, str3);

  lcd.setCursor(0, 0);
  lcd.print("Light:");
  lcd.setCursor(6, 0);
  lcd.print(str1);
  lcd.setCursor(11, 0);
  lcd.print("lux");

  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.setCursor(2, 1);
  lcd.print("C");
  lcd.setCursor(5, 1);
  lcd.print(humidity);
  lcd.setCursor(7, 1);
  lcd.print("%");

  lcd.setCursor(11, 1);
  lcd.print("res:");
  lcd.setCursor(15, 1);
  lcd.print(resolution);
}

void read_light() {
  light = lightMeter.readLightLevel();
}

void read_dht11() {
  int chk = DHT.read(DHT11_PIN);
  temperature = DHT.temperature;
  humidity = DHT.humidity;
}

void adjust_resolution() {
  tone(buzzer, 800, 100);
  delay(10);
  if (!digitalRead(interruptPin)) {
    if (resolution < 5) {
      resolution++;
    } else {
      resolution = 1;
    }
  }
}

void enviarDatosESP8266() {
  String dataString = String(light) + "|" + String(temperature) + "|" + String(humidity) + "|" +
                      String(lr_angle) + "|" + String(ud_angle) + "|" + String(resolution);

  Serial.println(dataString);
  espSerial.println(dataString);
}
