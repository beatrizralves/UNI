#include <DHT.h>
#include <WiFi.h>
#include <ThingSpeak.h>
#include <Keypad.h>
#define DHTTYPE DHT11

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

uint8_t DHT11_PIN = 19;
uint8_t motionPin = 33;
//uint8_t V_PIN = A12;

#define led 22
#define TRIG_PIN 12 //  connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN 14 //  connected to Ultrasonic Sensor's ECHO pin
int status = LOW;

DHT dht(DHT11_PIN, DHTTYPE);
float temp;
float hum;
float duration_us, distance_cm;

const char* ssid = "Mi Note 10";   
const char* password = "12345678";

WiFiClient client;

unsigned long myChannelNumberDHT = 2049829;
const char * myWriteAPIKey = "5TY1QWRXYXOGYQO2";

void setup(){
  Serial.begin(9600);
   // configure the trigger pin to output mode
  pinMode(TRIG_PIN, OUTPUT);
  // configure the echo pin to input mode
  pinMode(ECHO_PIN, INPUT);
  pinMode(DHT11_PIN, INPUT);
  //pinMode(V_PIN,OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  dht.begin();
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client); 
}

void loop(){

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, password); 
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  temp = dht.readTemperature();
  hum = dht.readHumidity();
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  // print the value to Serial Monitor
  Serial.println("------------SENSOR ULTRASSONICO------------");
  Serial.print("Distancia: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

   Serial.println("------------SENSOR TEMPERATURA E HUMIDADE------------");
   Serial.print("Temp: ");
   Serial.println(temp);
   if(temp<19){//liga o led(resistencia de aquecimento)
      digitalWrite(led,HIGH);
   }

   if(temp>=19){//Desliga o led(resistencia de aquecimento)
      digitalWrite(led,LOW);
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

  int x = ThingSpeak.setField( 1, temp);
  int y = ThingSpeak.setField( 2, hum);
  ThingSpeak.writeFields(myChannelNumberDHT, myWriteAPIKey);


  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(100);
}