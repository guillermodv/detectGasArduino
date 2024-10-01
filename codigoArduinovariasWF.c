#include <WiFi.h>
#include <HTTPClient.h>

// Lista de SSID y contraseñas
const char* wifiNetworks[][2] = {
  {"prueba", "33042804"},
  {"Guillermo", "gallogallo"},
  {"prueba3", "password3"}
};

const int numOfNetworks = sizeof(wifiNetworks) / sizeof(wifiNetworks[0]);
const int deviceID = 001;

// Tu dirección de servidor
String serverName = "http://localhost:3001/core/1/1";

// Variables de tiempo
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

// Pin del LED
const int ledPin = 2;  // Cambia este pin según tu hardware
unsigned long ledBlinkInterval = 0;
unsigned long previousMillis = 0; // Almacena el último tiempo de parpadeo

void connectToWiFi() {
  for (int i = 0; i < numOfNetworks; i++) {
    Serial.print("Trying to connect to WiFi network: ");
    Serial.println(wifiNetworks[i][0]);

    WiFi.begin(wifiNetworks[i][0], wifiNetworks[i][1]);

    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED && attempt < 10) {  // Intentar 10 veces antes de cambiar de red
      delay(500);
      Serial.print(".");
      ledBlinkInterval = 500; // Parpadeo medio mientras intenta conectar
      blinkLED();
      attempt++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("");
      Serial.print("Connected to WiFi network with IP Address: ");
      Serial.println(WiFi.localIP());
      ledBlinkInterval = 1000; // Parpadeo lento cuando está conectado
      return;  // Si la conexión fue exitosa, salir de la función
    }

    Serial.println("");
    Serial.println("Failed to connect to WiFi, trying next network...");
  }

  Serial.println("Could not connect to any WiFi network.");
  ledBlinkInterval = 100; // Parpadeo rápido si no está conectado
}

void blinkLED() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= ledBlinkInterval) {
    previousMillis = currentMillis;
    int ledState = digitalRead(ledPin);
    digitalWrite(ledPin, !ledState);  // Invertir el estado del LED
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);  // Configurar el pin del LED como salida
  connectToWiFi();
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  blinkLED();  // Llamar la función de parpadeo del LED en cada ciclo

  // Enviar una solicitud HTTP POST cada 5 segundos
  if ((millis() - lastTime) > timerDelay) {
    // Comprobar si la conexión WiFi está activa
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String serverPath = serverName;

      http.begin(serverPath.c_str());

      // Enviar solicitud GET
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }

      // Liberar recursos
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
      connectToWiFi();  // Intentar reconectar si la conexión WiFi se pierde
    }
    lastTime = millis();
  }
}


