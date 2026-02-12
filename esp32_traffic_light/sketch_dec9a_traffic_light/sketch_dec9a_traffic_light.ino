  #include <WiFi.h>
  #include <Firebase_ESP_Client.h>

  // Insert RTDB URLefine the RTDB URL */
  #define DATABASE_URL ""
  // Insert Firebase project API Key
  #define DATABASE_SECRET ""

  // Insert your network credentials
  #define WIFI_SSID ""
  #define WIFI_PASSWORD ""

  // #define WIFI_SSID "TestingIoT"
  // #define WIFI_PASSWORD "UjiCobaIoT"

  //Define Firebase Data object
  FirebaseData fbdo;

  FirebaseAuth auth;
  FirebaseConfig config;

  #define PIN_RED_A     26 // The Arduino pin connected to R pin of traffic light module
  #define PIN_YELLOW_A  27// The Arduino pin connected to Y pin of traffic light module
  #define PIN_GREEN_A   14 // The Arduino pin connected to G pin of traffic light module

  #define PIN_RED_B     4 // The Arduino pin connected to R pin of traffic light module
  #define PIN_YELLOW_B  2// The Arduino pin connected to Y pin of traffic light module
  #define PIN_GREEN_B   15 // The Arduino pin connected to G pin of traffic light module

  #define PIN_RED_C    21 // The Arduino pin connected to R pin of traffic light module
  #define PIN_YELLOW_C 18// The Arduino pin connected to Y pin of traffic light module
  #define PIN_GREEN_C  19 // The Arduino pin connected to G pin of traffic light module

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
    pinMode(PIN_RED_A, OUTPUT);
    pinMode(PIN_YELLOW_A, OUTPUT);
    pinMode(PIN_GREEN_A, OUTPUT);
    pinMode(PIN_RED_B, OUTPUT);
    pinMode(PIN_YELLOW_B, OUTPUT);
    pinMode(PIN_GREEN_B, OUTPUT);
    pinMode(PIN_RED_C, OUTPUT);
    pinMode(PIN_YELLOW_C, OUTPUT);
    pinMode(PIN_GREEN_C, OUTPUT);
  }

  void yellow(int red, int yellow, int green){
      digitalWrite(red, LOW);
      digitalWrite(yellow, HIGH);
      digitalWrite(green, LOW);

      delay(3000);
  }

   void red(int red, int yellow, int green, int time){
      digitalWrite(red, HIGH);
      digitalWrite(yellow, LOW);
      digitalWrite(green, LOW);

      delay(time);
  }

   void green(int red, int yellow, int green, int time){
      digitalWrite(red, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(green, HIGH);

      delay(time);
  }

  // the loop function runs over and over again forever
  void loop() {
    // Serial.print(".");
    // delay(100);
    if (!Firebase.RTDB.getInt(&fbdo, F("/smart_traffic/status_a")) || !Firebase.RTDB.getInt(&fbdo, F("/smart_traffic/status_b")) || !Firebase.RTDB.getInt(&fbdo, F("/smart_traffic/status_c"))) {
      Serial.println("Failed to get status_a. Retrying...");
      delay(500); // Add a delay before retrying
      return;
    }
    String ultra_a = Firebase.RTDB.getInt(&fbdo, F("/smart_traffic/status_a")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str();
    String ultra_b = Firebase.RTDB.getInt(&fbdo, F("/smart_traffic/status_b")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str();
    String ultra_c = Firebase.RTDB.getInt(&fbdo, F("/smart_traffic/status_c")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str();
    String cam = Firebase.RTDB.getInt(&fbdo, F("/smart_traffic/object")) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str();
    // red light on

    Serial.println("Status A : "+ ultra_a);
    Serial.println("Status B : "+ ultra_b);
    Serial.println("Status C : "+ ultra_c);
    Serial.println("Object : " + cam);
    if(ultra_c == "3"){
      red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
      red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
      green(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 10000);
      yellow(PIN_RED_C,  PIN_YELLOW_C, PIN_GREEN_C);
    }else if(ultra_c == "2"){
      red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
      red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
      green(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 15000);
      yellow(PIN_RED_C,  PIN_YELLOW_C, PIN_GREEN_C);
    }else if(ultra_c == "1"){
      red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
      red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
      green(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 20000);
      yellow(PIN_RED_C,  PIN_YELLOW_C, PIN_GREEN_C);
    }

    if(ultra_b == "3" || cam == "1"){
      red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
      red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
      green(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 10000);
      yellow(PIN_RED_B,  PIN_YELLOW_B, PIN_GREEN_B);
    }else if(ultra_b == "2" || cam == "2"){
      red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
      red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
      green(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 15000);
      yellow(PIN_RED_B,  PIN_YELLOW_B, PIN_GREEN_B);
    }else if(ultra_b == "1" || cam.toInt() >= 3){
      red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
      red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
      green(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 20000);
      yellow(PIN_RED_B,  PIN_YELLOW_B, PIN_GREEN_B);
    }

    if(ultra_a == "3"){
      red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
      red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
      green(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 10000);
      yellow(PIN_RED_A,  PIN_YELLOW_A, PIN_GREEN_A);
    }else if(ultra_a == "2"){
      red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
      red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
      green(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 15000);
      yellow(PIN_RED_A,  PIN_YELLOW_A, PIN_GREEN_A);
    }else if(ultra_a == "1"){
      red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
      red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
      green(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 20000);
      yellow(PIN_RED_A,  PIN_YELLOW_A, PIN_GREEN_A);
    }

    // if(ultra_a == "3" && ultra_b == "3" && ultra_c == "3"){
    //   // C Green
    //   red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
    //   red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
    //   green(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 10000);
    //   yellow(PIN_RED_C,  PIN_YELLOW_C, PIN_GREEN_C);
      
    //   // B Green
    //   red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
    //   red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
    //   green(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 10000);
    //   yellow(PIN_RED_B,  PIN_YELLOW_B, PIN_GREEN_B);
      
    //   // A Green
    //   red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
    //   red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
    //   green(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 10000);
    //   yellow(PIN_RED_A,  PIN_YELLOW_A, PIN_GREEN_A);

    // }
    // if(ultra_c == "2"){
    //   // C Green
    //   red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
    //   red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
    //   green(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 15000);
    //   yellow(PIN_RED_C,  PIN_YELLOW_C, PIN_GREEN_C);
      
    //   // B Green
    //   red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
    //   red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
    //   green(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 10000);
    //   yellow(PIN_RED_B,  PIN_YELLOW_B, PIN_GREEN_B);
      
    //   // A Green
    //   red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
    //   red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
    //   green(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 10000);
    //   yellow(PIN_RED_A,  PIN_YELLOW_A, PIN_GREEN_A);

    // }
    // if(ultra_b == "2" || cam == "2"){
    //   // C Green
    //   red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
    //   red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
    //   green(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 10000);
    //   yellow(PIN_RED_C,  PIN_YELLOW_C, PIN_GREEN_C);
      
    //   // B Green
    //   red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
    //   red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
    //   green(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 15000);
    //   yellow(PIN_RED_B,  PIN_YELLOW_B, PIN_GREEN_B);
      
    //   // A Green
    //   red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
    //   red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
    //   green(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 10000);
    //   yellow(PIN_RED_A,  PIN_YELLOW_A, PIN_GREEN_A);

    // }
    // if(ultra_a == "2"){
    //   // C Green
    //   red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
    //   red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
    //   green(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 10000);
    //   yellow(PIN_RED_C,  PIN_YELLOW_C, PIN_GREEN_C);
      
    //   // B Green
    //   red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
    //   red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
    //   green(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 10000);
    //   yellow(PIN_RED_B,  PIN_YELLOW_B, PIN_GREEN_B);
      
    //   // A Green
    //   red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
    //   red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
    //   green(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 15000);
    //   yellow(PIN_RED_A,  PIN_YELLOW_A, PIN_GREEN_A);

    // }
    // if(ultra_c == "1"){
    //   // C Green
    //   red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
    //   red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
    //   green(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 20000);
    //   yellow(PIN_RED_C,  PIN_YELLOW_C, PIN_GREEN_C);
      
    //   // B Green
    //   red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
    //   red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
    //   green(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 10000);
    //   yellow(PIN_RED_B,  PIN_YELLOW_B, PIN_GREEN_B);
      
    //   // A Green
    //   red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
    //   red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
    //   green(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 15000);
    //   yellow(PIN_RED_A,  PIN_YELLOW_A, PIN_GREEN_A);

    // }
    // if(ultra_b == "1" || cam == "3"){
    //   // C Green
    //   red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
    //   red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
    //   green(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 10000);
    //   yellow(PIN_RED_C,  PIN_YELLOW_C, PIN_GREEN_C);
      
    //   // B Green
    //   red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
    //   red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
    //   green(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 20000);
    //   yellow(PIN_RED_B,  PIN_YELLOW_B, PIN_GREEN_B);
      
    //   // A Green
    //   red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
    //   red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
    //   green(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 10000);
    //   yellow(PIN_RED_A,  PIN_YELLOW_A, PIN_GREEN_A);

    // }
    // if(ultra_a == "1"){
    //   // C Green
    //   red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
    //   red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
    //   green(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 10000);
    //   yellow(PIN_RED_C,  PIN_YELLOW_C, PIN_GREEN_C);
      
    //   // B Green
    //   red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
    //   red(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 0);
    //   green(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 10000);
    //   yellow(PIN_RED_B,  PIN_YELLOW_B, PIN_GREEN_B);
      
    //   // A Green
    //   red(PIN_RED_B, PIN_YELLOW_B, PIN_GREEN_B, 0);
    //   red(PIN_RED_C, PIN_YELLOW_C, PIN_GREEN_C, 0);
    //   green(PIN_RED_A, PIN_YELLOW_A, PIN_GREEN_A, 20000);
    //   yellow(PIN_RED_A,  PIN_YELLOW_A, PIN_GREEN_A);

    // }
  }
