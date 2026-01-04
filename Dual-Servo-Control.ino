#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

const char* ssid = "Mistero-Dual-Servo";
const char* password = "MisteroV2";

ESP8266WebServer server(80);

// ===== Servo Objects =====
Servo servo1;
Servo servo2;

// ===== Servo Pins =====
int servo1Pin = D7;   // GPIO13
int servo2Pin = D6;   // GPIO12

// ===== Servo Positions =====
int servo1Pos = 0;
int servo2Pos = 0;

#define LED_PIN LED_BUILTIN

// ===== Root Page =====
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <title>Mistero-Servo-Controller</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin-top: 40px;
        }

        button {
            font-size: 18px;
            padding: 25px 25px;
            margin: 30px;
            cursor: pointer;
        }

        .box {
            border: 1px solid #ccc;
            padding: 15px;
            margin: 25px;
            display: inline-block;
            width: 480px;
        }
    </style>
</head>

<body>

    <h2>Hello, Buddy</h2>

    <!-- LIGHT CONTROL -->
    <div class="box">
        <h3>Light Control</h3>
        <button onclick="moveServo('/set', 142)">Light ON</button>
        <button onclick="moveServo('/set2', 45)">Light OFF</button>
    </div>

    <!-- FAN CONTROL -->
    <div class="box">
        <h3>Fan Control</h3>
        <button onclick="moveServo('/set', 43)">Fan ON</button>
        <button onclick="moveServo('/set2', 132)">Fan OFF</button>
    </div>

    <script>
        function moveServo(endpoint, position) {
            fetch(`${endpoint}?pos=${position}`);
            setTimeout(() => {
                fetch(`${endpoint}?pos=90`); // return to neutral
            }, 400);
        }
    </script>

</body>

</html>
)rawliteral";

  server.send(200, "text/html", html);
}

// ===== Servo 1 Handler =====
void handleSetServo1() {
  if (server.hasArg("pos")) {
   servo1Pos = server.arg("pos").toInt();
    servo1Pos = constrain(servo1Pos, 0, 180);
    servo1.write(servo1Pos);
    server.send(200, "text/plain", "Servo 1 set to " + String(servo1Pos));
  } else {
    server.send(400, "text/plain", "Missing position");
  }
}

// ===== Servo 2 Handler =====
void handleSetServo2() {
  if (server.hasArg("pos")) {
    servo2Pos = server.arg("pos").toInt();
    servo2Pos = constrain(servo2Pos, 0, 180);
   
    servo2.write(servo2Pos);
    server.send(200, "text/plain", "Servo 2 set to " + String(servo2Pos));
  } else {
    server.send(400, "text/plain", "Missing position");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // LED OFF

  // ===== Attach Servos =====
  servo1.attach(servo1Pin, 500, 2500);
  servo2.attach(servo2Pin, 500, 2500);

  servo1.write(90);
  servo2.write(90);

  // ===== WiFi AP =====
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // ===== Routes =====
  server.on("/", handleRoot);
  server.on("/set", handleSetServo1);
  server.on("/set2", handleSetServo2);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
