# AIS NB-IoT Library
AIS NB-IoT Library is designed for DEVIO NB-Shield I to connect with your own IoT Platform/ Server. 
The library is using Software Serial for communication with Arduino board.

## Function List:

### General Function:
	debug(<Boolean>)
		○ True : Print Log on Serial Monitor
		○ False: No Log on Serial Monitor
	setupDevice(<serverPort>) 
	getDeviceIP()
		○ Return: <String IP>
	str2HexStr(< String data>)
		○ Return: <String dataHexString>
	pingIP(serverIP)
		○ Return type: pingRESP
		○ Value: addr, ttl, rtt

### Send Data:
	sendUDPmsgStr(<serverIP>,< serverPort>,<udpData>)
		○ Maximum udpData : 512 Bytes
	sendUDPmsg (<serverIP>,< serverPort>,<udpDataHEX>)
		○ Maximum udpDataHEX : 512 Bytes

### Receive Data: 
	waitResponse()
		○ Return type: UDPReceive
		○ Value: ip_address, port, length, data, remaining_length

## Device Compatibility:
### DEVIO NB-SHIELD I (Quectel BC95)
DEVIO NB-SHIELD I is an extension board for Arduino UNO R3 compatible pinout. 
[![P1261997-2-tran.png](https://s9.postimg.org/b7t14hapb/P1261997-2-tran.png)](https://postimg.org/image/a5iulxrvv/)
### Specifications:
	Communication Module: Quectel BC95
	Network Technology: LTE Cat. NB1 (NB-IoT)
	Frequency: Band 8 (900MHz)
	Data transmission: (Single tone)
		○ Downlink 24 kbps
		○ Uplink 15.625 kbps
	Protocol stack
		○ UDP
		○ CoAP
	eSIM on-board
	Serial Communication (UART)
		Hardware Serial 
		○ RX : PIN0
		○ TX : PIN1
		Software Serial
		○ RX : PIN8
		○ TX : PIN9


# Getting Started
  1. Connect 'DEVIO NB-SHIELD I' on Arduino Board
  2. Coding & Compile using Arduino Desktop IDE
		- Install the Arduino board drivers
		- Import AIS NB-IoT Library from Library Manager
		- Open AIS NB-IoT Example and specify your server IP & Port
		- Compile & Upload
