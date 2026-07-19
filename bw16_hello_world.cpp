#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#include "vector"
#include "wifi_conf.h"
#include "map"
#include "wifi_util.h"
#include "debug.h"
#include "WiFi.h"
#include "WiFiServer.h"
#include "WiFiClient.h"

// ==================== CONFIG ====================
char *ssid = "BW16_HelloWorld";
char *pass = "12345678";

// ==================== GLOBAL VARIABLES ====================
WiFiServer server(80);
int request_count = 0;

// ==================== HELPER FUNCTIONS ====================

// Parse HTTP request path
String parseRequest(String request) {
  int path_start = request.indexOf(' ') + 1;
  int path_end = request.indexOf(' ', path_start);
  return request.substring(path_start, path_end);
}

// Create HTTP response header
String makeResponse(int code, String content_type) {
  String response = "HTTP/1.1 " + String(code) + " OK\r\n";
  response += "Content-Type: " + content_type + "; charset=UTF-8\r\n";
  response += "Connection: close\r\n";
  response += "Access-Control-Allow-Origin: *\r\n\r\n";
  return response;
}

// ==================== PAGE HANDLERS ====================

// Handler cho trang Hello World
void handleRoot(WiFiClient &client) {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Hello World - BW16</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
            padding: 20px;
        }

        .container {
            background: white;
            border-radius: 15px;
            box-shadow: 0 20px 60px rgba(0, 0, 0, 0.3);
            padding: 50px;
            text-align: center;
            max-width: 700px;
            animation: slideIn 0.6s ease-out;
        }

        @keyframes slideIn {
            from {
                opacity: 0;
                transform: translateY(-40px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }

        .emoji {
            font-size: 80px;
            margin-bottom: 20px;
            animation: wave 1s ease-in-out infinite;
        }

        @keyframes wave {
            0%, 100% { transform: rotate(0deg); }
            25% { transform: rotate(20deg); }
            75% { transform: rotate(-20deg); }
        }

        h1 {
            color: #333;
            font-size: 56px;
            margin-bottom: 15px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            background-clip: text;
        }

        .subtitle {
            color: #666;
            font-size: 20px;
            margin-bottom: 40px;
            font-weight: 300;
        }

        .button-group {
            display: flex;
            gap: 15px;
            justify-content: center;
            flex-wrap: wrap;
            margin-bottom: 40px;
        }

        .button {
            padding: 14px 32px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 16px;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s;
            box-shadow: 0 5px 15px rgba(102, 126, 234, 0.3);
        }

        .button:hover {
            transform: translateY(-3px);
            box-shadow: 0 8px 25px rgba(102, 126, 234, 0.5);
        }

        .button:active {
            transform: translateY(-1px);
        }

        .info-card {
            background: #f8f9fa;
            border-left: 4px solid #667eea;
            padding: 20px;
            border-radius: 8px;
            text-align: left;
            margin-bottom: 20px;
        }

        .info-card h3 {
            color: #667eea;
            margin-bottom: 10px;
            font-size: 18px;
        }

        .info-card p {
            color: #555;
            margin: 8px 0;
            font-size: 14px;
        }

        .output {
            background: #f0f0f0;
            border: 2px solid #667eea;
            border-radius: 8px;
            padding: 20px;
            margin-top: 20px;
            min-height: 60px;
            display: flex;
            align-items: center;
            justify-content: center;
            color: #667eea;
            font-weight: bold;
            font-size: 16px;
        }

        .stat {
            display: inline-block;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 10px 20px;
            border-radius: 20px;
            margin: 5px;
            font-size: 14px;
        }

        footer {
            margin-top: 30px;
            padding-top: 20px;
            border-top: 1px solid #eee;
            color: #999;
            font-size: 12px;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="emoji">👋</div>
        <h1>Hello World!</h1>
        <div class="subtitle">Chào mừng đến BW16 Web Server</div>

        <div class="button-group">
            <button class="button" onclick="sayHello()">💬 Xin Chào</button>
            <button class="button" onclick="getTime()">⏰ Xem Giờ</button>
            <button class="button" onclick="getInfo()">ℹ️ Thông Tin</button>
        </div>

        <div class="output" id="output">
            👋 Bấm nút để tương tác
        </div>

        <div class="info-card">
            <h3>📊 Thông Tin Hệ Thống</h3>
            <p>🎯 Board: <strong>BW16 (RTL8710)</strong></p>
            <p>📡 Chế độ: <strong>Access Point (AP)</strong></p>
            <p>🌐 IP: <strong>192.168.4.1</strong></p>
            <p>🔐 Mật khẩu WiFi: <strong>12345678</strong></p>
            <p>📝 Ngôn ngữ: <strong>C++ (Arduino)</strong></p>
            <div style="margin-top: 15px;">
                <span class="stat">Requests: <span id="request-count">0</span></span>
            </div>
        </div>

        <footer>
            <p>BW16 Hello World Web Server v1.0 | 🚀 Đơn giản & Dễ học</p>
        </footer>
    </div>

    <script>
        let requestCount = 0;

        function sayHello() {
            const greetings = [
                '👋 Xin chào bạn!',
                '🌍 Hello World!',
                '🎉 Chào mừng đến BW16!',
                '⚡ Tuyệt vời!',
                '🚀 Amazing!'
            ];
            const random = greetings[Math.floor(Math.random() * greetings.length)];
            updateOutput(random);
            updateRequest();
        }

        function getTime() {
            const now = new Date();
            const time = now.toLocaleTimeString('vi-VN');
            const date = now.toLocaleDateString('vi-VN');
            updateOutput(`⏰ ${date}<br>🕐 ${time}`);
            updateRequest();
        }

        function getInfo() {
            updateOutput('🎯 Board: BW16<br>📡 IP: 192.168.4.1:80<br>🔐 WiFi: BW16_HelloWorld');
            updateRequest();
        }

        function updateOutput(text) {
            document.getElementById('output').innerHTML = text;
        }

        function updateRequest() {
            requestCount++;
            document.getElementById('request-count').textContent = requestCount;
        }

        // Auto display khi load page
        window.onload = () => {
            updateOutput('✅ Trang web đã tải thành công! 🎉');
        };
    </script>
</body>
</html>
)rawliteral";

  String response = makeResponse(200, "text/html");
  response += html;
  client.write(response.c_str());
  request_count++;
}

// Handler cho API - trả JSON
void handleApi(WiFiClient &client) {
  String json = "{";
  json += "\"status\":\"ok\",";
  json += "\"board\":\"BW16\",";
  json += "\"ssid\":\"BW16_HelloWorld\",";
  json += "\"ip\":\"192.168.4.1\",";
  json += "\"port\":80,";
  json += "\"requests\":" + String(request_count) + ",";
  json += "\"message\":\"Hello World from BW16!\"";
  json += "}";

  String response = makeResponse(200, "application/json");
  response += json;
  client.write(response.c_str());
  request_count++;
}

// Handler 404
void handle404(WiFiClient &client) {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>404 Not Found</title>
    <style>
        body {
            font-family: Arial;
            text-align: center;
            background: #f5f5f5;
            padding: 50px;
        }
        h1 { color: #e74c3c; }
        a { color: #667eea; text-decoration: none; }
    </style>
</head>
<body>
    <h1>404 - Không Tìm Thấy Trang</h1>
    <p>Trang này không tồn tại!</p>
    <a href="/">← Quay lại Trang Chủ</a>
</body>
</html>
)rawliteral";

  String response = makeResponse(404, "text/html");
  response += html;
  client.write(response.c_str());
}

// ==================== SETUP ====================

void setup() {
  DEBUG_SER_INIT();
  randomSeed(millis());

  // Config IP Address
  IPAddress local_ip(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(local_ip, gateway, subnet);

  // Start AP Mode
  WiFi.apbegin(ssid, pass, (char *) "1");

  // Start Web Server
  server.begin();

  DEBUG_SER_PRINT("\n");
  DEBUG_SER_PRINT("========================================\n");
  DEBUG_SER_PRINT("🚀 BW16 Hello World Web Server Started!\n");
  DEBUG_SER_PRINT("========================================\n");
  DEBUG_SER_PRINT("📡 SSID: ");
  DEBUG_SER_PRINT(ssid);
  DEBUG_SER_PRINT("\n");
  DEBUG_SER_PRINT("🔐 Password: ");
  DEBUG_SER_PRINT(pass);
  DEBUG_SER_PRINT("\n");
  DEBUG_SER_PRINT("🌐 IP Address: 192.168.4.1\n");
  DEBUG_SER_PRINT("🔗 Open: http://192.168.4.1\n");
  DEBUG_SER_PRINT("📡 API: http://192.168.4.1/api\n");
  DEBUG_SER_PRINT("========================================\n\n");

  delay(1000);
}

// ==================== LOOP ====================

void loop() {
  WiFiClient client = server.available();

  if (client.connected()) {
    String request = "";

    // Read request
    while (client.available()) {
      char c = client.read();
      request += c;
      delay(1);
    }

    // Parse path
    String path = parseRequest(request);

    DEBUG_SER_PRINT("Request: ");
    DEBUG_SER_PRINT(path);
    DEBUG_SER_PRINT(" [Count: ");
    DEBUG_SER_PRINT(String(request_count));
    DEBUG_SER_PRINT("]\n");

    // Route handler
    if (path == "/") {
      handleRoot(client);
    } else if (path == "/api") {
      handleApi(client);
    } else {
      handle404(client);
    }

    // Close connection
    client.stop();
  }

  delay(10);
}
