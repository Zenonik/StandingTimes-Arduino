#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

WiFiClient wifiClient;

WiFiClientSecure client;

const char* ssid = "";
const char* password = "";

String serverName = "";
String api_key = "";

const int trigPin = 12;
const int echoPin = 14;

#define SOUND_SPEED 0.034

long duration;
float distanceCm;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;

    String serverPath = serverName + "?key=" + api_key + "&value=" + isStanding();

    client.setInsecure(); //the magic line, use with caution
    client.connect(serverPath, 443);

    http.begin(client, serverPath.c_str());

    int httpResponseCode = http.POST(serverPath);

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.print(httpResponseCode);
      String payload = http.getString();
      Serial.print("      HTTP message: ");
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  delay(10000);
}

long getDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED/2;
  return distanceCm;
}

String isStanding() {
  if (getDistance() > 80){
    return "true";
  }
  else {
    return "false";
  }
}