#ifndef AIS_NB_IoT_h
#define AIS_NB_IoT_h

#define ATLSOFTSERIAL 1

#include <Arduino.h>
#include <Stream.h>

#define MODE_STRING 0
#define MODE_STRING_HEX 1

#if ATLSOFTSERIAL 
	#include "AltSoftSerial.h"
#endif

struct AIS_NB_IoT_RES
{
	unsigned char status;
	String data;
	String temp;
};

struct signal
{
	String csq;
	String rssi;
	String ber;
};

struct UDPSend
{
	bool status;
	String strsend;
	unsigned char socket;
	unsigned int length;
};

struct UDPReceive
{
	unsigned char socket;
	String ip_address;
	unsigned int port;
	unsigned int length;
	String data;
	unsigned int remaining_length;
};
struct pingRESP
{
	bool status;
	String addr;
	String ttl;
	String rtt;
};
class AIS_NB_IoT
{
public:
	AIS_NB_IoT();
	bool debug;

	void (*Event_debug)(char *data);
	void reset();
	void rebootModule();
	bool waitReady();
	bool setPhoneFunction(unsigned char mode);

	//General
	String getFirmwareVersion();
	String getIMEI();
    String getIMSI();
	pingRESP pingIP(String IP);

	//Network
	bool setAutoConnectOn();
	bool setAutoConnectOff();
	String getAutoConnect();
	String getNetworkStatus();
    //bool setAPN(String apn);
	String getAPN();


	bool cgatt(unsigned char mode);
	bool getNBConnect();
	
	void setupDevice(String serverPort);
	bool attachNB(String serverPort);
	bool detachNB();

	String getDeviceIP();
	signal getSignal();

	void createUDPSocket(String port);

    UDPSend sendUDPmsg(String addressI,String port,String data);
	UDPSend sendUDPmsg(String addressI,String port,unsigned int len,char *data,unsigned char send_mode);
	UDPSend sendUDPmsgStr(String addressI,String port,String data);
	
	bool closeUDPSocket();

	UDPReceive waitResponse();

	String toString(String dat);
	String str2HexStr(String strin);
	char char_to_byte(char c);
	
	void receive_UDP(UDPReceive rx);

private:
		AIS_NB_IoT_RES wait_rx_bc(long tout,String str_wait);

protected:
	 Stream *_Serial;	
};

#endif