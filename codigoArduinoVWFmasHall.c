#include <WiFi.h>
#include <HTTPClient.h>

const char* wifiNetworks[][2] = {
  {"prueba", "33042804"},
  {"Guillermo", "gallogallo"},
  {"WIFIUtn", ""},
  {"prueba3", "password3"}
};

const int numOfNetworks = sizeof(wifiNetworks) / sizeof(wifiNetworks[0]);
const int deviceID = 001;

String serverName = "http://192.168.1.44:3001/core";

unsigned long lastTime = 0;
unsigned long timerDelay = 8000;

const int ledPin = 2;  
unsigned long ledBlinkInterval = 0;
unsigned long previousMillis = 0; 

void connectToWiFi() {
  for (int i = 0; i < numOfNetworks; i++) {
    Serial.print("Trying to connect to WiFi network: ");
    Serial.println(wifiNetworks[i][0]);

    WiFi.begin(wifiNetworks[i][0], wifiNetworks[i][1]);

    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < 10) { 
      delay(500);
      Serial.print(".");
      ledBlinkInterval = 600; 
      blinkLED();
      attempt++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("");
      Serial.print("Connected to WiFi network with IP Address: ");
      Serial.println(WiFi.localIP());
      ledBlinkInterval = 1000; 
      return; 
    }

    Serial.println("");
    Serial.println("Failed to connect to WiFi, trying next network...");
  }

  Serial.println("Could not connect to any WiFi network.");
  ledBlinkInterval = 80; 
}

void blinkLED() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= ledBlinkInterval) {
    previousMillis = currentMillis;
    int ledState = digitalRead(ledPin);
    digitalWrite(ledPin, !ledState); 
  }
}

void sendGetRequest() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String serverPath = serverName + "/" + String(deviceID) + "/1";
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();  
      Serial.println("Respuesta del servidor: " + response);
      ledBlinkInterval = 1000;  
    } else {
      Serial.print("Código de error: ");
      Serial.println(httpResponseCode);
      Serial.println("Reintentando en 8 segundos...");
      ledBlinkInterval = 80;  
      delay(5000);  
    }
    http.end();
  } else {
    Serial.println("WiFi desconectado");
    connectToWiFi();  
  }
}

void readHallSensor() {
  int hallValue = hallRead();  // Leer el valor del sensor de efecto Hall
  Serial.print("Valor del sensor de efecto Hall: ");
  Serial.println(hallValue);

  // Puedes tomar alguna acción basada en el valor del sensor
  if (hallValue > 100) {  // Ejemplo de umbral
    Serial.println("Campo magnético fuerte detectado");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  connectToWiFi();
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  blinkLED();
  
  if ((millis() - lastTime) > timerDelay) {
    sendGetRequest();
    readHallSensor();  // Leer el valor del sensor de efecto Hall
    lastTime = millis();
  }
}
