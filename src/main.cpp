#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "ESP32_AP";
const char *password = "12345678";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

struct PinConfig {
  String mode = "input"; // input, output, pwm
  int value = 0;         // LOW/HIGH 或 duty cycle
};

PinConfig pinConfigs[22];  // ESP32-C3 大約 22 GPIO

// 處理 WebSocket 訊息
void handleWsMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    String msg = String((char*)data).substring(0, len);
    printf("WS Msg: %s\n", msg.c_str());

    // 格式: set,<pin>,<mode>,<value>
    // 例如: set,8,output,1
    int pin, value;
    char mode[10];
    if (sscanf(msg.c_str(), "set,%d,%9[^,],%d", &pin, mode, &value) == 3) {
      pinConfigs[pin].mode = String(mode);
      pinConfigs[pin].value = value;

      if (pinConfigs[pin].mode == "input") {
        pinMode(pin, INPUT);
      } else if (pinConfigs[pin].mode == "output") {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, value ? HIGH : LOW);
      } else if (pinConfigs[pin].mode == "pwm") {
        pinMode(pin, OUTPUT);
        int channel = pin % 8;  // 簡單分配 8 個 channel
        ledcAttachPin(pin, channel);
        ledcSetup(channel, 5000, 8); // 5kHz, 8-bit
        ledcWrite(channel, value);  // duty
      }
    }
  }
}

// 提供 HTML
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>ESP32 GPIO Controller</title>
  <style>
    body { font-family: sans-serif; }
    .pin { margin: 5px; padding: 10px; border: 1px solid #ccc; display: inline-block; }
  </style>
</head>
<body>
  <h2>ESP32 GPIO Controller</h2>
  <div id="pins"></div>

  <script>
    var ws = new WebSocket("ws://" + location.host + "/ws");

    function setPin(pin, mode, value) {
      ws.send("set," + pin + "," + mode + "," + value);
    }

    function createUI() {
      let pinsDiv = document.getElementById("pins");
      for (let i = 0; i < 22; i++) {
        let div = document.createElement("div");
        div.className = "pin";
        div.innerHTML = `
          <h4>GPIO${i}</h4>
          <select id="mode${i}" onchange="updateMode(${i})">
            <option value="input">Input</option>
            <option value="output">Output</option>
            <option value="pwm">PWM</option>
          </select>
          <div id="control${i}">
            <button onclick="setPin(${i},'output',1)">HIGH</button>
            <button onclick="setPin(${i},'output',0)">LOW</button>
          </div>
        `;
        pinsDiv.appendChild(div);
      }
    }

    function updateMode(pin) {
      let mode = document.getElementById("mode"+pin).value;
      let ctrl = document.getElementById("control"+pin);
      if (mode === "input") {
        ctrl.innerHTML = "(read only)";
        setPin(pin, "input", 0);
      } else if (mode === "output") {
        ctrl.innerHTML = `
          <button onclick="setPin(${pin},'output',1)">HIGH</button>
          <button onclick="setPin(${pin},'output',0)">LOW</button>
        `;
        setPin(pin, "output", 0);
      } else if (mode === "pwm") {
        ctrl.innerHTML = `
          <input type="range" min="0" max="255" value="0" oninput="setPin(${pin},'pwm',this.value)">
        `;
        setPin(pin, "pwm", 0);
      }
    }

    createUI();
  </script>
</body>
</html>
)rawliteral";

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
  if(type == WS_EVT_DATA){
    handleWsMessage(arg, data, len);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  IPAddress ip = WiFi.softAPIP();
  printf("IP Address: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", index_html);
  });

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.begin();
}

void loop() {}
