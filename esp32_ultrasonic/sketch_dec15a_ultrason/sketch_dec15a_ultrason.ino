#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL ""
// Insert Firebase project API Key
#define DATABASE_SECRET ""

// Insert your network credentials
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;


#define maxCapacity 3

struct US_LED {
  String ID;
  int TRIG_PIN;
  int ECHO_PIN;
  // int LED_PIN_ZERO;
  // int LED_PIN_FULL;
  // int isFUll;
};

US_LED trafficSensor_a[maxCapacity] = {
  {"1", 12,13},
  {"2", 27,14},
  {"3", 26,25},
};

US_LED trafficSensor_b[maxCapacity] = {
  {"1", 2,15},
  {"2", 4,16},
  {"3", 17,5},
};

US_LED trafficSensor_c[maxCapacity] = {
  {"1", 32,33},
  {"2", 23,22},
  {"3", 18,19},
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("WIFI CONNECTED");

  /* Assign the api key (required) */
  config.signer.tokens.legacy_token = DATABASE_SECRET;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

float readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}

float cm;


String trafficStatus(float distance1, float distance2, float distance3){
  if((distance1 <= 10) && (distance2 > 10) && (distance3 > 10)){
    return "lancar";
  }
  else if((distance1 <= 10) && (distance2 <= 10) && (distance3 > 10)){
    return "macet";
  }
  else if((distance1 <= 10) && (distance2 <= 10) && (distance3 <= 10)){
    return "sangat macet";
  }else{
    return "lancar";
  }
}

void sendStatusToFirebase_a(String status){
  float distance[maxCapacity];
  
  if(status == "macet"){
    // Jika Macet
    for(int i = 0; i < maxCapacity; i++){
      cm = readUltrasonicDistance(trafficSensor_a[i].TRIG_PIN, trafficSensor_a[i].ECHO_PIN) / 58;
      distance[i] = cm;
    }

    if((distance[2] <= 10) && (distance[1] <= 10) && (distance[0] > 10)){
      Serial.println("Status a : 2");

      Firebase.RTDB.setInt(&fbdo, F("/smart_traffic/status_a"), 2) ? "ok" : fbdo.errorReason().c_str();
    }
  }else if(status == "sangat macet"){
    // Jika sangat Macet
    for(int i = 0; i < maxCapacity; i++){
      cm = readUltrasonicDistance(trafficSensor_a[i].TRIG_PIN, trafficSensor_a[i].ECHO_PIN) / 58;
      distance[i] = cm;
    }
    if((distance[2] <= 10) && (distance[1] <= 10) && (distance[0] <= 10)){
      Serial.println("Status a : 1");

      Firebase.RTDB.setInt(&fbdo, F("/smart_traffic/status_a"), 1) ? "ok" : fbdo.errorReason().c_str();
    }
  }else{
    // Jika Lancar
      Serial.println("Status a : 3");
      Firebase.RTDB.setInt(&fbdo, F("/smart_traffic/status_a"), 3) ? "ok" : fbdo.errorReason().c_str();
  }

  Serial.println("");
}

void sendStatusToFirebase_b(String status){
  float distance[maxCapacity];
  
  if(status == "macet"){
    // Jika Macet
    for(int i = 0; i < maxCapacity; i++){
      cm = readUltrasonicDistance(trafficSensor_b[i].TRIG_PIN, trafficSensor_b[i].ECHO_PIN) / 58;
      distance[i] = cm;
    }

    if((distance[2] <= 10) && (distance[1] <= 10) && (distance[0] > 10)){
      Serial.println("Status b : 2");

      Firebase.RTDB.setInt(&fbdo, F("/smart_traffic/status_b"), 2) ? "ok" : fbdo.errorReason().c_str();
    }
  }else if(status == "sangat macet"){
    // Jika sangat Macet
    for(int i = 0; i < maxCapacity; i++){
      cm = readUltrasonicDistance(trafficSensor_b[i].TRIG_PIN, trafficSensor_b[i].ECHO_PIN) / 58;
      distance[i] = cm;
    }
    if((distance[2] <= 10) && (distance[1] <= 10) && (distance[0] <= 10)){
      Serial.println("Status b : 1");

      Firebase.RTDB.setInt(&fbdo, F("/smart_traffic/status_b"), 1) ? "ok" : fbdo.errorReason().c_str();
    }
  }else{
    // Jika Lancar
      Serial.println("Status b : 3");
      Firebase.RTDB.setInt(&fbdo, F("/smart_traffic/status_b"), 3) ? "ok" : fbdo.errorReason().c_str();
  }

  Serial.println("");
}

void sendStatusToFirebase_c(String status){
  float distance[maxCapacity];
  
  if(status == "macet"){
    // Jika Macet
    for(int i = 0; i < maxCapacity; i++){
      cm = readUltrasonicDistance(trafficSensor_c[i].TRIG_PIN, trafficSensor_c[i].ECHO_PIN) / 58;
      distance[i] = cm;
    }

    if((distance[2] <= 10) && (distance[1] <= 10) && (distance[0] > 10)){
      Serial.println("Status c : 2");

      Firebase.RTDB.setInt(&fbdo, F("/smart_traffic/status_c"), 2) ? "ok" : fbdo.errorReason().c_str();
    }
  }else if(status == "sangat macet"){
    // Jika sangat Macet
    for(int i = 0; i < maxCapacity; i++){
      cm = readUltrasonicDistance(trafficSensor_c[i].TRIG_PIN, trafficSensor_c[i].ECHO_PIN) / 58;
      distance[i] = cm;
    }
    if((distance[2] <= 10) && (distance[1] <= 10) && (distance[0] <= 10)){
      Serial.println("Status c  : 1");

      Firebase.RTDB.setInt(&fbdo, F("/smart_traffic/status_c"), 1) ? "ok" : fbdo.errorReason().c_str();
    }
  }else{
    // Jika Lancar
      Serial.println("Status c : 3");
      Firebase.RTDB.setInt(&fbdo, F("/smart_traffic/status_c"), 3) ? "ok" : fbdo.errorReason().c_str();
  }

  Serial.println("");
}

void updateTrafficStatus_a() {
  float distance[maxCapacity];
  for(int i = 0; i < maxCapacity; i++){
    cm = readUltrasonicDistance(trafficSensor_a[i].TRIG_PIN, trafficSensor_a[i].ECHO_PIN) / 58;
    distance[i] = cm;
    String cmS = String(trafficSensor_a[i].ID) + " distance in CM : " + String(cm);
    Serial.println(cmS);
  }
  String status = trafficStatus(distance[2], distance[1], distance[0]);
  Serial.println("Status a : " + status);
  delay(5000);
  sendStatusToFirebase_a(status);
}

void updateTrafficStatus_b() {
  float distance[maxCapacity];
  for(int i = 0; i < maxCapacity; i++){
    cm = readUltrasonicDistance(trafficSensor_b[i].TRIG_PIN, trafficSensor_b[i].ECHO_PIN) / 58;
    distance[i] = cm;
    String cmS = String(trafficSensor_b[i].ID) + " distance in CM : " + String(cm);
    Serial.println(cmS);
  }
  String status = trafficStatus(distance[2], distance[1], distance[0]);
  Serial.println("Status b : " + status);
  delay(5000);
  sendStatusToFirebase_b(status);
}

void updateTrafficStatus_c() {
  float distance[maxCapacity];
  for(int i = 0; i < maxCapacity; i++){
    cm = readUltrasonicDistance(trafficSensor_c[i].TRIG_PIN, trafficSensor_c[i].ECHO_PIN) / 58;
    distance[i] = cm;
    String cmS = String(trafficSensor_c[i].ID) + " distance in CM : " + String(cm);
    Serial.println(cmS);
  }
  String status = trafficStatus(distance[2], distance[1], distance[0]);
  Serial.println("Status c : " + status);
  delay(5000);
  sendStatusToFirebase_c(status);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("hello");
  updateTrafficStatus_a();
  updateTrafficStatus_b();
  updateTrafficStatus_c(); 
  delay(1000);
}