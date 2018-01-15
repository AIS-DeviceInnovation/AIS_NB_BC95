# AIS NB-IoT Library
AIS NB-IoT Library is designed for AIS NB-IoT Shield.

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
## AIS NB-IoT Shield (Quectel BC95)
AIS NB-IoT Shield is an extension board for Arduino UNO R3 compatible pinout. 

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

# Getting Started
  1. Connect AIS NB-IoT Shield on Arduino Board
  2. Download AIS NB-IoT Library from Github
  3. Coding & Compile using Arduino Desktop IDE
		- Install the Arduino board drivers
		- Import AIS NB-IoT Library
		- Open AIS NB-IoT Example
		- Compile & Upload
