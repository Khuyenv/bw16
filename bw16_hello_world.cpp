#include "WiFi.h"
#include "WiFiServer.h"
#include "WiFiClient.h"

char *ssid = "BW16_HelloWorld";
char *pass = "12345678";

WiFiServer server(80);

String parseRequest(String request) {
  int path_start = request.indexOf(' ') + 1;
  int path_end = request.indexOf(' ', path_start);
  return request.substring(path_start, path_end);
}

String makeResponse(int code, String content_type) {
  String response = "HTTP/1.1 " + String(code) + " OK\r\n";
  response += "Content-Type: " + content_type + "; charset=UTF-8\r\n";
  response += "Connection: close\r\n\r\n";
  return response;
}

void handleRoot(WiFiClient &client) {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Hello World - BW16</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background: #667eea;
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
            margin: 0;
            padding: 20px;
        }
        .container {
            background: white;
            border-radius: 10px;
            padding: 40px;
            text-align: center;
            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.2);
        }
        h1 {
            color: #333;
            font-size: 48px;
            margin: 0;
        }
        p {
            color: #666;
            font-size: 18px;
            margin: 20px 0;
        }
        .button {
            padding: 12px 25px;
            background: #667eea;
            color: white;
            border: none;
            border-radius: 5px;
            font-size: 16px;
            cursor: pointer;
            margin: 10px;
        }
        .button:hover {
            background: #5568d3;
        }
        #output {
            margin-top: 20px;
            padding: 15px;
            background: #f0f0f0;
            border-radius: 5px;
            font-weight: bold;
            color: #667eea;
            min-height: 30px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>👋 Hello World!</h1>
        <p>Chào mừng đến BW16 Web Server</p>
        <button class="button" onclick="sayHello()">Xin Chào</button>
        <button class="button" onclick="getTime()">Xem Giờ</button>
        <div id="output">Bấm nút để tương tác</div>
    </div>

    <script>
        function sayHello() {
            document.getElementById('output').innerHTML = '👋 Xin chào bạn!';
        }
        function getTime() {
            const now = new Date();
            const time = now.toLocaleTimeString('vi-VN');
            document.getElementById('output').innerHTML = '🕐 ' + time;
        }
    </script>
</body>
</html>
)rawliteral";

  String response = makeResponse(200, "text/html");
  response += html;
  client.write(response.c_str());
}

void setup() {
  IPAddress local_ip(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(local_ip, gateway, subnet);
  WiFi.apbegin(ssid, pass, (char *) "1");
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client.connected()) {
    String request = "";
    while (client.available()) {
      request += (char)client.read();
      delay(1);
    }

    String path = parseRequest(request);
    
    if (path == "/") {
      handleRoot(client);
    } else {
      String response = makeResponse(404, "text/html");
      response += "<h1>404 Not Found</h1>";
      client.write(response.c_str());
    }
    client.stop();
  }
  delay(10);
}
