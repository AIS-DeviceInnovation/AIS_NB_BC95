#include "AIS_NB_BC95.h"

String apnName = "devkit.nb";

String serverIP = ""; // Your Server IP
String serverPort = ""; // Your Server Port

String udpData = "HelloWorld";

AIS_NB_BC95 AISnb;

const long interval = 20000;  //millisecond
unsigned long previousMillis = 0;

long cnt = 0;
void setup()
{ 
  AISnb.debug = true;
  
  Serial.begin(9600);
 
  AISnb.setupDevice(serverPort);

  String ip1 = AISnb.getDeviceIP();  
  delay(1000);
  
  pingRESP pingR = AISnb.pingIP(serverIP);
  previousMillis = millis();

}
void loop()
{ 
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
    {
      cnt++;     
           
      // Send data in String 
      UDPSend udp = AISnb.sendUDPmsgStr(serverIP, serverPort, udpData+String(cnt));
   
      //Send data in HexString     
      //udpDataHEX = AISnb.str2HexStr(udpData);
      //UDPSend udp = AISnb.sendUDPmsg(serverIP, serverPort, udpDataHEX);
      previousMillis = currentMillis;
  
    }
  UDPReceive resp = AISnb.waitResponse();
     
}



