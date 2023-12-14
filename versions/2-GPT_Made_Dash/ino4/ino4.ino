#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "a";
const char* password = "aaaaaaaa";

ESP8266WebServer server(80);

const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>Hila | Dashboard</title>
  <link rel="stylesheet" href="https://fonts.googleapis.com/icon?family=Material+Icons">
  <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Roboto:300,400,500,700&display=swap">
  <link rel="stylesheet" href="https://code.getmdl.io/1.3.0/material.indigo-pink.min.css">
  <script defer src="https://code.getmdl.io/1.3.0/material.min.js"></script>
  <script src="https://code.jquery.com/jquery-3.5.1.min.js"></script>
  <script src="https://code.highcharts.com/highcharts.js"></script>
  <script src="https://code.highcharts.com/modules/exporting.js"></script>
  <script src="https://code.highcharts.com/modules/export-data.js"></script>
  <script src="https://code.highcharts.com/maps/modules/map.js"></script>
  <style>
    .table-container {
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
    <div class="table-container">
      <table id="sensorTable" class="mdl-data-table mdl-js-data-table mdl-data-table--selectable mdl-shadow--2dp" style="width:100%;">
        <thead>
          <tr>
            <th class="mdl-data-table__cell--non-numeric">Time</th>
            <th>Sensor 1</th>
            <th>Sensor 2</th>
          </tr>
        </thead>
        <tbody>
          <!-- Sensor data rows will be inserted here -->
        </tbody>
      </table>
    </div>
    <div id="sensorChart" style="width:100%; height:400px;"></div>
  </main>
</div>

<script>
  // Dummy data for the table
  var sensorData = [
    { time: '2023-11-05 12:00', sensor1: 20, sensor2: 30 },
    // Add more sensor data here
  ];

  // Function to populate the sensor data table
  function populateTable(data) {
    var tableBody = document.querySelector('#sensorTable tbody');
    tableBody.innerHTML = ''; // Clear existing data
    data.forEach(function(row) {
      var tr = document.createElement('tr');
      tr.innerHTML = '<td class="mdl-data-table__cell--non-numeric">' + row.time + '</td>' +
                     '<td>' + row.sensor1 + '</td>' +
                     '<td>' + row.sensor2 + '</td>';
      tableBody.appendChild(tr);
    });
  }

  // Call the function to populate the table
  populateTable(sensorData);

  // Highcharts plot configuration
  Highcharts.chart('sensorChart', {
    chart: {
      type: 'line'
    },
    title: {
      text: 'Sensor Data'
    },
    xAxis: {
      categories: sensorData.map(function(data) { return data.time; })
    },
    yAxis: {
      title: {
        text: 'Value'
      }
    },
    series: [{
      name: 'Sensor 1',
      data: sensorData.map(function(data) { return data.sensor1; })
    }, {
      name: 'Sensor 2',
      data: sensorData.map(function(data) { return data.sensor2; })
    }]
  });

  function exportData() {
    // Implement data export functionality here
    alert('Export functionality is not implemented in this example.');
  }
</script>

</body>
</html>
)=====";

void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print the local IP address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", handleRoot);

  // Start server
  server.begin();
}

void loop() {
  server.handleClient();
}
