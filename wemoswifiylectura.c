#include <WiFi.h>

#define MQ7_PIN A0          // Pin al que está conectado el MQ-7
const char* ssid = "xxxx";       // Nombre de la red WiFi
const char* password = "xxx"; // Contraseña de la red WiFi

void setup() {
  Serial.begin(9600);                // Inicia el monitor serial
  pinMode(MQ7_PIN, INPUT);           // Configura el pin del MQ-7 como entrada
  pinMode(LED_BUILTIN, OUTPUT);      // Configura el pin del LED como salida

  // Conexión a la red WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  
  // Espera hasta que se conecte
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado a la red WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());    // Imprime la dirección IP asignada
}

void loop() {
  int sensorValue = analogRead(MQ7_PIN);       // Lee el valor del sensor
  float voltage = sensorValue * (3.3 / 4095.0); // Convierte el valor a voltaje

  // Parpadeo del LED
  digitalWrite(LED_BUILTIN, HIGH);  // Enciende el LED
  delay(500);                       // Espera 500 ms
  digitalWrite(LED_BUILTIN, LOW);   // Apaga el LED
  delay(500);                       // Espera 500 ms
  
  // Imprime los valores del sensor en el monitor serial
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print(" | Voltage: ");
  Serial.println(voltage);

  // Espera adicional antes de la siguiente lectura
  delay(1000); 
}
