#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include <FastLED.h>


#define LED_PIN     5
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB


String DeviceName = "My LED Strip";

bool Is_Avalable = true ;
int NUM_LEDS  =  215;

byte BRIGHTNESS = 64;
byte AnimationFrame =0 ;

byte AnimatedMode = 0 ; 


int UpdateFrequency = 130;

CRGBPalette16 CurrentPalette= RainbowColors_p;
TBlendType    CurrentBlending = LINEARBLEND;

CRGB leds[215]; 
ESP8266WebServer server(80);


unsigned long previousMillis = 0; 


void SendDeviceInfo() 
{       // not ready yet
   String Json ="{ \"DeviceType\":\"LED_strip_esp8266\", \"DeviceName\":\"My LED Strip\", \"LED_Num\":30, \"IsAvalable\":true, }";
  server.send(200, "text/plain", Json );
 
}


 void handleNotFound()
{
  String currentURI= server.uri() ;
  
  String bufer; 

  
  if (currentURI.indexOf("RGB")!=-1)
  {
  CRGB color ;
  
  bufer =  currentURI.substring(currentURI.indexOf("*r")+2,currentURI.indexOf("r*") );
  color.red= bufer.toInt();
  bufer =  currentURI.substring(currentURI.indexOf("*g")+2,currentURI.indexOf("g*") );
  color.green= bufer.toInt();
  bufer = currentURI.substring(currentURI.indexOf("*b")+2,currentURI.indexOf("b*") );
  color.blue= bufer.toInt();
  //*//
  bufer = currentURI.substring(currentURI.indexOf("*i")+2,currentURI.indexOf("i*") );
  //*//
  BRIGHTNESS  = bufer.toInt();
  
    FastLED.setBrightness(  BRIGHTNESS );
    for (int i =0; i<NUM_LEDS;i++ )
      {
      leds[i]=color;
      }
      FastLED.show();
      AnimatedMode =0;
      server.send(200, "text/plain", "Ready");
       
     return ;
   }
   //*//
  if (currentURI.indexOf("Gradient")!=-1)
  { 
  CRGB color0 ;
  CRGB color1 ;
   //*//
   bufer =  currentURI.substring(currentURI.indexOf("*r")+2,currentURI.indexOf("r*") );
   color0.red= bufer.toInt();
   bufer =  currentURI.substring(currentURI.indexOf("*g")+2,currentURI.indexOf("g*") );
   color0.green= bufer.toInt();
   bufer = currentURI.substring(currentURI.indexOf("*b")+2,currentURI.indexOf("b*") );
   color0.blue= bufer.toInt();
   //*//
    bufer =  currentURI.substring(currentURI.indexOf("*R")+2,currentURI.indexOf("R*") );
   color1.red= bufer.toInt();
   bufer =  currentURI.substring(currentURI.indexOf("*G")+2,currentURI.indexOf("G*") );
   color1.green= bufer.toInt();
   bufer = currentURI.substring(currentURI.indexOf("*B")+2,currentURI.indexOf("B*") );
   color1.blue= bufer.toInt();
   //*//
   bufer = currentURI.substring(currentURI.indexOf("*i")+2,currentURI.indexOf("i*") );
   BRIGHTNESS  = bufer.toInt();
   //*//
   FastLED.setBrightness(  BRIGHTNESS );
   fill_gradient_RGB(leds, NUM_LEDS, color0, color1);
   FastLED.show(); 
   AnimatedMode =0;
   server.send(200, "text/plain", "Ready");
  
     return ;
  }
    if (currentURI.indexOf("Anim")!=-1)
  {
      bufer =  currentURI.substring(currentURI.indexOf("*N")+2,currentURI.indexOf("N*") );
      AnimatedMode = bufer.toInt();
      bufer =  currentURI.substring(currentURI.indexOf("*i")+2,currentURI.indexOf("i*") );
      BRIGHTNESS  = bufer.toInt();
      
      FastLED.setBrightness(  BRIGHTNESS );
      
      server.send(200, "text/plain", "Ready");
      
      return ;
  }

   server.send(200, "text/plain", "Error!");
}
void setup() {
//
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
//
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  WiFi.begin("chameleon_network", "chameleonsnetwork");  //Network
//
  while (WiFi.status() != WL_CONNECTED)  //Wait for connection
  { 
    delay(500);
    Serial.println("Waiting to connect…");
  }
//
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //Print the local IP

  server.onNotFound(handleNotFound);
  server.on("/DeviceInfo", SendDeviceInfo);
  server.begin();                    //Start the server
  Serial.println("Server listening");
}



void GenerateAnimationFrame ( byte ColorIndex, byte ColorSharpness )
{  
    for( int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = ColorFromPalette( CurrentPalette, ColorIndex, BRIGHTNESS, CurrentBlending);
        ColorIndex += ColorSharpness;
    }
     
}
 
void loop()
{
 
  if (AnimatedMode!=0)
  {
    switch (AnimatedMode)
    {
      case 1:{ GenerateAnimationFrame (AnimationFrame,1);}
      
      
    }
     
 
 unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000/UpdateFrequency) 
  {
    
    previousMillis = currentMillis;
    AnimationFrame++;
    FastLED.show(); 
  }
  
      server.handleClient();   
  }
  else { server.handleClient();  }
  
     
      
    
    
  
  
  }
  
  
