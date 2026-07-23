
#include "WiFiClient.h"
#include "WiFi.h"  
#include "WiFiServer.h"

char *ssid = "khuyen_voi_to"; // đổi tên wifi
char *pass = "123456789"; // đổi mật khẩu

WiFiServer server(80);

String parseRequest(String request) {
  int path_start = request.indexOf(' ') + 1;
  int path_end = request.indexOf(' ',path_start);
  return  request.substring(path_start,path_end);

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
    <title>BW16 Server</title>
</head>
<body>
    <h1>khuyến đẹp trai</h1>
</body>
</html>
)rawliteral";

  String response = makeResponse(200, "text/html");
  response += html;
  client.write(response.c_str());
}

          

void setup() {
  // put your setup code here, to run once:
  IPAddress local_ip(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(local_ip, gateway, subnet);
  WiFi.apbegin(ssid, pass, (char *) "1");
  server.begin();

}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    if (client.connected()) {
      String request = "";
      unsigned long timeout = millis();

      // Cho phép đợi tối đa 2000ms (2 giây) để nhận đủ Request
      while (client.connected() && millis() - timeout < 2000) {
        if (client.available()) {
          char c = client.read();
          request += c;

          // HTTP Request Header luôn kết thúc bằng một dòng trống (\r\n\r\n)
          if (request.endsWith("\r\n\r\n")) {
            break; // Đã nhận đủ phần Header, thoát vòng lặp ngay
          }
        }
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
  }
  delay(10);
}