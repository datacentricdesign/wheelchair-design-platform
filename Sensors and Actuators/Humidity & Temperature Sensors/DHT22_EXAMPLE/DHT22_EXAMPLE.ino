#include "DHT.h"          // Library for DHT Sensors

#define DHT_PIN 10        // Digital data pin for the sensor

#define DHT_TYPE DHT22    // Defining our sensor

// This initializes our DHT sensor, takes as input the data pin
// and the DHT type (11 or 22). This returns a DHT object variable
DHT dht(DHT_PIN, DHT_TYPE);

float humidity, temperature, temperature_f;
float heat_idx_f, heat_idx_c; // Variables to compute heat index, 
// in fahrenheit and celsius

void setup() {
  
  Serial.begin(9600);     // Starting Serial with 9600 baud.
  Serial.println("Starting DHT22 Example"); 

  dht.begin();            // Start Sensor data collection
}

void loop() {
  // Wait 2 seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds
  // Sensor readings may also be up to 2 seconds 'old' ( slow sensor)
  float humidity = dht.readHumidity();   // Reading humidity 
  
  // Read temperature as Celsius (the default),
  float temperature = dht.readTemperature();

  // Send True as a parameter to read in Fahrenheit
  float temperature_f = dht.readTemperature(true); 

  // Check if any reads failed and exit early (to try again).
  // Whenever a read fails, the function will return NaN (Not a Number)
  
  if (isnan( humidity ) || isnan( temperature ) || isnan( temperature_f )) { 
    Serial.println("Failed to read data from DHT 22!");
    return;               // this will end the loop, but it will be called right away  again!
  }

  // Compute heat indexes in Fahrenheit (the default)
  float heat_idx_f = dht.computeHeatIndex(temperature_f, humidity);
  // Compute heat index in Celsius (send false as a third parameter)
  float heat_idx_c = dht.computeHeatIndex(temperature, humidity, false);


  // Logging of our data 
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %  Temperature: ");
  Serial.print(temperature);
  Serial.print("°C (");
  Serial.print(temperature_f);
  Serial.print("°F)  Heat index: ");
  Serial.print(heat_idx_c);
  Serial.print("°C (");
  Serial.print(heat_idx_f);
  Serial.println("°F).");
  
}
