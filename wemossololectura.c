#define MQ7_PIN A0  // Pin al que está conectado el MQ-7

void setup() {
  Serial.begin(9600);                // Inicia el monitor serial
  pinMode(MQ7_PIN, INPUT);           // Configura el pin del MQ-7 como entrada
  pinMode(LED_BUILTIN, OUTPUT);      // Configura el pin del LED como salida
}

void loop() {
  int sensorValue = analogRead(MQ7_PIN);       // Lee el valor del sensor
  float voltage = sensorValue * (5.5 / 4095.0); // Convierte el valor a voltaje

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
