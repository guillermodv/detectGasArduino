#include <WiFi.h>
#include <HTTPClient.h>

#define MQ7_PIN 2 

const char* wifiNetworks[][2] = {
  {"prueba", "33042804"},
  {"Guillermo", "gallogallo"},
  {"WifiUTN", ""},
  {"Ian", "labestiamatematica"}
};


const int numOfNetworks = sizeof(wifiNetworks) / sizeof(wifiNetworks[0]);
const int deviceID = 001;

String serverName = "http://detectgas.brazilsouth.cloudapp.azure.com:3001/measure";

unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

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

// Función para generar un número aleatorio entre 20 y 500
int generarMedicionAleatoria() {
  return random(10, 225); // El valor máximo es excluido, por eso 501
}

void sendGetRequest() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Generar medición aleatoria
    int medicion = generarMedicionAleatoria();
    // Medicion del sensor
    int value = analogRead(MQ7_PIN);
    
    Serial.println("Valor del sensor :" + String(value));
    String serverPath = serverName + "/" + String(deviceID) + "/" + String(medicion);

    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
    Serial.println("Intentando escribir :" + serverPath);

    if (httpResponseCode > 0) {
      String response = http.getString();  // Obtener la respuesta del servidor
      Serial.println("Respuesta del servidor: " + response);
      ledBlinkInterval = 1000;  // Parpadeo lento cuando la solicitud es exitosa
    } else {
      Serial.print("Código de error: ");
      Serial.println(httpResponseCode);
      Serial.println("Reintentando en 60 segundos...");
      ledBlinkInterval = 80;  // Parpadeo rápido si la solicitud falla
      delay(5000);  
    }
    http.end();
  } else {
    Serial.println("WiFi desconectado");
    connectToWiFi();  // Intentar reconectar si se pierde la conexión WiFi
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(MQ7_PIN, INPUT); 
  connectToWiFi();
  Serial.println("Timer set to 60 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  blinkLED();  
  if ((millis() - lastTime) > timerDelay) {
    sendGetRequest(); 
    lastTime = millis();
  }
}
