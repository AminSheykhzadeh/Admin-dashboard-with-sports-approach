#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

// Replace with your network credentials
const char* ssid = "a";
const char* password = "aaaaaaaa";

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void handleRoot() {
  String html = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  /* Add your CSS styles here */
  body { font-family: Arial, sans-serif; }
  table { width: 100%; border-collapse: collapse; }
  th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }
  th { background-color: #f2f2f2; }
</style>
<script src="https://code.highcharts.com/highcharts.js"></script>
<script src="https://code.highcharts.com/modules/exporting.js"></script>
</head>
<body>
<div id="container" style="width:100%; height:400px;"></div>
<div id="container2" style="width:100%; height:400px;"></div>
<table id="sensorTable">
  <tr><th>Timestamp</th><th>Sensor 1</th><th>Sensor 2</th></tr>
</table>
<script>
  var webSocket = new WebSocket('ws://' + window.location.hostname + ':81');
  var chart1, chart2;

  Highcharts.chart('container', {
    chart: { type: 'line', events: { load: function() { chart1 = this; } } },
    title: { text: 'Sensor 1 Data' },
    xAxis: { type: 'datetime' },
    yAxis: { title: { text: 'Value' } },
    exporting: { enabled: true },
    series: [{ name: 'Sensor 1', data: [] }]
  });

  Highcharts.chart('container2', {
    chart: { type: 'line', events: { load: function() { chart2 = this; } } },
    title: { text: 'Sensor 2 Data' },
    xAxis: { type: 'datetime' },
    yAxis: { title: { text: 'Value' } },
    exporting: { enabled: true },
    series: [{ name: 'Sensor 2', data: [] }]
  });

  webSocket.onmessage = function(event) {
  console.log("Data received: ", event.data);
  var now = new Date();
  var sensorData = JSON.parse(event.data);
  if(sensorData && sensorData.sensor1 !== undefined && sensorData.sensor2 !== undefined) {
    updateChart(chart1, now, sensorData.sensor1);
    updateChart(chart2, now, sensorData.sensor2);
    updateTable(now, sensorData.sensor1, sensorData.sensor2);
  } else {
    console.error("Invalid sensor data format: ", event.data);
  }
};

function updateChart(chart, time, value) {
  // Ensure that value is a number
  if(typeof value === 'number') {
    chart.series[0].addPoint([time.getTime(), value], true, true);
  } else {
    console.error("Invalid data type for chart value: ", value);
  }
}

  function updateTable(time, sensor1, sensor2) {
    var table = document.getElementById('sensorTable');
    var newRow = table.insertRow(1); // Insert new row after the header
    newRow.innerHTML = '<td>' + time.toISOString() + '</td><td>' + sensor1.toFixed(2) + '</td><td>' + sensor2.toFixed(2) + '</td>';
    while (table.rows.length > 11) table.deleteRow(table.rows.length - 1);
  }
</script>
</body>
</html>
)=====";
  server.send(200, "text/html", html);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  // Handle WebSocket event
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  server.on("/", handleRoot);
  server.begin();
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  webSocket.loop();
  // Dummy sensor data simulation
  static unsigned long lastUpdateTime = 0;
  if (millis() - lastUpdateTime > 1000) {
    lastUpdateTime = millis();
    float sensor1Value = analogRead(A0); // Replace with actual sensor read
    float sensor2Value = analogRead(A0); // Replace with actual sensor read
    String jsonData = "{\"sensor1\":" + String(sensor1Value) + ",\"sensor2\":" + String(sensor2Value) + "}";
    webSocket.broadcastTXT(jsonData);
  }
}
