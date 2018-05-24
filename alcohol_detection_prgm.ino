#include <ESP8266WiFi.h>      //Based on WiFi.h from Arduino WiFi shield library.
#include <PubSubClient.h>     //It is a form of library for wifi shield.

                        // Update these with values suitable for your network.

const char* ssid = "Teni";
const char* password = "sarvani*";
const char* mqtt_server = "iot.eclipse.org";


//Assignig the variables
 WiFiClient espClient;            
 PubSubClient client(espClient);   

  //Assigning datatypes
 char msg[50];                        
 int value=0;
 int mq135 = A0;
 int relay = 4;//D2
 

void setup() {       // put your setup code here, to run once:

  pinMode(mq135,INPUT);      // Initialize the BUILTIN_LED pin as an input
  pinMode(relay, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);    //setting the baurd rate
  setup_wifi();
  client.setServer(mqtt_server, 1883);//Setting the server  and port
  client.setCallback(callback);  //calling the callback function
}



void setup_wifi() {                     
    delay(10);
    
  //connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);               //checking for wifi connection

  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//Declaring the callback function
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("alcohol", "Ready to publish");
      // ... and resubscribe
      client.subscribe("detection");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  client.publish("alcohol", msg);
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
//  for getting the values of sensor repeatedly
  value=analogRead(mq135);
  char al_char[20];
  String al_val = String(value);
  al_val.toCharArray(al_char,10);
  if (client.connect("c")){
    Serial.println("sending message");
    client.publish("alcohol",al_char);  
    delay(5000);
  }

//  if(value>=310){
  
//    snprintf (msg,75,"you can't start the car %d",value); // for publishing the message      
//    Serial.print("Publish message: ");                
//    Serial.println(msg);
//    digitalWrite(relay,HIGH);
//    delay(3000);
//  }
//  else{
//    snprintf (msg, 120, "you can start the car  %d",value);
//    Serial.print("Publish message: ");
//    Serial.println(msg);
//    digitalWrite(relay,LOW);
    
//    delay(3000);
//    }    
}

 
