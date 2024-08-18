#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include <L3G.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "MAX30105.h"
#include "heartRate.h"

//DHT beign
#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
//DHT end

//GPS beign
const int gpsRxPin = D6; // Connect GPS TX to D2 on NodeMCU
const int gpsTxPin = D7; // Connect GPS RX to D1 on NodeMCU
SoftwareSerial gpsSerial(gpsRxPin, gpsTxPin);
//GPS end

//Gyrascope beign
L3G gyro;
bool gyroAvailable = true;
//Gyrascope end

//Body Temperature beign
#define ONE_WIRE_BUS D4
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
//Body Temperature end

//max begin
MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
//max end


const char* ssid = "Redmi Note 9";
const char* password = "12345678";
const char* mqtt_server = "192.168.57.250"; // Replace with your Mosquitto broker address 192.168.57.250
const int mqtt_port = 1883;
const char* topic = "sensors/heart_pulse"; // Replace with your desired topic

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Wire.begin();
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT Broker..");
    if (client.connect("M001")) {
      Serial.println("Connected to MQTT Broker!");
    } else {
      Serial.print("failed with state ");
      Serial.println(client.state());
    }
  }

  //DHT begin
  dht.begin();
  //DHT end

  //GPS beign
  gpsSerial.begin(9600);
  //GPS beign

  //Gyrascope beign
  Wire.begin(0x0F); ///////////////////////////////////////// Change pins

  if (!gyro.init())
  {
    Serial.println("Failed to autodetect gyro type!");
    gyroAvailable = false;
    // while (1);
  }else{
    gyro.enableDefault();
  }

  
  //Gyrascope end

  //Body Temperature beign
  sensors.begin();
  //Body Temperature end

  //max begin
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST, 0x57)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
  }else{
  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

  }
  //max end
}

void loop() {
  // Serial.println(1);

  unsigned long startTime = millis();
  while(millis() - startTime < 10000){
      //GPS beign
      // Serial.println(2);
  static String receivedData;

  // Check if data is available on the GPS serial port
    while (gpsSerial.available() > 0 ) {
      // Read the incoming byte from the GPS module
      // Serial.println(3);
      char incomingByte = gpsSerial.read();

      // Check if newline character is received
      if (incomingByte == '\n') {
        Serial.println(receivedData);
        // Process the received data (in this example, just printing it)
        if(String(receivedData[4])=="L"){
          float longitudeVal = receivedData.substring(7,9).toFloat();
          float longitudeTime = receivedData.substring(9,17).toFloat();
          float latitudeVal = receivedData.substring(20,23).toFloat();
          float latitudeTime = receivedData.substring(23,31).toFloat();

          String longitude = String(longitudeVal+longitudeTime/60.0,7);
          String latitude = String(latitudeVal+latitudeTime/60.0,7);
          String location = longitude + "_" + latitude;
          
          String GPSReading = "M001," +location;
          client.publish("sensors/GPS", GPSReading.c_str(), true);
          Serial.println(location);
          receivedData = "";
          break;
          // delay(100);
        }
        
        // Reset the string for the next row of data
        receivedData = "";
      } else {
        // Accumulate the characters until newline is received
        receivedData += incomingByte;
      }
    }
  
  //GPS end

    //max begin
    long irValue = particleSensor.getIR();
    long redValue = particleSensor.getRed();

    if (checkForBeat(irValue) == true)
    {
      //We sensed a beat!
      long delta = millis() - lastBeat;
      lastBeat = millis();

      beatsPerMinute = 60 / (delta / 1000.0);

      if (beatsPerMinute < 255 && beatsPerMinute > 20)
      {
        rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
        rateSpot %= RATE_SIZE; //Wrap variable

        //Take average of readings
        beatAvg = 0;
        for (byte x = 0 ; x < RATE_SIZE ; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
      }
      float ratio = (float)redValue / (float)irValue;
      float spo2 = 104 - 17 * ratio;

      String HearRateSensorReading = "";
      String Spo2SensorReading = "";

      if (irValue < 50000){
        Serial.print(" No finger?");
        HearRateSensorReading = "M001,No finger?";
        Spo2SensorReading = "M001,No finger?";
      }
      else{
        HearRateSensorReading = "M001," + String(beatAvg);
        Spo2SensorReading = "M001," + String(spo2);
      }

      client.publish("sensors/heart_pulse", HearRateSensorReading.c_str(), true);
      client.publish("sensors/blood_oxygen", Spo2SensorReading.c_str(), true);

      Serial.print("IR=");
      Serial.print(irValue);
      Serial.print(", BPM=");
      Serial.print(beatsPerMinute);
      Serial.print(", Avg BPM=");
      Serial.print(beatAvg);
      Serial.print(", SpO2=");
      Serial.print(spo2);

      

      Serial.println();
    }
    //max end

    //Gyrascope beign
    if(gyro.init()){
      gyro.read();

      String gyrascopeReading = "M001," + String(gyro.g.x) + "," + String(gyro.g.y) + "," + String(gyro.g.z);

      client.publish("sensors/gyrascope", gyrascopeReading.c_str(), true);

      Serial.print("G ");
      Serial.print("X: ");
      Serial.print((int)gyro.g.x);
      Serial.print(" Y: ");
      Serial.print((int)gyro.g.y);
      Serial.print(" Z: ");
      Serial.println((int)gyro.g.z);

      delay(200);
    }
    
    //Gyrascope end
  }

  

  //DHT begin
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  String Shumidity = String(humidity);
  String Stemperature = String(temperature);
  // Your code to publish or subscribe to messages goes here
  // Example: publish a message to the topic
  String TemperatureSensorReading = "M001,"+Stemperature;
  String HumiditySensorReading = "M001," +Shumidity;
  delay(1000);
  client.publish("sensors/temperature", TemperatureSensorReading.c_str(), true);
  client.publish("sensors/humidity", HumiditySensorReading.c_str(), true);
  
  //DHT end


  // //Body Temperature beign
  // sensors.requestTemperatures(); 

  // String bodyTempSensorReading = "M001," + String(sensors.getTempCByIndex(0));
  // client.publish("sensors/bodyTemp", bodyTempSensorReading.c_str(), true);
  
  // // Serial.print("Celsius temperature: ");
  // // // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  // // Serial.print(sensors.getTempCByIndex(0)); 
  // // Serial.print(" - Fahrenheit temperature: ");
  // // Serial.println(sensors.getTempFByIndex(0));
  // delay(100);
  // //Body Temperature end

  //gas beign
  float n=analogRead(A0);
  String ExgasSensorReading = "M001," + String(n);
  client.publish("sensors/exgas", ExgasSensorReading.c_str(), true);
  delay(100);
  //gas end


}
