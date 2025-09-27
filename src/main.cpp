#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include "index_html.h"  // 自動生成

// 修改成你的 WiFi 資料
const char *ssid = "105_router";
const char *password = "1029384756";

AsyncWebServer server(80);

void setup() {
    delay(1000);

    printf("Connecting to WiFi...\n");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        printf(".");
    }
    printf("\nWiFi Connected!\n");

    IPAddress ip = WiFi.localIP();
    printf("IP Address: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", index_html);
    });

    server.begin();
    printf("HTTP server started\n");
}

void loop() {
    // 不需要內容，AsyncWebServer 自動處理請求
}
