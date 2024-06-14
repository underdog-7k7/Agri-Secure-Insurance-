#include "NimBLEDevice.h" //ble 150bytes
#include <WiFi.h>
// #include "BluetoothSerial.h" //500bytes 
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 21
#define DHTTYPE DHT11
#define SSID "your_wifi_endpoint"
#define PASSWORD "your_wifi_password"
#define SERVERNAME "http://api.thingspeak.com/update"
#define API_KEY "your_thingsspeak_apikey"
#define ENCODING "application/x-www-form-urlencoded"
#define soil_data 35
#define rain_data 34


NimBLECharacteristic *pCharacteristic =NULL;
NimBLEServer *pServer = NULL;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    Serial.println("Farm Module MAC Address: E8:6B:EA:D0:A8:D6");
    dht.begin();
    WiFi.begin(SSID,PASSWORD);
    Serial.println("Connecting");

    while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }

    Serial.println("Initializing Wifi...");
    Serial.print(F("Connected to WiFi network with IP Address: "));
    Serial.println(WiFi.localIP());

    NimBLEDevice::init("AgriSecureFarmModule");  
    NimBLEServer *pServer = NimBLEDevice::createServer();
    NimBLEService *pService = pServer->createService("5f7d4fc1-75f1-4569-bcf1-11a7b4dda674");
    pCharacteristic = pService->createCharacteristic("0d3b0353-b26a-44e2-9413-d104b8af0360",NIMBLE_PROPERTY::NOTIFY | NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);
    Serial.println("Bluetooth initiliazed...");
    pService->start();
    pCharacteristic->setValue("Initial tranmission");
    Serial.println("Bluetooth starting advertisment...");
    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID("5f7d4fc1-75f1-4569-bcf1-11a7b4dda674"); 
    pAdvertising->start();
    Serial.println("Advertisement alive, check phone");

}

void loop() {
  int8_t h = dht.readHumidity();
  int8_t t = dht.readTemperature();
  int soil_mm = analogRead(soil_data);
  int rain_mm = analogRead(rain_data);

  Serial.println("Temparature: "+String(t)+"  "+"Humidity: "+String(h)+" "+"Rainfall: "+String(rain_mm)+" "+"Soil hydrogenics: "+String(soil_mm));

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Error from sensor endpoint, please check connections of all sensors on board"));
    pCharacteristic->setValue("Sensor Error, check farm_module!"); //BLE Transmission 
    pCharacteristic->notify();
    return;
  }


  String packet = "t: "+String(t)+" "+"h: "+String(h)+" "+"r: "+String(rain_mm)+" "+"s: "+String(soil_mm)+" "+"\n";
  pCharacteristic->setValue(packet); //BLE Transmission 
  pCharacteristic->notify();

  if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      http.begin(client,SERVERNAME);
      http.addHeader("Content-Type",ENCODING);
      // Data to send with HTTP POST
      String httpRequestData = "api_key=" + String(API_KEY) + "&field1=" + String(t) + "&field2=" + String(h) + "&field3="+String(rain_mm)+ "&field4="+String(soil_mm);          
      http.POST(httpRequestData);
      http.end();
    }
    else {
      Serial.println(F("Failed to update TS server,check Internet connection!"));
    }
  delay(2000);
}
