# Mistero-Servo-Control
Password for Wifi - 1803@###

Wifi-Name - Mistero-Servo
# Preference-Url
```http://arduino.esp8266.com/stable/package_esp8266com_index.json```
# Search On Default Browser
192.168.4.1
# Connection
Connect the Servo-PWM(orange wire) to D7 of esp8266

Connect the  Servo red wire to Vin of esp8266 or external 5v power source

Connect Servo grey wire to gnd of esp8266

NOTE- Use this connection only if powering the esp8266 through micro usb.



# Source-Code
```ino
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

const char* ssid = "Mistero-Servo"; // You can change your wifi-name
const char* password = "1803@###"; // You can change your password
Servo myServo;

ESP8266WebServer server(80);

int servoPin = D7;   // GPIO2 
int servoPos = 0;
#define LED_PIN LED_BUILTIN

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Mistero-Servo-Control</title>
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; }
    button { font-size: 20px; padding: 15px 30px; margin: 10px; }
  </style>
</head>
<body>
  <h2>Servo Control</h2>
  <button onclick="fetch('/set?pos=115'); setTimeout(() => { fetch('/set?pos=90'); }, 400);">Off</button>   // Change the degree 115 to your convenience automatically it comes to 90 degree after turning on off the switch 
  <button onclick="fetch('/set?pos=65'); setTimeout(() => { fetch('/set?pos=90'); }, 400);">ON</button>    //  Change the degree 65 to your convenience automatically it comes to 90 degree after turning on off the switch -->
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

void handleSet() {
  if (server.hasArg("pos")) {
    servoPos = server.arg("pos").toInt();
    servoPos = constrain(servoPos, 0, 180);
    myServo.write(servoPos);
    server.send(200, "text/plain", "Servo set to " + String(servoPos));
  } else {
    server.send(400, "text/plain", "Missing position");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // LED OFF (active LOW)  // Want to on the onboard led just replace HIGH to LOW

  myServo.attach(servoPin, 500, 2500);
  myServo.write(90); // Initial position of servo, You can change at your convenience 

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}



```
