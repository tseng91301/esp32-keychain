#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32-C3 Web</title>
</head>
<body>
  <h1>Hello from ESP32-C3 SuperMini!</h1>
  <button onclick="alert('ESP32 Clicked!')">Click Me</button>
</body>
</html>

)rawliteral";
