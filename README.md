# AIS NB-IoT Library
AIS NB-IoT Library is designed for `DEVIO NB-SHIELD I` to connect with your own IoT Platform/ Server. 
The library is using Software Serial for communication with Arduino board.

For MEGA, Please connect GPIO8 to GPIO48 and GPIO09 to GPIO46

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
### DEVIO NB-SHIELD I
`DEVIO NB-SHIELD I` is an extension board for Arduino UNO R3 compatible pinout. 
For `DEVIO NB-SHIELD I` Pinout, please go to "www.ais.co.th/aiap" in the "IoT Dev Center" menu.
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
	Serial Communication (UART)
		Hardware Serial 
		○ RX : GPIO0
		○ TX : GPIO1
		Software Serial
		○ RX : GPIO8
		○ TX : GPIO9
	Dimensions: 
		o 54*74*29 mm (Width x Length x Height)(Excluded antenna)
	Additional Feature:
		o NB-IoT Module Reset Button
		o Power Status LED
	NB-IoT Module Certification
		○ NBTC, CE, CCC, SRRC, NAL, GCF


# Getting Started
  1. Connect `DEVIO NB-SHIELD I` on Arduino Board
  2. Coding & Compile using Arduino Desktop IDE
		- Install the Arduino board drivers
		- Import AIS NB-IoT Library from Library Manager
		- Open AIS NB-IoT Example and specify your server IP & Port
		- Compile & Upload

สนใจสั่งซื้อ DEVIO NB-Shield I ได้ที่ AIS online store (https://store.ais.co.th/th/accessories/other/devio-nb-shield-i.html)

สอบถามข้อมูลเพิ่มเติม ติดต่อ AIS IoT Alliance Program (www.ais.co.th/aiap) ที่หน้า Business Enquiry
