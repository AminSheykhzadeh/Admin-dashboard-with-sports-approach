#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

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
      "<!DOCTYPE html><html><head><title>Hila | Dashboard</title>"
      "<script src='https://code.highcharts.com/highcharts.js'></script>"
      "<script src='https://code.highcharts.com/modules/exporting.js'></script>"
      "<script src='https://code.highcharts.com/modules/export-data.js'></script>"
      "</head><body>"
      "<div id='chart-container' style='width:100%; height:400px;'></div>"
      "<script>"
      "Highcharts.chart('chart-container', {"
      "  chart: {"
      "    type: 'line',"
      "    events: {"
      "      load: function () {"
      "        var series = this.series[0];"
      "        setInterval(function () {"
      "          fetch('/data').then(response => response.json()).then(data => {"
      "            var x = (new Date()).getTime(), y = data.value;"
      "            series.addPoint([x, y], true, true);"
      "          });"
      "        }, 1000);"
      "      }"
      "    }"
      "  },"
      "  time: {"
      "    useUTC: false"
      "  },"
      "  title: { text: 'Live Data' },"
      "  exporting: {"
      "    enabled: true"
      "  },"
      "  series: [{"
      "    name: 'Random Data',"
      "    data: (function () {"
      "      var data = [], time = (new Date()).getTime(), i;"
      "      for (i = -19; i <= 0; i += 1) {"
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
      "</body></html>"
    );
  });

  server.on("/data", HTTP_GET, []() {
    String json = "{\"value\": " + String(random(100)) + "}";
    server.send(200, "application/json", json);
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
