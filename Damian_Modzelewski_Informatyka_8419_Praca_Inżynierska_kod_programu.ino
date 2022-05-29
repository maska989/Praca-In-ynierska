#include <TinyGPS++.h>
#include <SoftwareSerial.h> 
#include "Nextion.h"
#include "DHT.h"
SoftwareSerial gpsSerial(8,9); /* 8tx 9rx */
TinyGPSPlus gps;
#define DHTPIN 12
#define DHTTYPE DHT22
#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)0.5))
float lattitude,longitude,temp;
uint32_t satellitesValue,sats,wil; 
int8_t T,T2,T3,H;
double altidute,Speed,Spedo; 
char buffer[10] ={0}; 
static const uint32_t GPSBaud = 9600;
int bat;
DHT dht(DHTPIN, DHTTYPE); 
NexText t1 = NexText(1,4,"long");
NexText kmh = NexText(0,14,"kmh"); 
NexText t2 = NexText(1,5,"lat");
NexText Wys = NexText(1,10,"Wys");
NexText t3 = NexText(0,3,"temp");
NexGauge KM = NexGauge(0,2,"z0");
NexTouch *nex_listen_list[] =  
  { 
  NULL
  }; 


  
  void setup() {
    gpsSerial.begin(GPSBaud);
    Serial.begin(9600);
    delay (5000); 
    nexInit();
    dht.begin();
    pinMode(bat, INPUT);
    }  
    void loop(){    
      smartDelay(500); 
      bat = analogRead(A0);
      int Cell = (bat/1023)*100;
      temp = dht.readTemperature(); 
      wil = dht.readHumidity();  
      lattitude = (gps.location.lat()); 
     longitude = (gps.location.lng());  
     T = (gps.time.hour());  
     T2 = (gps.time.minute());     
     T3 = (gps.time.second());   
     H=(T % 24)+1;    
     sats = (gps.satellites.value());  
     Speed = (gps.speed.kmph());  
     Spedo = Pre(Speed); 
     altidute =(gps.altitude.meters());
     KM.setValue(Spedo); 
     
      memset(buffer, 0 ,sizeof(buffer)); /*wyzerowanie buffora*/   
      itoa(altidute,buffer,10); 
      Wys.setText(buffer); 
      memset(buffer, 0 ,sizeof(buffer)); 
      String Lat = String(lattitude,6); 
      Lat.toCharArray(buffer, 10); 
      t1.setText(buffer);   
      memset(buffer, 0 ,sizeof(buffer)); 
      String Long = String(longitude,6);    
      Long.toCharArray(buffer, 10);  
      t2.setText(buffer);   
      memset(buffer, 0 ,sizeof(buffer)); 
      itoa(temp,buffer,10); 
      t3.setText(buffer);    
      memset(buffer, 0 ,sizeof(buffer)); 
      itoa(Speed,buffer,10);  
      kmh.setText(buffer);
               
      Serial.print("wil.val=");   
      Serial.print(wil);   
      smartSend(); 
      Serial.print("h.val=");   
      Serial.print(H);  
      smartSend();   
      Serial.print("m.val=");  
      Serial.print(T2);  
      smartSend();   
      Serial.print("s.val=");  
      Serial.print(T3);   
      smartSend();
      Serial.print("n0.val=");  
      Serial.print(sats);  
      smartSend();    
      Serial.print("j0.val=");    
      Serial.print(Cell);    
      smartSend(); 
   
} 
 void smartSend()
 {
      Serial.write(0xff); 
      Serial.write(0xff);   
      Serial.write(0xff);
 }

void smartDelay(unsigned long ms)
{ 
  unsigned long start = millis(); 
do{  
  while (gpsSerial.available()) 
  gps.encode(gpsSerial.read()); 
  } while (millis() - start < ms);
  }

  
  double Pre (double i){
    double x; double y=0;   
    if(i<=39){ 
      x=i+320;  
      return x; 
      }
      else if(i==0&&i==40){  
        return 0;  
        } 
        else if(i>40){ 
          return i-40;
          }
  }
