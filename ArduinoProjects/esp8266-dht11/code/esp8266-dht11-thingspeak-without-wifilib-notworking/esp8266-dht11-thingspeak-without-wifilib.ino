#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

String ssid = "bumba";  // SSID to connect to
String password = "gopal155353"; // Our virtual wifi has no password (so dont do your banking stuff on this network)

const String host = "184.106.153.149";

const int httpPort = 80;
const String postUrl = "/update?key=YIDDYP9NZJ52SZKD&field1=";

DHT dht(DHTPIN, DHTTYPE);

// the setup routine runs once when you press reset:
void setup() {
  // Start our ESP8266 Serial Communication
  Serial.begin(115200);   // Serial connection over USB to computer
  Serial.println("AT");   // Serial connection on Tx / Rx port to ESP8266
  delay(1000);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) Serial.println("Serial connection on Tx / Rx port to ESP8266 failed!");  // check if the ESP is running well
    
  // Connect to Wifi
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(1000);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) Serial.println("Connect to Wifi failed!"); // check if the ESP is running well
  
  // Open TCP connection to the host:
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(2000);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) Serial.println("Open TCP connection to the host failed!"); // check if the ESP is running well
}

// the loop routine runs over and over again forever:
void loop() {
  delay(2000);

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
  Serial.print(" *C ");
  
  // Construct our HTTP call
  String httpPacket = "GET " + postUrl + t + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int length = httpPacket.length();

  // Send our message length
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10); // Wait a little for the ESP to respond
  if (!Serial.find(">")) Serial.println("Send message length failed!"); // check if the ESP is running well

  // Send our http request
  Serial.print(httpPacket);
  delay(10); // Wait a little for the ESP to respond
  if (!Serial.find("SEND OK\r\n")) Serial.println("Send http request failed!"); // check if the ESP is running well
  
  delay(5000);
}
