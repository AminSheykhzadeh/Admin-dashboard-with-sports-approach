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
    server.send(200, "text/html",
      "<!DOCTYPE html>"
      "<html>"
      "<head>"
      "<title>Hila | Dashboard</title>"
      "<script src='https://code.highcharts.com/highcharts.js'></script>"
      "<script src='https://code.highcharts.com/modules/exporting.js'></script>"
      "<script src='https://code.highcharts.com/modules/export-data.js'></script>"
      "</head>"
      "<body>"
      "<table border='1' id='sensorTable' style='width:100%;'><tr><th>Time</th><th>Sensor 1</th><th>Sensor 2</th></tr></table>"
      "<div id='chart-container' style='width:100%; height:400px;'></div>"
      "<script>"
      "Highcharts.chart('chart-container', {"
      "  chart: {"
      "    type: 'line',"
      "    events: {"
      "      load: function () {"
      "        var series1 = this.series[0];"
      "        var series2 = this.series[1];"
      "        setInterval(function () {"
      "          fetch('/data').then(response => response.json()).then(data => {"
      "            var x = (new Date()).getTime(), y1 = data.sensor1, y2 = data.sensor2;"
      "            series1.addPoint([x, y1], true, true);"
      "            series2.addPoint([x, y2], true, true);"
      "            var table = document.getElementById('sensorTable');"
      "            var row = table.insertRow(1);"
      "            var cell1 = row.insertCell(0);"
      "            var cell2 = row.insertCell(1);"
      "            var cell3 = row.insertCell(2);"
      "            cell1.innerHTML = new Date(x).toLocaleTimeString();"
      "            cell2.innerHTML = y1;"
      "            cell3.innerHTML = y2;"
      "          });"
      "        }, 1000);"
      "      }"
      "    }"
      "  },"
      "  time: { useUTC: false },"
      "  title: { text: 'Live Sensor Data' },"
      "  exporting: { enabled: true },"
      "  series: [{"
      "    name: 'Sensor 1',"
      "    data: (function () {"
      "      var data = [], time = (new Date()).getTime();"
      "      for (var i = -19; i <= 0; i += 1) {"
      "        data.push({"
      "          x: time + i * 1000,"
      "          y: Math.random() * 100"
      "        });"
      "      }"
      "      return data;"
      "    }())"
      "  }, {"
      "    name: 'Sensor 2',"
      "    data: (function () {"
      "      var data = [], time = (new Date()).getTime();"
      "      for (var i = -19; i <= 0; i += 1) {"
      "        data.push({"
      "          x: time + i * 1000,"
      "          y: Math.random() * 100"
      "        });"
      "      }"
      "      return data;"
      "    }())"
      "  }]"
      "});"
      "</script>"
      "</body>"
      "</html>"
    );
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
