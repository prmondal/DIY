#include "DHT.h"
#include <ESP8266WiFi.h>

//gpio2
#define DHTPIN 2
#define DHTTYPE DHT11

const char* ssid     = "";
const char* password = "";

const char* host = "54.86.132.254"; //"184.106.153.149";
const int httpPort = 80;
const String postUrl = "/input/KJYKanvxx6UNZpj3yXdJ?private_key=vz08w6DAAXf2wlqMj4D8"; //"/update?key=YIDDYP9NZJ52SZKD";
//https://data.sparkfun.com/streams/KJYKanvxx6UNZpj3yXdJ

DHT dht(DHTPIN, DHTTYPE);

// the setup routine runs once when you press reset:
void setup() {
  // Start our ESP8266 Serial Communication
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// the loop routine runs over and over again forever:
void loop() {
  WiFiClient client;

  //connect to server
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\n");
  
  // This will send the request to the server
  Serial.println(String("GET ") + postUrl + "&humidity=" + h + "&temp=" + t + " HTTP/1.1\r\n" + "Host: " + host + "\n");
  client.print(String("GET ") + postUrl + "&humidity=" + h + "&temp=" + t + " HTTP/1.1\r\n" + "Host: " + host + "\r\n\r\n");
  //client.print(String("GET ") + postUrl + "&field1=" + t + " HTTP/1.1\r\n" + "Host: " + host + "\r\n\r\n");

  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  //Serial.println("GET " + postUrl + "&field2=" + h + " HTTP/1.1\r\n" + "Host: " + host + "\r\n\r\n");
  //client.print(String("GET ") + postUrl + "&field2=" + h + " HTTP/1.1\r\n" + "Host: " + host + "\r\n\r\n");

  delay(60000);
}
