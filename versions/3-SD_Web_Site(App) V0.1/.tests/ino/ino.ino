#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <SD.h>

const char* ssid = "a";
const char* password = "aaaaaaaa";

ESP8266WebServer server(80);
WebSocketsServer webSocket(81);

const int sensor1Pin = A0;
const int sensor2Pin = A0;
unsigned long past = 0;

float sensor1Value = 0;
float sensor2Value = 0;

void handleNotFound();
void handleRoot();
void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi !");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");

  server.on("/", HTTP_GET, handleRoot);
  server.onNotFound(handleNotFound);

  if (SD.begin(4)) {
    Serial.println("SD card initialized");
  } else {
    Serial.println("Error initializing SD card");
  }

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  
   
  server.handleClient();
  webSocket.loop();

  if(millis()-past>1500){
  // Simulate sensor data (replace with actual sensor readings)
  sensor1Value = random(0, 1000);
  sensor2Value = random(0, 1000);

  // Send data to connected clients
  String sensorData = "{\"sensor1\":" + String(sensor1Value/10) + ",\"sensor2\":" + String(sensor2Value/10) + "}";
  webSocket.broadcastTXT(sensorData);
  Serial.println(sensorData);
  past = millis();
  }
}

void handleRoot() {
  //  String html = R"=====(
  //   <!DOCTYPE html>
  //   <html>
  //   <head>
  //       <meta charset="UTF-8">
  //       <title>Real-time Sensor Data</title>

  //       <!-- Include Highcharts library -->
  //       <script src="https://code.highcharts.com/highcharts.js"></script>

  //       <!-- Include jQuery library (required by Highcharts) -->
  //       <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>

  //       <!-- Include Highcharts exporting module for chart export functionality -->
  //       <script src="https://code.highcharts.com/modules/exporting.js"></script>

  //       <!-- Include Highcharts WebSocket Adapter -->
  //       <script src="https://code.highcharts.com/adapters/standalone-framework.js"></script>
  //   </head>
  //   <body>
  //       <div id="container" style="width: 800px; height: 400px;"></div>

  //       <script>
  //           // Create a Highcharts chart
  //           Highcharts.setOptions({
  //               global: {
  //                   useUTC: false
  //               }
  //           });

  //           var chart;
  //           $(document).ready(function () {
  //               chart = new Highcharts.Chart({
  //                   chart: {
  //                       renderTo: 'container',
  //                       type: 'spline',
  //                       events: {
  //                           load: function () {
  //                               // Set up WebSocket connection
  //                               var socket = new WebSocket('ws://' + window.location.hostname + ':81/');
  //                               socket.onmessage = function (event) {
  //                                   console.log('Received data:', event.data); // Log received data to the console
  //                                   var data = JSON.parse(event.data);
  //                                   var x = (new Date()).getTime();
  //                                   var y1 = data.sensor1;
  //                                   var y2 = data.sensor2;
                                    
  //                                   // Add data to the chart series
  //                                   chart.series[0].addPoint([x, y1], true, true);
  //                                   chart.series[1].addPoint([x, y2], true, true);

  //                                   // Debugging: Log added data points to the console
  //                                   console.log('Added data points:', x, y1, y2);
  //                               };
  //                           }
  //                       }
  //                   },
  //                   title: {
  //                       text: 'Real-time Sensor Data'
  //                   },
  //                   xAxis: {
	// 	        type: 'datetime',
	// 	        tickPixelInterval: 150,
	// 	        labels: {
	// 	            formatter: function () {
  //   		            return Highcharts.dateFormat('%Y-%m-%d %H:%M:%S', this.value);
	// 	            }
  //   		    }
	// 	    },
  //                   yAxis: {
  //                       title: {
  //                           text: 'Value'
  //                       }
  //                   },
  //                   series: [
  //                       {
  //                           name: 'Sensor 1',
  //                           data: []
  //                       },
  //                       {
  //                           name: 'Sensor 2',
  //                           data: []
  //                       }
  //                   ],
  //                   credits: {
  //                       enabled: false
  //                   },
  //                   exporting: {
  //                       enabled: true
  //                   }
  //               });
  //           });
  //       </script>
  //   </body>
  //   </html>

  //   )=====";

  //  server.send(200, "text/html", html);

  File webPage = SD.open("/index.html");
  if (webPage) {
    server.streamFile(webPage, "text/html");
    webPage.close();
  } else {
    server.send(404, "text/plain", "File not found");
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  // Handle WebSocket events (if needed)
}
void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}



