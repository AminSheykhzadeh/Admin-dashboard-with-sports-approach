#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "a";
const char* password = "aaaaaaaa";

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html",R"=====(
    <!DOCTYPE html>
<html>
<head>
  <title>Hila | Dashboard</title>
  <link rel="stylesheet" href="https://fonts.googleapis.com/icon?family=Material+Icons">
  <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Roboto:300,400,500,700&display=swap">
  <link rel="stylesheet" href="https://code.getmdl.io/1.3.0/material.indigo-pink.min.css">
  <script defer src="https://code.getmdl.io/1.3.0/material.min.js"></script>
  <script src="https://code.highcharts.com/highcharts.js"></script>
  <script src="https://code.highcharts.com/modules/exporting.js"></script>
  <script src="https://code.highcharts.com/modules/export-data.js"></script>
  <style>
    .mdl-layout__header-row {
      justify-content: space-between;
    }
    .mdl-layout__drawer-button {
      color: white;
    }
    .content {
      padding: 20px;
    }
  </style>
</head>
<body>

  <div class="mdl-layout mdl-js-layout mdl-layout--fixed-header">
    <header class="mdl-layout__header">
      <div class="mdl-layout__header-row">
        <span class="mdl-layout-title">Hila | Dashboard</span>
      </div>
    </header>
    <div class="mdl-layout__drawer">
      <span class="mdl-layout-title">Menu</span>
      <nav class="mdl-navigation">
        <a class="mdl-navigation__link" href="">Settings</a>
        <a class="mdl-navigation__link" href="">About</a>
        <a class="mdl-navigation__link" href="">Profile</a>
        <a class="mdl-navigation__link" href="">Logout</a>
        <a class="mdl-navigation__link" href="" onclick="exportData()">Export</a>
      </nav>
    </div>
    <main class="mdl-layout__content">
      <div class="content">
        <table border="1" id="sensorTable" style="width:100%;"><tr><th>Time</th><th>Sensor 1</th><th>Sensor 2</th></tr></table>
        <div id="chart-container" style="width:100%; height:400px;"></div>
        <!-- Include additional content here -->
      </div>
    </main>
  </div>

  <script>
    // Highcharts and data-fetching script goes here
    function exportData() {
      // Implement data export functionality here
      alert('Export functionality is not implemented in this example.');
    }
  </script>

</body>
</html>
)=====");
  });

  server.on("/data", HTTP_GET, []() {
    StaticJsonDocument<200> json;
    json["sensor1"] = random(100); // Simulating sensor 1 data
    json["sensor2"] = random(100); // Simulating sensor 2 data
    String jsonString;
    serializeJson(json, jsonString);
    server.send(200, "application/json", jsonString);
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
