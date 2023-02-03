/*
AIS_NB_BC95 v1.0.5
Author: DEVI/AIS
Create Date: 1 May 2017
Modified: 31 Jan 2023
Released for private use
*/

#include "AIS_NB_BC95.h"

// ################### Buffer #######################
String input;
String buffer;
// ################### counter value ################
byte k = 0;
// ################## flag ##########################
bool end        = false;
bool send_NSOMI = false;
bool flag_rcv   = true;
// ################### Parameter ####################
bool          en_rcv   = false;
unsigned long previous = 0;
unsigned char sendMode = 0;
String        sendStr;

void event_null(char *data) {}

AltSoftSerial myserial;

AIS_NB_BC95::AIS_NB_BC95() {
  Event_debug = event_null;
}

void AIS_NB_BC95::setupDevice(String serverPort) {
  myserial.begin(9600);
  _Serial = &myserial;

  Serial.println(F("############ AIS_NB_BC95 Library by AIS/DEVI V1.0.6 ############"));
  reset();

  bc95        = isBC95();
  String imei = getIMEI();
  if (debug)
    Serial.print(F("# Module IMEI-->  "));
  if (debug)
    Serial.println(imei);
  String fmver = getFirmwareVersion();
  if (debug)
    Serial.print(F("# Firmware ver-->  "));
  if (debug)
    Serial.println(fmver);
  String imsi = getIMSI();
  if (debug)
    Serial.print(F("# IMSI SIM-->  "));
  if (debug)
    Serial.println(imsi);

  attachNB(serverPort);
  createUDPSocket(serverPort);
}

void AIS_NB_BC95::reset() {
  rebootModule();
  while (!setPhoneFunction(1)) {
    Serial.print(F("."));
  }
  Serial.println();
}

void AIS_NB_BC95::rebootModule() {
  // delay(1000);

  _Serial->println(F("AT"));
  AIS_NB_BC95_RES res = wait_rx_bc(500, F("OK"));
  _Serial->println(F("AT+NRB"));
  if (debug)
    Serial.print(F("# Reboot Module"));
  while (!waitReady())
    ;
  {
    if (debug)
      Serial.print(F("."));
  }
  _Serial->flush();
  delay(5000);
}

bool AIS_NB_BC95::waitReady() {
  static bool reset_state = false;
  if (_Serial->available()) {
    String input = _Serial->readStringUntil('\n');
    if (input.indexOf(F("OK")) != -1) {
      return (true);
    }
  }
  return (false);
}

bool AIS_NB_BC95::setPhoneFunction(unsigned char mode) {
  _Serial->print(F("AT+CFUN="));
  _Serial->println(mode);
  AIS_NB_BC95_RES res = wait_rx_bc(1000, F("OK"));
  delay(2000);
  return (res.status);
}

String AIS_NB_BC95::getIMEI() {
  String out;
  _Serial->println(F("AT+CGSN=1"));
  AIS_NB_BC95_RES res = wait_rx_bc(1000, F("OK"));
  out                 = res.temp;
  out.replace(F("OK"), "");
  if (out.length() < 7) {
    return "";
  }
  out = out.substring(7, out.length());
  res = wait_rx_bc(1000, F("OK"));
  return (out);
}

String AIS_NB_BC95::getFirmwareVersion() {
  _Serial->println(F("AT+CGMR"));
  AIS_NB_BC95_RES res = wait_rx_bc(1000, F("OK"));
  String          out = res.temp;
  // if (debug){Serial.println(out);}
  out.replace(F("OK"), "");
  out = out.substring(0, out.length());
  // out = out.substring(10,28);
  res = wait_rx_bc(500, F("OK"));
  return (out);
}
String AIS_NB_BC95::getIMSI() {
  _Serial->println(F("AT+CIMI"));
  AIS_NB_BC95_RES res = wait_rx_bc(1000, F("OK"));
  String          out = res.temp;
  out.replace(F("OK"), "");
  out = out.substring(0, out.length());
  res = wait_rx_bc(500, F("OK"));
  return (out);
}

pingRESP AIS_NB_BC95::pingIP(String IP) {
  pingRESP pingr;
  String   data = "";
  _Serial->println("AT+NPING=" + IP);

  AIS_NB_BC95_RES res = wait_rx_bc(3000, F("+NPING"));

  if (res.status) {
    data = res.data;
    _Serial->println(data);
    int index    = data.indexOf(F(":"));
    int index2   = data.indexOf(F(","));
    int index3   = data.indexOf(F(","), index2 + 1);
    pingr.status = true;
    pingr.addr   = data.substring(index + 1, index2);
    pingr.ttl    = data.substring(index2 + 1, index3);
    pingr.rtt    = data.substring(index3 + 1, data.length());
    // Serial.println("# Ping Success");
    if (debug)
      Serial.println("# Ping IP:" + pingr.addr + ",ttl= " + pingr.ttl + ",rtt= " + pingr.rtt);

  } else {
    if (debug)
      Serial.println("# Ping Failed");
  }
  res = wait_rx_bc(500, F("OK"));
  return pingr;
}

String AIS_NB_BC95::getDeviceIP() {
  String data = "";
  _Serial->println(F("AT+CGPADDR"));
  AIS_NB_BC95_RES res = wait_rx_bc(3000, F("+CGPADDR"));
  if (res.status) {
    data       = res.data;
    int index  = data.indexOf(F(":"));
    int index2 = data.indexOf(F(","));
    data       = res.data.substring(index2 + 1, data.length());
    if (debug)
      Serial.println("# Device IP: " + data);

  } else {
    data = "";
  }
  res = wait_rx_bc(500, F("OK"));
  return data;
}

bool AIS_NB_BC95::setAutoConnectOn() {
  _Serial->println(F("AT+NCONFIG=AUTOCONNECT,TRUE"));
  AIS_NB_BC95_RES res = wait_rx_bc(1000, F("OK"));
  return (res.status);
}

bool AIS_NB_BC95::setAutoConnectOff() {
  _Serial->println(F("AT+NCONFIG=AUTOCONNECT,FALSE"));
  AIS_NB_BC95_RES res = wait_rx_bc(1000, F("OK"));
  return (res.status);
}

String AIS_NB_BC95::getNetworkStatus() {
  String out  = "";
  String data = "";

  _Serial->println(F("AT+CEREG=2"));
  AIS_NB_BC95_RES res = wait_rx_bc(500, F("OK"));
  _Serial->println(F("AT+CEREG?"));
  res = wait_rx_bc(2000, F("+CEREG"));
  if (res.status) {
    data       = res.data;
    int index  = data.indexOf(F(":"));
    int index2 = data.indexOf(F(","));
    int index3 = data.indexOf(F(","), index2 + 1);
    out        = data.substring(index2 + 1, index2 + 2);
    if (out == F("1")) {
      out = F("Registered");
    } else if (out == "0") {
      out = F("Not Registered");
    } else if (out == "2") {
      out = F("Trying");
    }
    if (debug)
      Serial.println("# Get Network Status : " + out);
  }
  res = wait_rx_bc(1000, F("OK"));
  _Serial->flush();
  return (out);
}
/*
bool AIS_NB_BC95:: setAPN(String apn)
{
    String cmd = "AT+CGDCONT=1,\"IP\",";
    cmd += "\""+apn+"\"";
    _Serial->println(cmd);
    AIS_NB_BC95_RES res = wait_rx_bc(1000,F("OK"));
    return(res.status);
}*/

String AIS_NB_BC95::getAPN() {
  String data = "";
  String out  = "";
  _Serial->println(F("AT+CGDCONT?"));
  AIS_NB_BC95_RES res = wait_rx_bc(2000, F("+CGDCONT"));
  if (res.status) {
    int index  = 0;
    int index2 = 0;
    data       = res.data;
    index      = data.indexOf(F(":"));
    index2     = data.indexOf(F(","));

    index  = res.data.indexOf(F(","), index2 + 1);
    index2 = res.data.indexOf(F(","), index + 1);
    out    = data.substring(index + 2, index2 - 1);
    Serial.println("# Get APN: " + out);
  }
  res = wait_rx_bc(500, F("OK"));
  return (out);
}

bool AIS_NB_BC95::attachNB(String serverPort) {
  bool ret = false;
  if (!getNBConnect()) {
    if (debug)
      Serial.print(F("# Connecting NB-IoT Network"));
    for (int i = 1; i < 60; i += 1) {
      setPhoneFunction(1);
      setAutoConnectOn();
      cgatt(1);
      delay(3000);
      if (getNBConnect()) {
        ret = true;
        break;
      }
      Serial.print(F("."));
    }
  } else {
    return true;
  }

  if (ret) {
    if (debug)
      Serial.print(F("> Connected"));
    createUDPSocket(serverPort);
  } else {
    if (debug)
      Serial.print(F("> Disconnected"));
  }
  if (debug)
    Serial.println(F("\n################################################################"));
  return ret;
}
bool AIS_NB_BC95::detachNB() {
  bool ret = false;
  _Serial->flush();
  if (debug)
    Serial.print(F("# Disconnecting NB-IoT Network"));
  cgatt(0);
  delay(1000);
  for (int i = 1; i < 60; i += 1) {
    Serial.print(F("."));
    if (!getNBConnect()) {
      ret = true;
      break;
    }
  }
  if (debug)
    Serial.println(F("> Disconnected"));
  return ret;
}

bool AIS_NB_BC95::cgatt(unsigned char mode) {
  _Serial->print("AT+CGATT=");
  _Serial->println(mode);
  AIS_NB_BC95_RES res = wait_rx_bc(5000, F("OK"));
  return (res.status);
}

bool AIS_NB_BC95::getNBConnect() {
  _Serial->println(F("AT+CGATT?"));
  AIS_NB_BC95_RES res = wait_rx_bc(500, F("+CGATT"));
  bool            ret;
  if (res.status) {
    if (res.data.indexOf(F("+CGATT:0")) != -1)
      ret = false;
    if (res.data.indexOf(F("+CGATT:1")) != -1)
      ret = true;
  }
  res = wait_rx_bc(500, F("OK"));
  return (ret);
}

signal AIS_NB_BC95::getSignal() {
  _Serial->println(F("AT+CSQ"));
  AIS_NB_BC95_RES res = wait_rx_bc(500, F("+CSQ"));
  signal          sig;
  int             x = 0;
  String          tmp;
  if (res.status) {
    if (res.data.indexOf(F("+CSQ")) != -1) {
      int index  = res.data.indexOf(F(":"));
      int index2 = res.data.indexOf(F(","));
      tmp        = res.data.substring(index + 1, index2);
      if (tmp == F("99")) {
        sig.csq  = F("N/A");
        sig.rssi = F("N/A");
      } else {
        sig.csq  = tmp;
        x        = tmp.toInt();
        x        = (2 * x) - 113;
        sig.rssi = String(x);
      }
      sig.ber = res.data.substring(index2 + 1);
      if (debug)
        Serial.println("# Get CSQ Signal: csq= " + sig.csq + ", rssi= " + sig.rssi +
                       ", ber= " + sig.ber);
    }
  }
  res = wait_rx_bc(500, F("OK"));
  return (sig);
}

void AIS_NB_BC95::createUDPSocket(String port) {
  if (bc95) {
    _Serial->print(F("AT+NSOCR=DGRAM,17,"));
    _Serial->println(port + ",1");
  } else {
    _Serial->println(F("AT+NSOCR=DGRAM,17,5684,1"));
  }

  AIS_NB_BC95_RES res = wait_rx_bc(3000, F("OK"));
  // Serial.println(res.temp);
  UDPsocket = res.temp.substring(0, 2);
  UDPsocket.trim();
  delay(3000);
  res = wait_rx_bc(500, F("OK"));
}

UDPSend AIS_NB_BC95::sendUDPmsg(String addressI, String port, unsigned int len, char *data,
                                unsigned char send_mode) {
  sendMode = send_mode;

  UDPSend ret;
  if (!attachNB(port)) {
    if (debug)
      Serial.println("# >Disconnected");
    return ret;
  }
  if (debug)
    Serial.println(F("\n################################################################"));
  if (debug)
    Serial.print(F("# Sending Data IP="));
  if (debug)
    Serial.print(addressI);
  if (debug)
    Serial.print(F(" PORT="));
  if (debug)
    Serial.print(port);
  if (debug)
    Serial.println();

  _Serial->print(F("AT+NSOST="));
  _Serial->print(UDPsocket);
  // if (debug) Serial.print(F("AT+NSOST=0"));
  _Serial->print(F(","));
  // if (debug) Serial.print(",");
  _Serial->print(addressI);
  // if (debug)Serial.print(addressI);
  _Serial->print(F(","));
  // if (debug)Serial.print(",");
  _Serial->print(port);
  // if (debug) Serial.print(port);
  _Serial->print(F(","));
  // if (debug)Serial.print(",");

  if (send_mode == MODE_STRING_HEX) {
    _Serial->print(String(len / 2));
    // if (debug) Serial.print(String(len/2));
  } else {
    _Serial->print(String(len));
    // if (debug) Serial.print(String(len));
  }

  _Serial->print(F(","));

  if (debug)
    Serial.print(F("# Data="));
  if (send_mode == MODE_STRING_HEX) {
    for (int i = 0; i < len; i++) {
      _Serial->print(data[i]);
      if (debug)
        Serial.print(data[i]);
    }
  }
  if (send_mode == MODE_STRING) {
    printHEX(data);
  }

  _Serial->println();
  if (debug)
    Serial.println();

  AIS_NB_BC95_RES res = wait_rx_bc(5000, F("OK"));
  ret.status          = false;
  ret.socket          = 0;
  ret.length          = 0;
  if (res.status) {
    ret.status = true;
    int index  = res.temp.indexOf(F(","));
    int index2 = res.temp.indexOf(F("O"));
    ret.socket = res.temp.substring(index - 1, index).toInt();
    ret.length = res.temp.substring(index + 1, index2).toInt();
    if (debug)
      Serial.println("# Send OK");
  } else {
    if (debug)
      Serial.println("# Send ERROR");
  }

  // Serial.println(F("\n###############################################"));

  return (ret);
}

UDPReceive AIS_NB_BC95::waitResponse() {
  unsigned long current = millis();
  UDPReceive    rx_ret;

  if (en_rcv && (current - previous >= 250) && !(_Serial->available())) {
    _Serial->print(F("AT+NSORF="));
    _Serial->println(UDPsocket);
    _Serial->println(F(",100"));
    // Serial.println(F("AT+NSORF"));
    previous = current;
  }

  if (_Serial->available()) {
    char data = (char)_Serial->read();
    if (data == '\n' || data == '\r') {
      if (k > 2) {
        end = true;
        k   = 0;
      }
      k++;
    } else {
      input += data;
    }
    // if(debug) Serial.println(input);
  }
  if (end) {
    // Serial.println(input);
    if (input.indexOf(F("+NSONMI:")) != -1) {
      // if (debug)
      //   Serial.print(F("send_NSOMI: "));
      // if(debug) Serial.println(input);
      if (input.indexOf(F("+NSONMI:")) != -1) {
        // if (debug)
        //   Serial.print(F("found NSONMI "));
        _Serial->print(F("AT+NSORF="));
        _Serial->print(UDPsocket);
        _Serial->println(F(",100"));
        input      = F("");
        send_NSOMI = true;
      }
      end = false;
    } else {
      // if(debug) Serial.print(F("get buffer: "));
      // if(debug) Serial.println(input);

      end = false;

      int index1 = input.indexOf(F(","));
      if (index1 != -1) {
        int index2 = input.indexOf(F(","), index1 + 1);

        int index3 = input.indexOf(F(","), index2 + 1);
        int index4 = input.indexOf(F(","), index3 + 1);
        int index5 = input.indexOf(F(","), index4 + 1);
        int index6 = input.indexOf(F("\r"));

        rx_ret.socket           = input.substring(0, index1).toInt();
        rx_ret.ip_address       = input.substring(index1 + 1, index2);
        rx_ret.port             = input.substring(index2 + 1, index3).toInt();
        rx_ret.length           = input.substring(index3 + 1, index4).toInt();
        rx_ret.data             = input.substring(index4 + 1, index5);
        rx_ret.remaining_length = input.substring(index5 + 1, index6).toInt();

        if (debug)
          receive_UDP(rx_ret);
      }

      send_NSOMI = false;
      input      = F("");
    }
  }
  return rx_ret;
}    // end waitResponse

UDPSend AIS_NB_BC95::sendUDPmsg(String addressI, String port, String data) {
  int  x_len = data.length();
  char buf[x_len + 2];
  data.toCharArray(buf, x_len + 1);
  return (sendUDPmsg(addressI, port, x_len, buf, MODE_STRING_HEX));
}
UDPSend AIS_NB_BC95::sendUDPmsgStr(String addressI, String port, String data) {
  sendStr    = data;
  int  x_len = data.length();
  char buf[x_len + 2];
  data.toCharArray(buf, x_len + 1);
  return (sendUDPmsg(addressI, port, x_len, buf, MODE_STRING));
}

bool AIS_NB_BC95::closeUDPSocket() {
  _Serial->println(F("AT+NSOCL=0"));
  AIS_NB_BC95_RES res = wait_rx_bc(1000, F("OK"));
  return (res.status);
}
AIS_NB_BC95_RES AIS_NB_BC95::wait_rx_bc(long tout, String str_wait) {
  unsigned long   pv_ok      = millis();
  unsigned long   current_ok = millis();
  String          input;
  unsigned char   flag_out = 1;
  unsigned char   res      = -1;
  AIS_NB_BC95_RES res_;
  res_.temp = "";
  res_.data = "";

  while (flag_out) {
    if (_Serial->available()) {
      input = _Serial->readStringUntil('\n');

      res_.temp += input;
      if (input.indexOf(str_wait) != -1) {
        res      = 1;
        flag_out = 0;
      } else if (input.indexOf(F("ERROR")) != -1) {
        res      = 0;
        flag_out = 0;
      }
    }
    current_ok = millis();
    if (current_ok - pv_ok >= tout) {
      flag_out = 0;
      res      = 0;
      pv_ok    = current_ok;
    }
  }

  res_.status = res;
  res_.data   = input;
  return (res_);
}

// Util Function
void AIS_NB_BC95::printHEX(char *str) {
  char *hstr;
  hstr        = str;
  char out[3] = "";
  int  i      = 0;
  bool flag   = false;
  while (*hstr) {
    flag = itoa((int)*hstr, out, 16);

    if (flag) {
      _Serial->print(out);

      if (debug) {
        Serial.print(out);
      }
    }
    hstr++;
  }
}
String AIS_NB_BC95::toString(String dat) {
  String str = "";
  for (int x = 0; x < dat.length(); x += 2) {
    char c = char_to_byte(dat[x]) << 4 | char_to_byte(dat[x + 1]);
    str += c;
  }
  return (str);
}
String AIS_NB_BC95::str2HexStr(String strin) {
  int    lenuse = strin.length();
  char   charBuf[lenuse * 2 - 1];
  char   strBuf[lenuse * 2 - 1];
  String strout = "";
  strin.toCharArray(charBuf, lenuse * 2);
  for (int i = 0; i < lenuse; i++) {
    sprintf(strBuf, "%02X", charBuf[i]);

    if (String(strBuf) != F("00")) {
      strout += strBuf;
    }
  }

  return strout;
}

char AIS_NB_BC95::char_to_byte(char c) {
  if ((c >= '0') && (c <= '9')) {
    return (c - 0x30);
  }
  if ((c >= 'A') && (c <= 'F')) {
    return (c - 55);
  }
}

void AIS_NB_BC95::receive_UDP(UDPReceive rx) {
  String dataStr;
  Serial.println(F("################################################################"));
  Serial.println(F("# Incoming Data"));
  Serial.println("# IP--> " + rx.ip_address);
  Serial.println("# Port--> " + String(rx.port));
  Serial.println("# Length--> " + String(rx.length));
  if (sendMode == MODE_STRING_HEX) {
    Serial.println("# Data--> " + rx.data);
  } else {
    dataStr = toString(rx.data);
    Serial.println("# Data--> " + dataStr);
  }
  Serial.println("# Remaining length--> " + String(rx.remaining_length));
  Serial.println(F("################################################################"));
}

bool AIS_NB_BC95::isBC95() {
  _Serial->println(F("AT+CGMM"));
  AIS_NB_BC95_RES res = wait_rx_bc(500, F("OK"));

  if (res.temp.indexOf(F("BC95GJB-02-STD")) != -1)
    return false;
  else if (res.temp.indexOf(F("BC95HB-02-STD_900")) != -1)
    return true;
}