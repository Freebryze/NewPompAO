#include <NTPClient.h>

#include <SI114X.h>



#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


#include <WiFi.h>
#include <FirebaseESP32.h>
//#include <HTTPClient.h>


#define WIFI_SSID "PocoBen"
#define WIFI_PASSWORD "FoudreNoire"

#define FIREBASE_HOST "pompao-d3a8b-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "XtEO0hNiQfMcUrDyup6LxtscUvTlF576LzfrFOYV"


//Define the Firebase Data object
FirebaseData fbdo;
FirebaseJsonArray arr;



WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
#define DHTPIN 15

#define DHTTYPE    DHT22
DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;
int sensorPin = 26;
int sensorLightPin = 36;
float rawRange = 4096;
float logRange = 5.0;

//sunlight
SI114X SI1145 = SI114X();


void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(fbdo, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(fbdo, "tiny");

  // put your setup code here, to run once:
  pinMode(sensorPin, INPUT);
 
  Serial.begin(9600);
  // Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  while (!SI1145.Begin()) {
        Serial.println("Si1145 is not ready!");
        delay(1000);
  }
  Serial.println("Si1145 is ready!");
  //----------------------------------
  timeClient.setTimeOffset(7200);
  timeClient.begin();
}

void loop() {
  //sleep mode
/*
  esp_sleep_enable_timer_wakeup(5000000); //5 seconds
  int ret = esp_light_sleep_start();
  Serial.print("light_sleep:");
  Serial.println(ret);
*/
  
  timeClient.update();

  Serial.println(timeClient.getFormattedTime());
  Serial.println(24*(timeClient.getDay()-1) + timeClient.getHours());

  
  FirebaseJson json;

  
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    json.clear().add("temperature", event.temperature);
  }
  
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    json.add("humidity", event.relative_humidity);
  }

  //sensor humidite sol
  Serial.print(F("Humidity contact: "));
  Serial.println(analogRead(sensorPin));

  json.add("groundHumidity", analogRead(sensorPin));
  json.add("date", timeClient.getHours())
  //sunlight
  Serial.print("Vis: "); Serial.println(SI1145.ReadVisible());
  Serial.print("IR: "); Serial.println(SI1145.ReadIR());
  //the real UV value must be div 100 from the reg value , datasheet for more information.
  Serial.print("UV: ");  Serial.println((float)SI1145.ReadUV() / 100);
  Serial.print("//--------------------------------------//\r\n");

  json.add("UV", (float)SI1145.ReadUV() / 100);
  json.add("createdAt", fbdo.stringData());
  if (Firebase.set(fbdo, "/arduino0/.json" , json)) {
    Serial.println("C good");
  } else {
    Serial.print("json ERROR : ");
    Serial.println(fbdo.errorReason());
  }


  //-----------------delay 1 hour
  
  delay(3600000);
  
}
