#include <ThingSpeak.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// definisi variabel
long duration;
int distance;

const char * SSID = "Wokwi-GUEST"; 
const char * PASS = "";            
WiFiClient  client;               
//settingan thingspeak 
unsigned long channel = 2006551; //channel ID Pada thingspeak 
const char *apiKey = "BQESBZDOMPGNWQSU"; //API Key pada thingspeak 

#define TRIG_PIN  5  // ESP32 pin 5 connected ke Ultrasonic Sensor
#define ECHO_PIN  18  // ESP32 pin 18 connected ke Ultrasonic Sensor
#define SERVO_PIN 18  // ESP32 pin 18
#define DISTANCE_THRESHOLD  150 // Ketinggian air untuk membuka servo

Servo servo; // control servo

// variables will change:
float duration_us, distance_cm;

void setup() {
  Serial.begin (9600);       // initialize serial port
  pinMode(TRIG_PIN, OUTPUT); // set ESP32 pin to output mode
  pinMode(ECHO_PIN, INPUT);  // set ESP32 pin to input mode
  servo.attach(SERVO_PIN);   // attaches the servo on pin 9 to the servo object
  servo.write(0);

  WiFi.begin(SSID, PASS);                                
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
  delay(500);
  Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}

void loop() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);
  // calculate the distance
  distance_cm = 0.017 * duration_us;

  if (distance_cm < DISTANCE_THRESHOLD)
    servo.write(0); // TUTUP SERVO
  else
    servo.write(90);  // BUKA SERVO 

  // print the value to Serial Monitor
  Serial.print("KETINGGIAN AIR (CM): ");
  Serial.println(distance_cm);
  // Serial.println(" cm");

  // delay(500);

  //baru
  // i++;
 // lum = analogRead(34);
  
  int x = ThingSpeak.writeField(channel, 1, distance_cm, apiKey);
  if (x == 200) {
    Serial.println("Data NEW UPDATE " + String(distance_cm));
    Serial.print("distance: "); Serial.println(distance_cm);
    Serial.println("Update to ThingSpeak Successful.");
  }
  else {
    Serial.println("Data NEW UPDATE " + String(distance_cm));
    // Serial.println("Problem updating data. HTTP error code " + String(x));
  }
  
  Serial.println("Waiting...\n");
  //tahan selama 1 detik, program tidak menjalankan yang lain
  delay(500);
}

