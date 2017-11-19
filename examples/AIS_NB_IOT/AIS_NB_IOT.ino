#include "AIS_NB_IoT.h"

String serverIP = "<server IP>";
String serverPort = "<port IP>";

String udpData = "helloWorld";
String udpDataHEX = "4003BD16B6746573744E420568756D6964FF33382E3737";

AIS_NB_IoT AISnb;
AIS_NB_IoT_RES resp;

const long interval = 20000;  //millisecond
unsigned long previousMillis = 0;

long cnt = 0;
void setup()
{ 
  AISnb.debug = true;
  
  delay(2000);
  Serial.begin(9600);
 
  AISnb.setupDevice(serverPort);

  String ip1 = AISnb.getDeviceIP();    
  pingRESP pingR = AISnb.pingIP(serverIP); 

  previousMillis = millis();

}
void loop()
{
  
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval)
  {
      UDPSend udp; 
      cnt++;     
      // Send data in String 
      udp = AISnb.sendUDPmsgStr(serverIP, serverPort, udpData+String(cnt));
    
      //udpDataHEX = AISnb.str2HexStr(udpData);
   
      // Send data in HexString 
      //udp = AISnb.sendUDPmsg( serverIP, serverPort, udpDataHEX);
      previousMillis = currentMillis;
  
  }

 UDPReceive resp = AISnb.waitResponse();
 
}



