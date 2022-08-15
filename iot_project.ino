#define BLYNK_PRINT Serial
#define DHTTYPE DHT11
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define DHTPIN 2


char auth[] = "Lyd3EIOfieXZV2E4QPkx_AijKU33lGBy";
char ssid[] = "conniewongpektin@unifi";
char pass[] = "agp2992agp";


int mL1=D1;
int mL2=D2;
int mR1=D5;
int mR2=D6;
int heat=D0;
bool nLDR;
int nr;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;



void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int r = analogRead(A0);
  bool LDR = digitalRead(D3);
  r = map(r, 0, 1023, 100, 0);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
  Blynk.virtualWrite(V7, r);  //V2 is for Rainfall

if(nLDR!=LDR)
{
if(LDR == 0){
WidgetLED led1(V4);
led1.on();
digitalWrite(heat, LOW);
digitalWrite(mL1,LOW);
digitalWrite(mL2,HIGH);
delay(1000);
digitalWrite(mL1,LOW);
digitalWrite(mL2,LOW);
}
else if(LDR ==1)
{
WidgetLED led1(V4);
led1.off();
digitalWrite(heat, HIGH);
digitalWrite(mL1,HIGH);
digitalWrite(mL2,LOW);
delay(1000);
digitalWrite(mL1,LOW);
digitalWrite(mL2,LOW);
}
nLDR=LDR;
} 


if(r>10 && nr!=1){
digitalWrite(heat, HIGH);
digitalWrite(mL1,HIGH);
digitalWrite(mL2,LOW);
delay(1000);
digitalWrite(mL1,LOW);
digitalWrite(mL2,LOW);
nr=1;
}

else if(r==1 && nr!=2){
digitalWrite(heat, LOW);
digitalWrite(mL1,LOW);
digitalWrite(mL2,HIGH);
delay(1000);
digitalWrite(mL1,LOW);
digitalWrite(mL2,LOW);
nr=2;
}
}

BLYNK_WRITE(V1)
{
 int b = param[0].asInt();
  Serial.println(b);
  if(b==1)
  { digitalWrite(mL1,HIGH);
    digitalWrite(mL2,LOW);
    delay(1200);
    digitalWrite(mL1,LOW);
    digitalWrite(mL2,LOW);
  }
    else
    {
    digitalWrite(mL1,LOW);
    digitalWrite(mL2,LOW);
}
}

BLYNK_WRITE(V2)
{
 int b = param[0].asInt();
  Serial.println(b);
  if(b==1)
  { digitalWrite(mL1,LOW);
  digitalWrite(mL2,HIGH);
  delay(1200);
  digitalWrite(mL1,LOW);
  digitalWrite(mL2,LOW);
  }
    else
    {
    digitalWrite(mL1,LOW);
    digitalWrite(mL2,LOW);
}
}

BLYNK_WRITE(V3)
{
 int b = param[0].asInt();
  Serial.println(b);
  if(b==1)
  { digitalWrite(mR1,LOW);
  digitalWrite(mR2,HIGH);
  }
    else
    {
    digitalWrite(mR1,LOW);
    digitalWrite(mR2,LOW);
}
}

BLYNK_WRITE(V0)
{
 int b = param[0].asInt();
  Serial.println(b);
  if(b==1)
  { digitalWrite(heat, HIGH);
  }
    else
    {
digitalWrite(heat, LOW);
}
}
  
void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(heat, OUTPUT);
  pinMode(mL1,OUTPUT);
  pinMode(mL2,OUTPUT);
  pinMode(mR1,OUTPUT);
  pinMode(mR2,OUTPUT);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(10L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
