/*
 * =========================================================
 * BW16 LED RGB CONTROL - HƯỚNG DẪN ĐẦY ĐỦ
 * =========================================================
 * Mạch BW16 có 3 LED RGB:
 * - LED_R (PA_15) - Đỏ
 * - LED_G (PA_16) - Xanh lá cây
 * - LED_B (PA_17) - Xanh dương
 * 
 * KIỂU LED: Common Anode
 * - LOW  (0V)  → LED SÁNG ✅
 * - HIGH (5V)  → LED TẮT  ❌
 * =========================================================
 */

#include "WiFi.h"
#include "WiFiServer.h"
#include "WiFiClient.h"

// ============================================================
// PHẦN 1: KHAI BÁO PIN LED
// ============================================================

// BW16 LED pins (RTL8720)
#define LED_R PA_15   // LED Đỏ
#define LED_G PA_16   // LED Xanh lá
#define LED_B PA_17   // LED Xanh dương

// ============================================================
// PHẦN 2: BIẾN TOÀN CỤC
// ============================================================

char *ssid = "BW16_LED";
char *pass = "12345678";

WiFiServer server(80);

// Lưu trạng thái LED (true = ON, false = OFF)
bool led_r_state = false;
bool led_g_state = false;
bool led_b_state = false;

// ============================================================
// PHẦN 3: HÀM ĐIỀU KHIỂN LED CƠ BẢN
// ============================================================

/*
 * QUAN TRỌNG: BW16 dùng COMMON ANODE
 * - digitalWrite(pin, LOW)  = LED SÁNG
 * - digitalWrite(pin, HIGH) = LED TẮT
 */

// Bật LED
void ledOn(int pin) {
  digitalWrite(pin, LOW);    // LOW = sáng
  Serial.print("LED ON: ");
  Serial.println(pin);
}

// Tắt LED
void ledOff(int pin) {
  digitalWrite(pin, HIGH);   // HIGH = tắt
  Serial.print("LED OFF: ");
  Serial.println(pin);
}

// Toggle (bật/tắt nhanh)
void ledToggle(int pin) {
  int current = digitalRead(pin);
  digitalWrite(pin, !current);
}

// ============================================================
// PHẦN 4: HÀM ĐIỀU KHIỂN NHÓM LED
// ============================================================

// Bật tất cả LED
void allLedOn() {
  ledOn(LED_R);
  ledOn(LED_G);
  ledOn(LED_B);
  led_r_state = true;
  led_g_state = true;
  led_b_state = true;
  Serial.println("All LEDs ON");
}

// Tắt tất cả LED
void allLedOff() {
  ledOff(LED_R);
  ledOff(LED_G);
  ledOff(LED_B);
  led_r_state = false;
  led_g_state = false;
  led_b_state = false;
  Serial.println("All LEDs OFF");
}

// Tạo màu Đỏ
void setRed() {
  ledOn(LED_R);
  ledOff(LED_G);
  ledOff(LED_B);
  led_r_state = true;
  led_g_state = false;
  led_b_state = false;
  Serial.println("Color: RED");
}

// Tạo màu Xanh lá
void setGreen() {
  ledOff(LED_R);
  ledOn(LED_G);
  ledOff(LED_B);
  led_r_state = false;
  led_g_state = true;
  led_b_state = false;
  Serial.println("Color: GREEN");
}

// Tạo màu Xanh dương
void setBlue() {
  ledOff(LED_R);
  ledOff(LED_G);
  ledOn(LED_B);
  led_r_state = false;
  led_g_state = false;
  led_b_state = true;
  Serial.println("Color: BLUE");
}

// Tạo màu Vàng (Đỏ + Xanh lá)
void setYellow() {
  ledOn(LED_R);
  ledOn(LED_G);
  ledOff(LED_B);
  led_r_state = true;
  led_g_state = true;
  led_b_state = false;
  Serial.println("Color: YELLOW");
}

// Tạo màu Tím (Đỏ + Xanh dương)
void setMagenta() {
  ledOn(LED_R);
  ledOff(LED_G);
  ledOn(LED_B);
  led_r_state = true;
  led_g_state = false;
  led_b_state = true;
  Serial.println("Color: MAGENTA");
}

// Tạo màu Cyan (Xanh lá + Xanh dương)
void setCyan() {
  ledOff(LED_R);
  ledOn(LED_G);
  ledOn(LED_B);
  led_r_state = false;
  led_g_state = true;
  led_b_state = true;
  Serial.println("Color: CYAN");
}

// Tạo màu Trắng (Tất cả LED)
void setWhite() {
  ledOn(LED_R);
  ledOn(LED_G);
  ledOn(LED_B);
  led_r_state = true;
  led_g_state = true;
  led_b_state = true;
  Serial.println("Color: WHITE");
}

// ============================================================
// PHẦN 5: HIỆU ỨNG LED
// ============================================================

// Nháy LED
void blink(int pin, int times, int delay_ms) {
  for (int i = 0; i < times; i++) {
    ledOn(pin);
    delay(delay_ms);
    ledOff(pin);
    delay(delay_ms);
  }
}

// Nháy tất cả LED
void blinkAll(int times, int delay_ms) {
  for (int i = 0; i < times; i++) {
    allLedOn();
    delay(delay_ms);
    allLedOff();
    delay(delay_ms);
  }
}

// Chuyển đổi màu liên tục
void colorCycle() {
  setRed();
  delay(500);
  setGreen();
  delay(500);
  setBlue();
  delay(500);
}

// ============================================================
// PHẦN 6: HÀM HTTP
// ============================================================

String parseRequest(String request) {
  int path_start = request.indexOf(' ') + 1;
  int path_end = request.indexOf(' ', path_start);
  return request.substring(path_start, path_end);
}

String parseQuery(String request, String key) {
  int start = request.indexOf(key + "=");
  if (start == -1) return "";
  
  start += key.length() + 1;
  int end = request.indexOf("&", start);
  if (end == -1) end = request.indexOf(" ", start);
  
  return request.substring(start, end);
}

String makeResponse(int code, String content_type) {
  String response = "HTTP/1.1 " + String(code) + " OK\r\n";
  response += "Content-Type: " + content_type + "; charset=UTF-8\r\n";
  response += "Connection: close\r\n\r\n";
  return response;
}

// ============================================================
// PHẦN 7: TRANG WEB ĐIỀU KHIỂN
// ============================================================

void handleRoot(WiFiClient &client) {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>BW16 LED RGB Control</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Segoe UI', Tahoma, Geneva, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
        }

        .container {
            background: rgba(255, 255, 255, 0.95);
            border-radius: 20px;
            padding: 40px;
            max-width: 700px;
            width: 100%;
            box-shadow: 0 20px 60px rgba(0, 0, 0, 0.3);
        }

        h1 {
            text-align: center;
            color: #333;
            margin-bottom: 10px;
            font-size: 40px;
        }

        .subtitle {
            text-align: center;
            color: #666;
            margin-bottom: 30px;
            font-size: 14px;
        }

        /* LED Cards */
        .led-grid {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            gap: 15px;
            margin-bottom: 30px;
        }

        .led-card {
            padding: 20px;
            border-radius: 15px;
            text-align: center;
            border: 2px solid #ddd;
            cursor: pointer;
            transition: all 0.3s;
            background: white;
        }

        .led-card:hover {
            transform: translateY(-5px);
            box-shadow: 0 10px 25px rgba(0, 0, 0, 0.1);
        }

        .led-card.red {
            border-color: #ff6b6b;
        }

        .led-card.red.active {
            background: #ff6b6b;
            color: white;
            box-shadow: 0 0 20px rgba(255, 107, 107, 0.5);
        }

        .led-card.green {
            border-color: #51cf66;
        }

        .led-card.green.active {
            background: #51cf66;
            color: white;
            box-shadow: 0 0 20px rgba(81, 207, 102, 0.5);
        }

        .led-card.blue {
            border-color: #4dabf7;
        }

        .led-card.blue.active {
            background: #4dabf7;
            color: white;
            box-shadow: 0 0 20px rgba(77, 171, 247, 0.5);
        }

        .led-circle {
            width: 60px;
            height: 60px;
            border-radius: 50%;
            margin: 0 auto 15px;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 24px;
        }

        .led-card.red .led-circle {
            background: #ff6b6b;
        }

        .led-card.green .led-circle {
            background: #51cf66;
        }

        .led-card.blue .led-circle {
            background: #4dabf7;
        }

        .led-card h3 {
            margin-bottom: 5px;
            color: #333;
            font-size: 18px;
        }

        .led-card.active h3 {
            color: white;
        }

        .led-card p {
            font-size: 12px;
            color: #999;
        }

        .led-card.active p {
            color: rgba(255, 255, 255, 0.8);
        }

        /* Control Section */
        .control-section {
            background: #f5f5f5;
            padding: 25px;
            border-radius: 15px;
            margin-bottom: 20px;
        }

        .control-section h2 {
            font-size: 18px;
            margin-bottom: 15px;
            color: #333;
        }

        .button-grid {
            display: grid;
            grid-template-columns: repeat(2, 1fr);
            gap: 10px;
        }

        button {
            padding: 12px 20px;
            border: none;
            border-radius: 8px;
            font-size: 14px;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }

        .btn-action {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
        }

        .btn-action:hover {
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(102, 126, 234, 0.4);
        }

        .btn-off {
            background: #e0e0e0;
            color: #333;
        }

        .btn-off:hover {
            background: #d0d0d0;
        }

        .status-box {
            background: #f0f9ff;
            border: 2px solid #667eea;
            border-radius: 10px;
            padding: 15px;
            text-align: center;
            color: #667eea;
            font-weight: 600;
            margin-bottom: 20px;
        }

        .divider {
            height: 1px;
            background: #ddd;
            margin: 20px 0;
        }

        @media (max-width: 600px) {
            .container {
                padding: 20px;
            }

            .led-grid {
                grid-template-columns: 1fr;
            }

            h1 {
                font-size: 32px;
            }

            .button-grid {
                grid-template-columns: 1fr;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>💡 LED RGB Control</h1>
        <div class="subtitle">BW16 3-LED Dashboard</div>

        <div class="status-box" id="status">
            ✅ All LEDs Off
        </div>

        <!-- LED Control -->
        <div class="led-grid">
            <div class="led-card red" onclick="toggleLED('red')">
                <div class="led-circle">🔴</div>
                <h3>Red</h3>
                <p id="status-red">OFF</p>
            </div>
            <div class="led-card green" onclick="toggleLED('green')">
                <div class="led-circle">🟢</div>
                <h3>Green</h3>
                <p id="status-green">OFF</p>
            </div>
            <div class="led-card blue" onclick="toggleLED('blue')">
                <div class="led-circle">🔵</div>
                <h3>Blue</h3>
                <p id="status-blue">OFF</p>
            </div>
        </div>

        <!-- Quick Actions -->
        <div class="control-section">
            <h2>⚡ Quick Actions</h2>
            <div class="button-grid">
                <button class="btn-action" onclick="sendCommand('all_on')">All ON</button>
                <button class="btn-off" onclick="sendCommand('all_off')">All OFF</button>
                <button class="btn-action" onclick="sendCommand('yellow')">Yellow</button>
                <button class="btn-action" onclick="sendCommand('cyan')">Cyan</button>
                <button class="btn-action" onclick="sendCommand('magenta')">Magenta</button>
                <button class="btn-action" onclick="sendCommand('white')">White</button>
            </div>
        </div>

        <div class="divider"></div>

        <!-- Manual Control -->
        <div class="control-section">
            <h2>🎮 Manual Control</h2>
            <div class="button-grid">
                <button class="btn-action" onclick="setLED('red', 'on')">Red ON</button>
                <button class="btn-off" onclick="setLED('red', 'off')">Red OFF</button>
                <button class="btn-action" onclick="setLED('green', 'on')">Green ON</button>
                <button class="btn-off" onclick="setLED('green', 'off')">Green OFF</button>
                <button class="btn-action" onclick="setLED('blue', 'on')">Blue ON</button>
                <button class="btn-off" onclick="setLED('blue', 'off')">Blue OFF</button>
            </div>
        </div>
    </div>

    <script>
        let ledState = { red: false, green: false, blue: false };

        function sendCommand(cmd) {
            fetch(`/?cmd=${cmd}`)
                .then(() => updateStatus())
                .catch(e => console.error('Error:', e));
        }

        function setLED(led, state) {
            ledState[led] = (state === 'on');
            fetch(`/?led=${led}&action=${state}`)
                .then(() => updateStatus())
                .catch(e => console.error('Error:', e));
        }

        function toggleLED(led) {
            const newState = !ledState[led] ? 'on' : 'off';
            setLED(led, newState);
        }

        function updateStatus() {
            document.querySelectorAll('.led-card.red')[0].classList.toggle('active', ledState.red);
            document.querySelectorAll('.led-card.green')[0].classList.toggle('active', ledState.green);
            document.querySelectorAll('.led-card.blue')[0].classList.toggle('active', ledState.blue);

            document.getElementById('status-red').textContent = ledState.red ? 'ON' : 'OFF';
            document.getElementById('status-green').textContent = ledState.green ? 'ON' : 'OFF';
            document.getElementById('status-blue').textContent = ledState.blue ? 'ON' : 'OFF';

            let statusText = '✅ All LEDs Off';
            if (ledState.red && ledState.green && ledState.blue) statusText = '🌈 All LEDs On';
            else if (ledState.red && ledState.green) statusText = '💛 Yellow';
            else if (ledState.green && ledState.blue) statusText = '💙 Cyan';
            else if (ledState.red && ledState.blue) statusText = '💜 Magenta';
            else if (ledState.red) statusText = '❤️ Red ON';
            else if (ledState.green) statusText = '💚 Green ON';
            else if (ledState.blue) statusText = '💙 Blue ON';

            document.getElementById('status').textContent = statusText;
        }
    </script>
</body>
</html>
  )rawliteral";

  String response = makeResponse(200, "text/html");
  response += html;
  client.write(response.c_str());
}

// ============================================================
// PHẦN 8: SETUP
// ============================================================

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  // Cấu hình pin LED là OUTPUT
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  
  // Tắt tất cả LED lúc khởi động
  allLedOff();
  
  Serial.println("\n========================================");
  Serial.println("🚀 BW16 LED RGB Control");
  Serial.println("========================================");
  Serial.println("LED Pins:");
  Serial.println("  LED_R (Đỏ):      PA_15");
  Serial.println("  LED_G (Xanh lá): PA_16");
  Serial.println("  LED_B (Xanh dương): PA_17");
  Serial.println("Type: Common Anode (LOW = ON, HIGH = OFF)");
  Serial.println("========================================");
  
  // WiFi
  IPAddress local_ip(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(local_ip, gateway, subnet);
  WiFi.apbegin(ssid, pass, (char *) "1");
  server.begin();
  
  Serial.println("WiFi: BW16_LED");
  Serial.println("URL: http://192.168.4.1");
  Serial.println("========================================\n");
}

// ============================================================
// PHẦN 9: LOOP
// ============================================================

void loop() {
  WiFiClient client = server.available();
  
  if (client.connected()) {
    String request = "";
    while (client.available()) {
      request += (char)client.read();
      delay(1);
    }

    Serial.println("Request: " + request.substring(0, 50));

    String path = parseRequest(request);
    
    // Xử lý điều khiển LED
    if (path.startsWith("/?")) {
      String cmd = parseQuery(request, "cmd");
      String led = parseQuery(request, "led");
      String action = parseQuery(request, "action");
      
      if (cmd == "all_on") allLedOn();
      else if (cmd == "all_off") allLedOff();
      else if (cmd == "red") setRed();
      else if (cmd == "green") setGreen();
      else if (cmd == "blue") setBlue();
      else if (cmd == "yellow") setYellow();
      else if (cmd == "cyan") setCyan();
      else if (cmd == "magenta") setMagenta();
      else if (cmd == "white") setWhite();
      else if (led == "red") {
        if (action == "on") { ledOn(LED_R); led_r_state = true; }
        else { ledOff(LED_R); led_r_state = false; }
      }
      else if (led == "green") {
        if (action == "on") { ledOn(LED_G); led_g_state = true; }
        else { ledOff(LED_G); led_g_state = false; }
      }
      else if (led == "blue") {
        if (action == "on") { ledOn(LED_B); led_b_state = true; }
        else { ledOff(LED_B); led_b_state = false; }
      }
    }
    
    // Gửi trang web
    handleRoot(client);
    client.stop();
  }
  
  delay(10);
}
