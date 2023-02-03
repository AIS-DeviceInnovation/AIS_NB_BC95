![Library Version](https://img.shields.io/badge/Version-1.1.0-green)

# AIS NB-IoT Library
AIS NB-IoT Library is designed to work with `DEVIO NB-SHIELD I` and `DEVIO NB-SHIELD I Plus` and connect to your own IoT Platform/Server. The library uses Software Serial for communication with the Arduino board.

For MEGA, connect GPIO8 to GPIO48 and GPIO09 to GPIO46.

## Function List

### General Functions
- `debug(<Boolean>)`: Prints log on Serial Monitor if `True`, otherwise no log will be printed.
- `setupDevice(<serverPort>)`: sets up the device.
- `getDeviceIP()`: returns the IP as a string.
- `str2HexStr(< String data>)`: returns the string data as a hex string.
- `pingIP(serverIP)`: returns `pingRESP` with values `addr`, `ttl`, and `rtt`.

### Send Data
- `sendUDPmsgStr(<serverIP>,< serverPort>,<udpData>)`: sends a string message with a maximum size of 512 bytes.
- `sendUDPmsg (<serverIP>,< serverPort>,<udpDataHEX>)`: sends a hex string message with a maximum size of 512 bytes.

### Receive Data
- `waitResponse()`: returns `UDPReceive` with values `ip_address`, `port`, `length`, `data`, and `remaining_length`.

## Device Compatibility
### DEVIO NB-SHIELD I | DEVIO NB-SHIELD I Plus
`DEVIO NB-SHIELD I` and `DEVIO NB-SHIELD I Plus` are extension boards for Arduino UNO R3 with a compatible pinout. For pinout information, please visit "www.ais.co.th/aiap" in the "IoT Dev Center" menu.

### Specifications
- Communication Module: Quectel BC95 / Quectel BC95-G
- Network Technology: LTE Cat. NB1 (NB-IoT)
- Frequency: Band 8 (900MHz)
- Data Transmission: Single tone, downlink 24 kbps, uplink 15.625 kbps
- Protocol Stack: UDP, CoAP
- Serial Communication (UART): Hardware Serial RX on GPIO0 and TX on GPIO1; Software Serial RX on GPIO8 and TX on GPIO9.
- Dimensions: 54 x 74 x 29 mm (width x length x height, excluding antenna)
- Additional Features: NB-IoT Module Reset Button, Power Status LED.
- NB-IoT Module Certification: NBTC, CE, CCC, SRRC, NAL, GCF

# Getting Started
1. Connect `DEVIO NB-SHIELD I` or `DEVIO NB-SHIELD I Plus` to your Arduino board.
2. Code and compile using the Arduino Desktop IDE:
   - Install the Arduino board drivers.
   - Import the AIS NB-IoT Library from the Library Manager.
   - Open the AIS NB-IoT Example and specify your server IP and port.
   - Compile and upload the code.
   


สนใจสั่งซื้อ DEVIO NB-Shield I Plus ได้ที่ [AIS online store](https://store.ais.co.th/th/accessories/other/devio-nb-shield-i.html) หรือ [AIS Playground](https://aisplayground.ais.co.th/marketplace/products/64DkezcxuRk)\
สอบถามข้อมูลเพิ่มเติม ติดต่อ [DEVIO FB Fanpage](https://www.facebook.com/AISDEVIO)
