#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

Servo servo;
#define LED D1
const char* ssid = ""; // WiFi adınız
const char* password =  ""; //Wifi Şifreniz
const char* mqttServer = ""; // Modem tarafından size verilen ip adresi ile lokalde çalışabilirsiniz.
const int mqttPort = 1883;
unsigned long currentMillis;
int oldMillis = 10;
WiFiClient espClient;
PubSubClient client(espClient);

 
void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  servo.attach(D4); //D4
  servo.write(0);
  
  /* WiFi BAĞLANTISI */
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  

  /* MQTT BAĞLANTISI */
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client")) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("esp/test/receive", "ESP8266: online");
  client.subscribe("esp/test/send");
 
}


 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  String message, command;
  Serial.print("Message:");
  int cntrl = 0;
  for (int i = 0; i < length; i++) {
     if((char) payload[i] == ':')
     {
      cntrl = 1;
      continue;
     }
     if(cntrl == 0)
      command = command + (char) payload[i];  // convert *byte to string
     else if(cntrl == 1)
      message = message + (char) payload[i];
  }

  Serial.println(command);
  Serial.print(message);
  if (command == "led")
  {
    if (message == "on") { 
    	digitalWrite(LED, HIGH); 
    	client.publish("esp/test/receive", "ledOk!"); 
    } if (message == "off") { 
    	digitalWrite(LED, LOW);
    	client.publish("esp/test/receive", "ledOff!"); 
    }
  }
  else if (command == "servo")
  {
    servo.write(message.toInt()); 
    client.publish("esp/test/receive", "ServoOk!");
  }
  Serial.println();
  Serial.println("-----------------------");
 
}


 
void loop() {
  client.loop();
}
