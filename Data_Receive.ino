#include <Arduino.h>
#include <HTTPClient.h>
#include "ThingSpeak.h"
#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

char ssid[] = "SJ";   // your network SSID (name) 
char pass[] = "        ";   // your network password
WiFiClient  client;

unsigned long weatherStationChannelNumber = 971751;
unsigned int PM2_5 = 1, PM_10 = 2, CO = 3, CH_4 = 4,CO_2 = 5, Humid = 6, temper = 7;

int pm2,pm10,cm,methane,co2,humidity,temp;
int StatusCode = 0;

void setup() {  
  
  delay(500);  
  Serial.print("bauds=115200");  

  Serial.write(0xff);  
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.end();

  Serial.begin(115200);
 
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  
}

void loop() {  
sensrdt();
getpm2();
getpm10();
getco();
getch4();
getco2();
gethumid();
gettemp();

  Serial.print("Home.tmp.val=" + String(temp));
  SE();
  
  Serial.print("Home.h.val=" + String(humidity));
  SE();

  Serial.print("DHT.tmp1.val=" + String(temp));
  SE();
  
  Serial.print("DHT.h1.val=" + String(humidity)); 
  SE();

  Serial.print("PM2.pm2v.val=" + String(pm2));  
  SE();

  if(pm2<=30)
{
  Serial.print("PM2.pm2b.txt=");
  Serial.print("/");
  Serial.print("Good");
  Serial.print("/");
  
  Serial.print("PM2.pm2b.bco=1024");
  SE();
}else if(pm2<=60)
{
  Serial.print("PM2.pm2b.txt=");
  Serial.print("/");
  Serial.print("Satisfactory");
  Serial.print("/");
  
  Serial.print("PM2pm2b.bco=2016");
  SE();
}else if(pm2<=90)
{
  Serial.print("PM2.pm2b.txt=");
  Serial.print("/");
  Serial.print("Moderate");
  Serial.print("/");
  
  Serial.print("PM2.pm2b.bco=65488");
  SE();
  
}else if(pm2<=120)
{
  Serial.print("PM2.pm2b.txt=");
  Serial.print("/");
  Serial.print("Poor");
  Serial.print("/");
  
  Serial.print("PM2.pm2b.bco=64512");
  SE();
  
}else if(pm2<=250)
{
  Serial.print("PM2.pm2b.txt=");
  Serial.print("/");
  Serial.print("Very Poor");
  Serial.print("/");
  
  Serial.print("PM2.pm2b.bco=63488");
  SE();
  
}else if(pm2>250)
{
  Serial.print("PM2.pm2b.txt=");
  Serial.print("/");
  Serial.print("Severe");
  Serial.print("/");
  
  Serial.print("PM2.pm2b.bco=33288");
  SE();
  
}


  Serial.print("CH4.ch4v.val=" + String(methane));  

  SE();
  
  if(methane<=600)
{
  Serial.print("CH4.ch4b.txt=");
  Serial.print("/");
  Serial.print("Good");
  Serial.print("/");
  
  Serial.print("CH4.ch4b.bco=2016");
  SE();
  
}else if(methane<=800)
{
  Serial.print("CH4.ch4b.txt=");
  Serial.print("/");
  Serial.print("Okay");
  Serial.print("/");
  
  Serial.print("CH4.ch4b.bco=65488");
  SE();
}else if(methane<=1000)
{
  Serial.print("CH4.ch4b.txt=");
  Serial.print("/");
  Serial.print("Acceptable");
  Serial.print("/");
  
  Serial.print("CH4.ch4b.bco=64512");
  SE();
  
}else if(methane>1000)
{
  Serial.print("CH4.ch4b.txt=");
  Serial.print("/");
  Serial.print("Dangerous");
  Serial.print("/");
  
  Serial.print("CH4.ch4b.bco=63488");
  SE();
  
}

  Serial.print("PM10.pm10v.val=" + String(pm10));  

  SE();
  
  if(pm10<=50)
{
  Serial.print("PM10.pm10b.txt=");
  Serial.print("/");
  Serial.print("Good");
  Serial.print("/");
  
  Serial.print("PM10.pm10b.bco=1024");
  SE();
}else if(pm10<=100)
{
  Serial.print("PM10.pm10b.txt=");
  Serial.print("/");
  Serial.print("Satisfactory");
  Serial.print("/");
  
  Serial.print("PM10.pm10b.bco=2016");
  SE();
}else if(pm10<=250)
{
  Serial.print("PM10.pm10b.txt=");
  Serial.print("/");
  Serial.print("Moderate");
  Serial.print("/");
  
  Serial.print("PM10.pm10b.bco=65488");
  SE();
  
}else if(pm10<=350)
{
  Serial.print("PM10.pm10b.txt=");
  Serial.print("/");
  Serial.print("Poor");
  Serial.print("/");
  
  Serial.print("PM10.pm10b.bco=64512");
  SE();
  
}else if(pm10<=430)
{
  Serial.print("PM10.pm10b.txt=");
  Serial.print("/");
  Serial.print("Very Poor");
  Serial.print("/");
  
  Serial.print("pm10b.bco=63488");
  SE();
  
}else if(pm10>430)
{
  Serial.print("PM10.pm10b.txt=");
  Serial.print("/");
  Serial.print("Severe");
  Serial.print("/");
  
  Serial.print("pm10b.bco=33288");
  SE();
  
}

  Serial.print("CO.cov.val=" + String(cm));  

  SE();

  if(cm<=3)
{
  Serial.print("CO.cob.txt=");
  Serial.print("/");
  Serial.print("Good");
  Serial.print("/");
  
  Serial.print("CO.cob.bco=2016");
  SE();
}else if(cm<=5)
{
  Serial.print("CO.cob.txt=");
  Serial.print("/");
  Serial.print("Okay");
  Serial.print("/");
  
  Serial.print("CO.cob.bco=65488");
  SE();
}else if(cm<=6)
{
  Serial.print("CO.cob.txt=");
  Serial.print("/");
  Serial.print("Acceptable");
  Serial.print("/");
  
  Serial.print("CO.cob.bco=64512");
  SE();
  
}else if(cm>6)
{
  Serial.print("CO.cob.txt=");
  Serial.print("/");
  Serial.print("Dangerous");
  Serial.print("/");
  
  Serial.print("CO.cob.bco=63488");
  SE();
  
}

  Serial.print("CO2.co2v.val=" + String(co2));  

  SE();

  if(co2<=1000)
{
  Serial.print("CO2.co2b.txt=");
  Serial.print("/");
  Serial.print("Good");
  Serial.print("/");
  
  Serial.print("CO2.co2b.bco=2016");
  SE();
}else if(co2<=2000)
{
  Serial.print("CO2.co2b.txt=");
  Serial.print("/");
  Serial.print("Acceptable");
  Serial.print("/");
  
  Serial.print("CO2.co2b.bco=64512");
  SE();
  
}else if(co2>2000)
{
  Serial.print("CO2.co2b.txt=");
  Serial.print("/");
  Serial.print("Dangerous");
  Serial.print("/");
  
  Serial.print("CO2.c02b.bco=63488");
  SE();
  
}
}

void SE()
{
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
}

void sensrdt() {

  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected");
  }
}

void getpm2(){
  
  pm2 = ThingSpeak.readFloatField(weatherStationChannelNumber, PM2_5);  

  StatusCode = ThingSpeak.getLastReadStatus();
  if(StatusCode == 200){
    Serial.println("");
    Serial.println(" PM 2.5 : " + String(pm2));
  }
  else{
    Serial.println("Problem reading channel. HTTP error code " + String(StatusCode)); 
  }
}

void getpm10(){
  
  pm10 = ThingSpeak.readFloatField(weatherStationChannelNumber, PM_10);  

  StatusCode = ThingSpeak.getLastReadStatus();
  if(StatusCode == 200){
    Serial.println(" PM 10 : " + String(pm10));
  }
  else{
    Serial.println("Problem reading channel. HTTP error code " + String(StatusCode)); 
  } 
}

void getco(){

  cm = ThingSpeak.readFloatField(weatherStationChannelNumber, CO);  

  StatusCode = ThingSpeak.getLastReadStatus();
  if(StatusCode == 200){
    Serial.println(" Carbon Monoxide : " + String(cm));
  }
  else{
    Serial.println("Problem reading channel. HTTP error code " + String(StatusCode)); 
  }
}

void getch4(){

  methane = ThingSpeak.readFloatField(weatherStationChannelNumber, CH_4);  

  StatusCode = ThingSpeak.getLastReadStatus();
  if(StatusCode == 200){
    Serial.println(" Methane : " + String(methane));
  }
  else{
    Serial.println("Problem reading channel. HTTP error code " + String(StatusCode)); 
  }
}

void getco2(){

  co2 = ThingSpeak.readFloatField(weatherStationChannelNumber, CO_2);  

  StatusCode = ThingSpeak.getLastReadStatus();
  if(StatusCode == 200){
    Serial.println(" Carbon Dioxide : " + String(co2));
  }
  else{
    Serial.println("Problem reading channel. HTTP error code " + String(StatusCode)); 
  }
}

void gethumid(){

  humidity = ThingSpeak.readFloatField(weatherStationChannelNumber, Humid);  

  StatusCode = ThingSpeak.getLastReadStatus();
  if(StatusCode == 200){
    Serial.println(" Humidity : " + String(humidity));
  }
  else{
    Serial.println("Problem reading channel. HTTP error code " + String(StatusCode)); 
  }
}
  
void gettemp(){

  temp = ThingSpeak.readFloatField(weatherStationChannelNumber, temper);  

  StatusCode = ThingSpeak.getLastReadStatus();
  if(StatusCode == 200){
    Serial.println(" Temperature : " + String(temp) + " °C");
  }
  else{
    Serial.println("Problem reading channel. HTTP error code " + String(StatusCode)); 
  }
}
