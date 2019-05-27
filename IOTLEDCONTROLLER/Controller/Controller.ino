
#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>

/* Set these to your desired credentials. */
const char *ssid = "192.168.4.1Pass:12345678";
const char *password = "12345678";

String wifissid="";
String wifipass="";
String blynkauth="";
String mqttserv="";
String mqttport="";
String mqttlogin="";
String mqttpass="";
String mqtttopic="";
bool newwifidata = false;
bool clearedmem = false;
String errormsg="";
String mqttlogtop="";
String mqttname="";
int newvalue=0;
int oldvalue=0;
ESP8266WebServer server(80);

void handleRoot() {
  if(server.arg("usagetype") == "blynk" && server.hasArg("SSID")){
    //write NVRAM
    int addr=0;
    EEPROM.write(addr, 1);
    addr++;
    for(int i=addr; i<= addr+32;i++){
      EEPROM.write(i,int(server.arg("SSID")[i-addr]));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      EEPROM.write(i,int(server.arg("WIFIpass")[i-addr]));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      EEPROM.write(i,int(server.arg("BlynkToken")[i-addr]));
    }

    EEPROM.commit();
    delay(40);
    newwifidata = true;
    clearedmem = false;
    return;
  }
  if(server.arg("usagetype") == "mqtt" && server.hasArg("SSID")){
    //write NVRAM
     int addr=0;
    EEPROM.write(addr, 0);
    addr++;
    for(int i=addr; i<= addr+32;i++){
      EEPROM.write(i,int(server.arg("SSID")[i-addr]));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      EEPROM.write(i,int(server.arg("WIFIpass")[i-addr]));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      EEPROM.write(i,int(server.arg("MQTTserverAddress")[i-addr]));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      EEPROM.write(i,int(server.arg("MQTTserverPort")[i-addr]));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      EEPROM.write(i,int(server.arg("MQTTuser")[i-addr]));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      EEPROM.write(i,int(server.arg("MQTTpassword")[i-addr]));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      EEPROM.write(i,int(server.arg("MQTTtopic")[i-addr]));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      EEPROM.write(i,int(server.arg("MQTTlogtop")[i-addr]));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      EEPROM.write(i,int(server.arg("MQTTname")[i-addr]));
    }

    EEPROM.commit();
    delay(40);
    newwifidata = true;
    clearedmem = false;
    return;
  }

  if(!clearedmem){
    for(int i=0; i<300;i++){
      EEPROM.write(i,0);
      delay(40);
    }
    EEPROM.commit();
    clearedmem=true;
  }
  
  String content = "<html><body><form action='/' method='POST'>Choose for using with Blink or MQTT server<br>";
  content += "<input name='usagetype' type='radio' value='blynk'> Blynk<br>";
  content += "<input name='usagetype' type='radio' value='mqtt'> MQTT<br>";
  content += "<br>Blynk<br>";
  content += "UserAuthToken:<input type='text' name='BlynkToken' placeholder='UserAuthToken'><br>";
  content += "<br>MQTT<br>";
  content += "MQTT server:<input type='text' name='MQTTserverAddress' placeholder='server address'><br>";
  content += "MQTT server port:<input type='text' name='MQTTserverPort' placeholder='port'><br>";
  content += "MQTT user:<input type='text' name='MQTTuser' placeholder='username'><br>";
  content += "MQTT password:<input type='text' name='MQTTpassword' placeholder='password'><br>";
  content += "MQTT topic:<input type='text' name='MQTTtopic' placeholder='topic'><br>";
  content += "MQTT logs topic:<input type='text' name='MQTTlogtop' placeholder='logtopic'><br>";
  content += "MQTT device name for logs:<input type='text' name='MQTTname' placeholder='name'><br>";
  content += "<br>WI-FI Settings<br>";
  content += "SSID:<input type='text' name='SSID' placeholder='SSID'><br>";
  content += "Wi-Fi password:<input type='text' name='WIFIpass' placeholder='wifi password'><br>";
  content += "<input type='submit' name='SUBMIT' value='Submit'></form><br>";
  content += errormsg+"<br>";
	server.send(200, "text/html", content);
}

void setup() {
	delay(1000);
	Serial.begin(115200);
  EEPROM.begin(512);
	Serial.println();
  pinMode(14,OUTPUT);
  analogWrite(14,0);
  pinMode(13,INPUT);


}

void loop() {
  getdatafrommem();
  char charwifissid[wifissid.length()];
  char charwifipass[wifipass.length()];
  wifissid.toCharArray(charwifissid,32);
  wifipass.toCharArray(charwifipass,32);
  WiFi.mode(WIFI_STA);//!!!!!!!
  delay(200);
  WiFi.begin(charwifissid, charwifipass);
  int counterwifi = 0;
  Serial.println(wifissid);
  Serial.println(wifipass);
  Serial.println(blynkauth);
  Serial.println(mqttserv);
  Serial.println(mqttport);
  Serial.println(mqttlogin);
  Serial.println(mqttpass);
  Serial.println(mqtttopic);
  Serial.println(mqttlogtop);
  Serial.println(mqttname);
 
  if(digitalRead(13)==1){
    
     while (WiFi.status() != WL_CONNECTED) {
          delay(500);
         Serial.print(".");
      }
      Serial.println("CONNECTED!");
    if(EEPROM.read(0) == 1){
      char charblynkauth[blynkauth.length()];
      blynkauth.toCharArray(charblynkauth,33);
      Serial.println(charblynkauth);
      Blynk.begin(charblynkauth, charwifissid, charwifipass);
      while(true){
        Blynk.run();
      }
    }else{
      WiFiClient wclient; 
      delay(100);
      int por = mqttport.toInt();
      PubSubClient client(wclient, mqttserv, por);
      if (!client.connected()) {
          Serial.println(mqttserv);
          Serial.println(por);
          Serial.println(mqttlogin);
          Serial.println(mqttpass);
          char charlog[mqttlogin.length()];
          mqttlogin.toCharArray(charlog,32);
          char charpass[mqttpass.length()];
          mqttpass.toCharArray(charpass,32);
          Serial.println("Connecting to MQTT server");
          if (client.connect(MQTT::Connect("arduinoClient").set_auth(charlog, charpass))) {
                Serial.println("Connected to MQTT server");
                client.set_callback(callback);
                client.subscribe(mqtttopic); // подписывааемся по топик с данными для светодиода
                Serial.println("SUBSCRIBED");
                while(true){
                  client.loop();
                  if(newvalue!=oldvalue){
                    client.publish(mqttlogtop, mqttname+"#"+String(newvalue));
                    oldvalue = newvalue;
                  }
                }
           
          } else {
            Serial.println("Could not connect to MQTT server");
            errormsg =  "Could not connect to MQTT server";
       }
      }
    }

    
  }
  else{
  delay(100);
  newwifidata = false;
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
  while(true){
	  server.handleClient();
    if(newwifidata){
      break;
    }
  }
  
  }
}


void getdatafrommem(){
 wifissid="";
 wifipass="";
 blynkauth="";
 mqttserv="";
 mqttport="";
 mqttlogin="";
 mqttpass="";
 mqtttopic="";
 mqttlogtop="";
 mqttname="";
  if(int(EEPROM.read(0)) == 1){
    Serial.println("It IS BLYNK");
    int addr=1;
    for(int i=addr; i<= addr+32;i++){
      wifissid+=char(EEPROM.read(i));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      wifipass+=char(EEPROM.read(i));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      blynkauth+=char(EEPROM.read(i));
    }
  }
  if(int(EEPROM.read(0)) == 0){
    Serial.println("It IS MQTT");
    int addr=1;
    for(int i=addr; i<= addr+32;i++){
      wifissid+=char(EEPROM.read(i));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      wifipass+=char(EEPROM.read(i));
    }
      addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      mqttserv+=char(EEPROM.read(i));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      mqttport+=char(EEPROM.read(i));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      mqttlogin+=char(EEPROM.read(i));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      mqttpass+=char(EEPROM.read(i));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      mqtttopic+=char(EEPROM.read(i));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      mqttlogtop+=char(EEPROM.read(i));
    }
    addr=addr+32+1;
    for(int i=addr; i<= addr+32;i++){
      mqttname+=char(EEPROM.read(i));
    }
 wifissid=wifissid.substring(0,wifissid.length());
 wifipass=wifipass.substring(0,wifipass.length());
 blynkauth=blynkauth.substring(0,blynkauth.length());
 mqttserv=mqttserv.substring(0,mqttserv.length());
 mqttport=mqttport.substring(0,mqttport.length());
 mqttlogin=mqttlogin.substring(0,mqttlogin.length());
 mqttpass=mqttpass.substring(0,mqttpass.length());
 mqtttopic=mqtttopic.substring(0,mqtttopic.length());
 mqttlogtop=mqttlogtop.substring(0,mqttlogtop.length());
 mqttname=mqttname.substring(0,mqttname.length());
    
  }
}


void callback(const MQTT::Publish& pub)
{
String payload = pub.payload_string();
if(payload.substring(0,mqttname.length()) != mqttname){
    int stled = payload.toInt(); // преобразуем полученные данные в тип integer
    stled = min(stled,255);
    stled = max(stled,0);
    analogWrite(14, stled);
    newvalue = stled;
    oldvalue = newvalue-1;
}

}
