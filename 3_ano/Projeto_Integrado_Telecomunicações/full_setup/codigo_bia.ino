#include <DHT.h>
#include <WiFi.h>
#include <ThingSpeak.h>
#include <Arduino.h>


#define DHTTYPE DHT11

uint8_t DHT11_PIN = 4;
uint8_t motionPin = 27;
uint8_t V_PIN = A12;
#define led 19
#define TRIG_PIN 5 //  connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN 18 //  connected to Ultrasonic Sensor's ECHO pin
int status = LOW;
#define rain_sensor A6
#define led2 12
const int BUZZER_PIN = 14;


DHT dht(DHT11_PIN, DHTTYPE);
float temp;
float hum;
float duration_us, distance_cm;
unsigned long start_time;
unsigned long end_time;
unsigned long timer_start ;

//conection
char ssid[] = "AndroidAP7889"; // network SSID (name)
char pass [] = "biba2019";
// char ssid[] = "LApP3-4C"; // your network SSID (name)
// char pass [] = "LAP3LAP3";
WiFiClient client;

unsigned long myChannelNumberDHT = 2049781;
const char * myWriteAPIKey = "ENK3C9W7PZEKZ2JJ";


void setup(){
  Serial.begin(9600);
   // configure the trigger pin to output mode
  pinMode(TRIG_PIN, OUTPUT);
  // configure the echo pin to input mode
  pinMode(ECHO_PIN, INPUT);
  pinMode(DHT11_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(V_PIN,OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  pinMode(led2, OUTPUT);
  digitalWrite(led2, HIGH);
  dht.begin();
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client); 
}

void loop(){
   // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); 
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

   temp = dht.readTemperature();
   hum = dht.readHumidity();
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration_us = pulseIn(ECHO_PIN, HIGH); // save time duration value in "duration variable
  distance_cm= duration_us*0.034/2; //Convert pulse duration into distance  
  
  // print the value to Serial Monitor
  Serial.println("------------SENSOR ULTRASSONICO------------");
  Serial.print("Distancia: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
  if (distance_cm > 3.5) {
    if (timer_start == 0) {
      timer_start = millis();
    } else if ((millis() - timer_start) > 30000) {
      digitalWrite(BUZZER_PIN, HIGH);
      Serial.println("Distancia maior que 3.5cm por 30 segundos");
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    timer_start = 0;
  }
   Serial.println("------------SENSOR TEMPERATURA E HUMIDADE------------");
   Serial.print("Temp: ");
   Serial.println(temp);
   if(temp<=20.30){//liga o led(resistencia de aquecimento)
      digitalWrite(led,HIGH);
   }   
   
   if(temp>20.50){//Desliga o led(resistencia de aquecimento)
      digitalWrite(led,LOW);
   }
   
   if(temp>=21){//liga a ventoinha
     analogWrite(V_PIN,200);
   }

   if(temp<21){//desliga a ventoinha 
     analogWrite(V_PIN,0);
   }

   Serial.print("Humidade: ");
   Serial.println(hum);
   Serial.println("-------------SENSOR MOVIMENTO------------");
  if(status == HIGH){
    Serial.println("MOVIMENTO");
  }
  else{
    Serial.println("NAO HA MOVIMENTO");
  }
  status = digitalRead(motionPin);

  Serial.println("-------------SENSOR CHUVA------------");
  Serial.print("Value: ");
  int valor= analogRead(rain_sensor);
  Serial.println(valor);
  
  if(valor < 3500){
    digitalWrite(led2, HIGH);
  }
  else{
    digitalWrite(led2, LOW);
  }
  
  int x = ThingSpeak.setField( 1, temp);
  int y = ThingSpeak.setField( 2, hum);
  int z = ThingSpeak.setField( 3, status);
  int d = ThingSpeak.setField( 4, distance_cm);
  int c = ThingSpeak.setField( 5, valor);


  ThingSpeak.writeFields(myChannelNumberDHT, myWriteAPIKey);


  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delay(10);
}