/*
 * ========================================================
 * BW16 WEB SERVER - GIẢI THÍCH CHI TIẾT TỪNG DÒNG CODE
 * ========================================================
 * 
 * Tổng quan:
 * - Tạo Web Server HTTP trên BW16
 * - Tạo WiFi Access Point (AP)
 * - Hiển thị trang web HTML + CSS + JavaScript
 * - Xử lý client requests
 * 
 * ========================================================
 */

// ============================================================
// DÒNG 1-3: KÉAR CÁC THƯ VIỆN CẦN THIẾT
// ============================================================

#include "WiFi.h"
/*
 * #include "WiFi.h"
 * 
 * Giải thích:
 * - #include : lệnh nhúng (kéo vào) thư viện
 * - "WiFi.h"  : thư viện WiFi của BW16
 * - Cấp các hàm:
 *   - WiFi.apbegin()  : tạo WiFi Access Point
 *   - WiFi.config()   : cấu hình IP
 * 
 * Tại sao cần:
 * - Để dùng WiFi trên BW16
 * - Không có thư viện này → không thể kết nối WiFi
 */

#include "WiFiServer.h"
/*
 * #include "WiFiServer.h"
 * 
 * Giải thích:
 * - Thư viện tạo Web Server
 * - Cấp lớp WiFiServer để:
 *   - Lắng nghe trên một port (port 80 cho HTTP)
 *   - Chờ client kết nối
 *   - Nhận requests từ client
 */

#include "WiFiClient.h"
/*
 * #include "WiFiClient.h"
 * 
 * Giải thích:
 * - Thư viện quản lý kết nối client
 * - Cấp lớp WiFiClient để:
 *   - Đọc dữ liệu từ client
 *   - Gửi dữ liệu về cho client
 */

// ============================================================
// DÒNG 5-6: BIẾN TOÀN CỤC - THÔNG TIN WIFI
// ============================================================

char *ssid = "BW16_HelloWorld";
/*
 * char *ssid = "BW16_HelloWorld";
 * 
 * Giải thích từng phần:
 * - char              : kiểu ký tự (character)
 * - *                 : con trỏ (pointer) - lấy địa chỉ trong bộ nhớ
 * - ssid              : tên biến
 * - "BW16_HelloWorld" : giá trị gán vào
 * 
 * SSID là gì?
 * - Service Set Identifier
 * - Tên WiFi mà các thiết bị khác sẽ thấy
 * - Ví dụ: WiFi nhà bạn hiển thị "TP-LINK-1234"
 * - Ở đây là "BW16_HelloWorld"
 * 
 * Khi bạn tìm WiFi trên điện thoại, sẽ thấy:
 * - BW16_HelloWorld ← đây là SSID
 */

char *pass = "12345678";
/*
 * char *pass = "12345678";
 * 
 * Giải thích:
 * - pass : biến lưu mật khẩu WiFi
 * - "12345678" : mật khẩu (phải nhập khi kết nối)
 * 
 * Khi kết nối vào WiFi "BW16_HelloWorld":
 * - Yêu cầu nhập password
 * - Nhập "12345678"
 * - Kết nối thành công
 */

// ============================================================
// DÒNG 8: TẠO WEB SERVER
// ============================================================

WiFiServer server(80);
/*
 * WiFiServer server(80);
 * 
 * Giải thích từng phần:
 * - WiFiServer : lớp (class) để tạo web server
 * - server     : tên biến/object
 * - (80)       : port HTTP (mặc định)
 * 
 * Port là gì?
 * - Như "cửa" trên máy tính
 * - Mỗi ứng dụng dùng port khác nhau
 * - Port 80  : dành cho HTTP (web thường)
 * - Port 443 : dành cho HTTPS (web bảo mật)
 * - Port 22  : dành cho SSH
 * - Port 3306 : dành cho MySQL database
 * 
 * Vậy server sẽ:
 * - Lắng nghe (listen) trên port 80
 * - Chờ client kết nối vào port 80
 * - Nhận HTTP requests
 * 
 * Ví dụ URL:
 * - http://192.168.4.1       (port 80 được tự động thêm)
 * - http://192.168.4.1:80    (port 80 viết rõ)
 * - http://192.168.4.1:3000  (port khác, không phải HTTP)
 */

// ============================================================
// DÒNG 10-14: HÀM TRÍCH ĐƯỜNG DẪN TỪ HTTP REQUEST
// ============================================================

String parseRequest(String request) {
/*
 * String parseRequest(String request) {
 * 
 * Giải thích:
 * - String  : kiểu dữ liệu (chuỗi ký tự)
 * - parseRequest : tên hàm
 * - (String request) : tham số đầu vào
 *   - String request : nhận vào 1 chuỗi tên là "request"
 * 
 * Parse = phân tích, trích xuất
 * 
 * Hàm này làm gì?
 * - Nhận vào 1 HTTP request
 * - Trích ra đường dẫn (path)
 * - Trả về đường dẫn
 */

  int path_start = request.indexOf(' ') + 1;
/*
 * int path_start = request.indexOf(' ') + 1;
 * 
 * Giải thích từng phần:
 * - int : kiểu số nguyên
 * - path_start : tên biến
 * - request.indexOf(' ') : tìm vị trí của khoảng trắng ' ' trong request
 * - + 1 : cộng thêm 1
 * 
 * Ví dụ HTTP request:
 * "GET / HTTP/1.1"
 * 0123456789...
 * 
 * request.indexOf(' ') → tìm khoảng trắng đầu ti��n
 * → vị trí 3 (sau "GET")
 * 
 * +1 → 3 + 1 = 4
 * → vị trí bắt đầu của đường dẫn (ký tự '/')
 * 
 * Kết quả: path_start = 4
 */

  int path_end = request.indexOf(' ', path_start);
/*
 * int path_end = request.indexOf(' ', path_start);
 * 
 * Giải thích:
 * - indexOf(' ', path_start) : tìm khoảng trắng tiếp theo
 *   - ' ' : ký tự tìm kiếm
 *   - path_start : bắt đầu tìm từ vị trí này
 * 
 * Ví dụ:
 * "GET / HTTP/1.1"
 * 0123456789...
 * 
 * path_start = 4
 * Tìm từ vị trí 4:
 * "/ HTTP/1.1"
 *  01234567...
 * 
 * indexOf(' ', 4) → tìm khoảng trắng từ vị trí 4
 * → vị trí 5 (khoảng trắng sau '/')
 * 
 * Kết quả: path_end = 5
 */

  return request.substring(path_start, path_end);
/*
 * return request.substring(path_start, path_end);
 * 
 * Giải thích:
 * - return : trả về kết quả
 * - substring(a, b) : cắt chuỗi từ vị trí a đến vị trí b (không lấy b)
 * 
 * Ví dụ:
 * "GET / HTTP/1.1".substring(4, 5)
 * 
 * Từ vị trí 4 đến 5:
 * "GET / HTTP/1.1"
 *      ^
 *      vị trí 4
 * 
 * Cắt 1 ký tự → "/"
 * 
 * Kết quả hàm: "/"
 * 
 * Các ví dụ khác:
 * "GET /about HTTP/1.1" → "/about"
 * "GET /api/users HTTP/1.1" → "/api/users"
 */
}

// ============================================================
// DÒNG 16-21: HÀM TẠO HTTP RESPONSE HEADER
// ============================================================

String makeResponse(int code, String content_type) {
/*
 * String makeResponse(int code, String content_type) {
 * 
 * Giải thích:
 * - makeResponse : tên hàm
 * - (int code, String content_type) : 2 tham số đầu vào
 *   - int code : mã lỗi HTTP (200, 404, 500)
 *   - String content_type : loại dữ liệu (text/html, application/json)
 * 
 * Hàm này tạo HTTP Response Header
 * - Header : thông tin về response
 * - Body : nội dung thực tế
 * 
 * Ví dụ:
 * makeResponse(200, "text/html")
 * → tạo header cho HTML thành công
 * 
 * makeResponse(404, "text/html")
 * → tạo header cho HTML lỗi không tìm thấy
 */

  String response = "HTTP/1.1 " + String(code) + " OK\r\n";
/*
 * String response = "HTTP/1.1 " + String(code) + " OK\r\n";
 * 
 * Giải thích:
 * - String response : biến lưu response header
 * - = : gán giá trị
 * - "HTTP/1.1 " : phiên bản HTTP
 * - + String(code) : chuyển số thành chuỗi và nối vào
 * - + " OK\r\n" : nối thêm " OK" và xuống dòng
 * 
 * String(code) là gì?
 * - Hàm chuyển đổi kiểu dữ liệu
 * - int 200 → String "200"
 * 
 * \r\n là gì?
 * - \r : return (quay về đầu dòng)
 * - \n : newline (xuống dòng mới)
 * - Cùng với: xuống dòng (CRLF - chuẩn HTTP)
 * 
 * Ví dụ:
 * code = 200 → "HTTP/1.1 200 OK\r\n"
 * code = 404 → "HTTP/1.1 404 OK\r\n"
 */

  response += "Content-Type: " + content_type + "; charset=UTF-8\r\n";
/*
 * response += "Content-Type: " + content_type + "; charset=UTF-8\r\n";
 * 
 * Giải thích:
 * - += : gán thêm (không ghi đè, mà thêm vào)
 * - Content-Type : loại dữ liệu gửi về
 * - content_type : biến truyền vào
 * - charset=UTF-8 : encoding ký tự (hỗ trợ tiếng Việt)
 * 
 * Ví dụ:
 * content_type = "text/html"
 * → "Content-Type: text/html; charset=UTF-8\r\n"
 * 
 * Các loại Content-Type phổ biến:
 * - "text/html" : trang web HTML
 * - "application/json" : dữ liệu JSON
 * - "text/plain" : text thô
 * - "image/png" : hình ảnh PNG
 */

  response += "Connection: close\r\n\r\n";
/*
 * response += "Connection: close\r\n\r\n";
 * 
 * Giải thích:
 * - Connection: close : đóng kết nối sau khi gửi xong
 * - \r\n\r\n : 2 lần CRLF = dòng trống
 *             (kết thúc HTTP header)
 * 
 * Tại sao \r\n\r\n?
 * - HTTP chuẩn: header kết thúc bằng dòng trống
 * - Dòng trống = dấu hiệu "header kết thúc, body bắt đầu"
 * 
 * HTTP Response cuối cùng:
 * "HTTP/1.1 200 OK\r\n
 *  Content-Type: text/html; charset=UTF-8\r\n
 *  Connection: close\r\n
 *  \r\n"
 * 
 * [Dòng trống - kết thúc header]
 * [Body HTML sẽ được thêm vào sau]
 */

  return response;
}

// ============================================================
// DÒNG 23-80: HÀM XỬ LÝ VÀ GỬI TRANG WEB
// ============================================================

void handleRoot(WiFiClient &client) {
/*
 * void handleRoot(WiFiClient &client) {
 * 
 * Giải thích:
 * - void : hàm không trả về kết quả
 * - handleRoot : tên hàm
 * - (WiFiClient &client) : tham số
 *   - WiFiClient : kiểu đối tượng client
 *   - & : tham chiếu (reference)
 *   - client : tên biến
 * 
 * & (tham chiếu) là gì?
 * - Không copy object, mà truyền địa chỉ
 * - Vậy hàm có thể sửa object gốc
 * - Tiết kiệm bộ nhớ (không copy)
 * 
 * handleRoot làm gì?
 * - Xử lý request đến "/"
 * - Gửi lại trang web HTML
 * - Gửi header HTTP
 * - Gửi nội dung HTML
 */

  String html = R"rawliteral(
/*
 * String html = R"rawliteral(
 *   ... HTML CODE ...
 * )rawliteral";
 * 
 * Giải thích:
 * - R"rawliteral( ... )rawliteral" : cách viết string nhiều dòng
 * - Raw literal = chuỗi thô (không cần escape ký tự đặc biệt)
 * 
 * Tại sao dùng R"rawliteral(...)"?
 * - HTML có nhiều ký tự đặc biệt: ", ', \, etc
 * - Nếu viết bình thường phải escape: \" \' \\
 * - R"rawliteral" cho phép viết bình thường, không cần escape
 * 
 * Cách cũ (khó):
 * String html = "<html>" +
 *               "<body>" +
 *               "<h1>Hello</h1>" +
 *               "</body>" +
 *               "</html>";
 * 
 * Cách mới (sạch):
 * String html = R"rawliteral(
 * <html>
 *   <body>
 *     <h1>Hello</h1>
 *   </body>
 * </html>
 * )rawliteral";
 */

/*
 * ========================================================
 * PHẦN HTML/CSS/JAVASCRIPT (Dòng 26-60)
 * ========================================================
 * 
 * Trang web gồm 3 phần:
 * 1. HTML : cấu trúc (tags, elements)
 * 2. CSS  : trang trí (màu, font, layout)
 * 3. JS   : tương tác (click, events)
 */

<!DOCTYPE html>
/*
 * <!DOCTYPE html>
 * 
 * Khai báo:
 * - Đây là HTML5 (phiên bản HTML mới nhất)
 * - Trình duyệt biết cách render page
 */

<html>
/*
 * <html>
 * - Thẻ gốc chứa tất cả nội dung
 */

<head>
/*
 * <head>
 * - Phần đầu trang (không hiển thị)
 * - Chứa metadata, CSS, title, etc
 */

    <meta charset="UTF-8">
/*
 * <meta charset="UTF-8">
 * 
 * Giải thích:
 * - meta : thẻ metadata (thông tin về page)
 * - charset="UTF-8" : encoding ký tự
 * - Cho phép hiển thị tiếng Việt đúng
 * - Không có dòng này → tiếng Việt hiển thị lỗi
 */

    <meta name="viewport" content="width=device-width, initial-scale=1.0">
/*
 * <meta name="viewport" ...>
 * 
 * Giải thích:
 * - viewport : cửa sổ hiển thị
 * - width=device-width : độ rộng = độ rộng thiết bị
 * - initial-scale=1.0 : mức zoom ban đầu = 100%
 * 
 * Tại sao cần?
 * - Để trang web responsive (hiển thị tốt trên mobile)
 * - Không có dòng này → trên mobile sẽ bị zoom out
 */

    <title>Hello World - BW16</title>
/*
 * <title>Hello World - BW16</title>
 * 
 * Giải thích:
 * - Tiêu đề trang (hiển thị trên tab trình duyệt)
 * - "Hello World - BW16" : text hiển thị
 */

    <style>
/*
 * <style> ... </style>
 * 
 * Giải thích:
 * - Phần CSS (trang trí, layout)
 * - Viết CSS inline (trực tiếp trong HTML)
 * 
 * CSS là gì?
 * - Cascading Style Sheets
 * - Dùng để trang trí giao diện
 * - Màu sắc, font, kích thước, vị trí, etc
 */

        body {
            font-family: Arial, sans-serif;
/*
 * font-family: Arial, sans-serif;
 * 
 * Giải thích:
 * - font-family : kiểu chữ
 * - Arial : chữ chính (Arial)
 * - sans-serif : chữ dự phòng (sans-serif = không chân)
 * 
 * Nếu browser không có Arial:
 * → dùng sans-serif chung
 */

            background: #667eea;
/*
 * background: #667eea;
 * 
 * Giải thích:
 * - background : màu nền
 * - #667eea : mã màu HEX (tím nhạt)
 * 
 * Mã HEX:
 * - #RRGGBB (Red Green Blue)
 * - #667eea = R:66 G:7e B:ea (thập lục phân)
 */

            display: flex;
/*
 * display: flex;
 * 
 * Giải thích:
 * - display: flex : dùng Flexbox layout
 * - Cho phép xếp các phần tử theo hàng/cột
 * - Dễ dàng canh giữa
 */

            justify-content: center;
/*
 * justify-content: center;
 * 
 * Giải thích:
 * - justify-content : canh content theo trục chính (ngang)
 * - center : canh giữa
 * 
 * Kết quả: các element sẽ canh giữa ngang
 */

            align-items: center;
/*
 * align-items: center;
 * 
 * Giải thích:
 * - align-items : canh items theo trục phụ (dọc)
 * - center : canh giữa
 * 
 * Kết quả: các element sẽ canh giữa dọc
 * 
 * display: flex + justify-content + align-items:
 * → Element sẽ ở chính giữa màn hình (ngang + dọc)
 */

            min-height: 100vh;
/*
 * min-height: 100vh;
 * 
 * Giải thích:
 * - min-height : chiều cao tối thiểu
 * - 100vh : 100% viewport height
 * - vh = viewport height (chiều cao cửa sổ)
 * 
 * Ví dụ:
 * - Màn hình 1000px cao
 * - 100vh = 1000px
 * - 50vh = 500px
 * 
 * Kết quả: body cao bằng chiều cao màn hình
 */

            margin: 0;
            padding: 20px;
/*
 * margin: 0; → không có khoảng cách bên ngoài
 * padding: 20px; → khoảng cách bên trong = 20px
 * 
 * Margin vs Padding:
 * - Margin : khoảng cách NGOÀI (với element khác)
 * - Padding : khoảng cách TRONG (với nội dung)
 * 
 *      [Margin]
 *    ┌─────────────┐
 *    │ [Padding]   │
 *    │ ┌─────────┐ │
 *    │ │ Content │ │
 *    │ └─────────┘ │
 *    └─────────────┘
 */
        }

        .container {
/*
 * .container { ... }
 * 
 * Giải thích:
 * - .container : class selector (dùng tên class)
 * - Áp dụng CSS cho element có class="container"
 */

            background: white;
            border-radius: 10px;
/*
 * border-radius: 10px;
 * 
 * Giải thích:
 * - border-radius : bán kính góc tròn
 * - 10px : góc tròn 10 pixel
 * 
 * Kết quả: box có góc tròn (không góc vuông)
 */

            padding: 40px;
            text-align: center;
/*
 * text-align: center;
 * 
 * Giải thích:
 * - text-align : canh lề text
 * - center : canh giữa
 * 
 * Kết quả: text bên trong sẽ ở giữa
 */

            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.2);
/*
 * box-shadow: 0 10px 30px rgba(0, 0, 0, 0.2);
 * 
 * Giải thích:
 * - box-shadow : bóng của box
 * - 0 : offset ngang (0 = không lệch ngang)
 * - 10px : offset dọc (10px lệch xuống)
 * - 30px : độ mờ (blur radius)
 * - rgba(0, 0, 0, 0.2) : màu đen, trong suốt 80%
 * 
 * Kết quả: box có bóng đổ xuống dưới
 */
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
/*
 * padding: 12px 25px;
 * 
 * Giải thích:
 * - 12px : padding trên/dưới
 * - 25px : padding trái/phải
 */

            background: #667eea;
            color: white;
            border: none;
/*
 * border: none;
 * 
 * Giải thích:
 * - border : đường viền
 * - none : không có viền
 * 
 * Mặc định button có viền → loại bỏ bằng none
 */

            border-radius: 5px;
            font-size: 16px;
            cursor: pointer;
/*
 * cursor: pointer;
 * 
 * Giải thích:
 * - cursor : con trỏ chuột
 * - pointer : hình bàn tay (chỉ ra có thể click)
 * 
 * Kết quả: khi di chuột qua button → con trỏ thành tay
 */

            margin: 10px;
        }

        .button:hover {
            background: #5568d3;
        }

/*
 * .button:hover {
 * 
 * Giải thích:
 * - :hover : pseudo-class (trạng thái hover)
 * - Áp dụng CSS khi chuột đi qua element
 * 
 * Kết quả:
 * - Bình thường: button màu #667eea
 * - Khi hover: button màu #5568d3 (tối hơn)
 */

        #output {
/*
 * #output {
 * 
 * Giải thích:
 * - #output : ID selector (dùng ID)
 * - Áp dụng CSS cho element có id="output"
 * 
 * . (class) vs # (ID):
 * - class : dùng cho nhiều element
 * - ID : dùng cho 1 element duy nhất
 */

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
/*
 * <div class="container">
 * 
 * Giải thích:
 * - <div> : thẻ container (chứa các element khác)
 * - class="container" : dùng class "container"
 * - Áp dụng CSS .container vào div này
 */

        <h1>👋 Hello World!</h1>
/*
 * <h1>...</h1>
 * 
 * Giải thích:
 * - h1 : heading 1 (tiêu đề mức 1)
 * - Chữ lớn nhất, quan trọng nhất
 * - h1 > h2 > h3 > ... > h6
 */

        <p>Chào mừng đến BW16 Web Server</p>
/*
 * <p>...</p>
 * 
 * Giải thích:
 * - p : paragraph (đoạn văn)
 */

        <button class="button" onclick="sayHello()">Xin Chào</button>
/*
 * <button class="button" onclick="sayHello()">
 * 
 * Giải thích:
 * - <button> : nút bấm
 * - class="button" : dùng style CSS từ class .button
 * - onclick="sayHello()" : khi click → chạy hàm sayHello()
 */

        <button class="button" onclick="getTime()">Xem Giờ</button>

        <div id="output">Bấm nút để tương tác</div>
/*
 * <div id="output">
 * 
 * Giải thích:
 * - id="output" : ID duy nhất
 * - JavaScript sẽ dùng ID này để thay đổi nội dung
 */
    </div>

    <script>
/*
 * <script> ... </script>
 * 
 * Giải thích:
 * - Phần JavaScript (code tương tác)
 * - Chạy trên trình duyệt (client)
 */

        function sayHello() {
/*
 * function sayHello() {
 * 
 * Giải thích:
 * - function : định nghĩa hàm
 * - sayHello : tên hàm
 * - () : không có tham số
 */

            document.getElementById('output').innerHTML = '👋 Xin chào bạn!';
/*
 * document.getElementById('output')
 * 
 * Giải thích:
 * - document : đối tượng toàn bộ trang web
 * - getElementById('output') : tìm element có id="output"
 * - .innerHTML : nội dung HTML bên trong
 * 
 * = '👋 Xin chào bạn!';
 * - Gán nội dung mới
 * - Thay thế cái cũ: "Bấm nút để tương tác"
 * 
 * Kết quả:
 * - Click nút "Xin Chào"
 * - Nội dung div#output thay đổi
 * - Hiển thị "👋 Xin chào bạn!"
 */
        }

        function getTime() {
            const now = new Date();
/*
 * const now = new Date();
 * 
 * Giải thích:
 * - const : constant (hằng số, không thay đổi)
 * - new Date() : tạo object Date (lấy thời gian hiện tại)
 */

            const time = now.toLocaleTimeString('vi-VN');
/*
 * now.toLocaleTimeString('vi-VN');
 * 
 * Giải thích:
 * - toLocaleTimeString() : chuyển đổi thành chuỗi giờ
 * - 'vi-VN' : định dạng tiếng Việt
 * 
 * Ví dụ:
 * - Mặc định: "2:30:45 PM" (tiếng Anh)
 * - 'vi-VN': "14:30:45" (tiếng Việt - 24h)
 */

            document.getElementById('output').innerHTML = '🕐 ' + time;
/*
 * document.getElementById('output').innerHTML = '🕐 ' + time;
 * 
 * Giải thích:
 * - '🕐 ' + time : nối chuỗi
 * - '🕐 ' : emoji + khoảng trắng
 * - time : biến chứa giờ
 * 
 * Ví dụ:
 * time = "14:30:45"
 * → '🕐 ' + "14:30:45" = "🕐 14:30:45"
 */
        }
    </script>
</body>
</html>
  )rawliteral";

  String response = makeResponse(200, "text/html");
/*
 * String response = makeResponse(200, "text/html");
 * 
 * Giải thích:
 * - Gọi hàm makeResponse()
 * - Tham số 1: 200 (mã lỗi - thành công)
 * - Tham số 2: "text/html" (loại dữ liệu)
 * - Lấy kết quả HTTP header
 * 
 * Kết quả:
 * "HTTP/1.1 200 OK\r\n
 *  Content-Type: text/html; charset=UTF-8\r\n
 *  Connection: close\r\n
 *  \r\n"
 */

  response += html;
/*
 * response += html;
 * 
 * Giải thích:
 * - += : gán thêm (nối chuỗi)
 * - response : đã có header
 * - + html : thêm nội dung HTML
 * 
 * Kết quả:
 * [HTTP Header]
 * [HTML content]
 */

  client.write(response.c_str());
/*
 * client.write(response.c_str());
 * 
 * Giải thích:
 * - client.write() : gửi dữ liệu về client
 * - response.c_str() : chuyển String sang C-string
 *   - C-string = const char* (kiểu cũ của C)
 *   - String trong Arduino là kiểu mới
 *   - write() yêu cầu const char*
 *   - c_str() = "convert to C string"
 * 
 * Kết quả:
 * - Toàn bộ response (header + HTML) gửi đi
 * - Client nhận được
 * - Trình duyệt hiển thị
 */
}

// ============================================================
// DÒNG 82-88: HÀM SETUP - CHẠY 1 LẦN LÚC KHỞI ĐỘNG
// ============================================================

void setup() {
/*
 * void setup() {
 * 
 * Giải thích:
 * - Hàm Arduino chạy 1 lần lúc khởi động
 * - Dùng để:
 *   - Cấu hình pin
 *   - Khởi tạo biến
 *   - Bắt đầu WiFi
 *   - Bắt đầu server
 */

  IPAddress local_ip(192, 168, 4, 1);
/*
 * IPAddress local_ip(192, 168, 4, 1);
 * 
 * Giải thích:
 * - IPAddress : kiểu dữ liệu địa chỉ IP
 * - local_ip : biến
 * - (192, 168, 4, 1) : địa chỉ IP
 *   - 192 : phần 1
 *   - 168 : phần 2
 *   - 4 : phần 3
 *   - 1 : phần 4
 * 
 * IP này:
 * - 192.168 : phạm vi mạng riêng tư
 * - 4 : phân chia của bạn
 * - 1 : thiết bị chính (BW16 board)
 * 
 * Khi kết nối vào BW16:
 * - Nhập http://192.168.4.1 vào trình duyệt
 * - Kết nối đến board BW16
 */

  IPAddress gateway(192, 168, 4, 1);
/*
 * IPAddress gateway(192, 168, 4, 1);
 * 
 * Giải thích:
 * - gateway : cổng mặc định
 * - Giống local_ip (trỏ đến board)
 */

  IPAddress subnet(255, 255, 255, 0);
/*
 * IPAddress subnet(255, 255, 255, 0);
 * 
 * Giải thích:
 * - subnet : mạng con
 * - 255.255.255.0 : mặc định
 * - Định nghĩa phạm vi IP có thể sử dụng
 * - 192.168.4.0 - 192.168.4.255 (256 địa chỉ)
 */

  WiFi.config(local_ip, gateway, subnet);
/*
 * WiFi.config(local_ip, gateway, subnet);
 * 
 * Giải thích:
 * - WiFi.config() : cấu hình WiFi tĩnh (fixed IP)
 * - Truyền 3 tham số: local_ip, gateway, subnet
 * 
 * Tại sao cấu hình tĩnh?
 * - Mặc định: IP tự động gán (DHCP)
 * - Cấu hình tĩnh: IP cố định (dễ kết nối)
 * - Luôn là 192.168.4.1 → dễ nhớ
 */

  WiFi.apbegin(ssid, pass, (char *) "1");
/*
 * WiFi.apbegin(ssid, pass, (char *) "1");
 * 
 * Giải thích:
 * - WiFi.apbegin() : bắt đầu WiFi Access Point
 * - ssid : tên WiFi ("BW16_HelloWorld")
 * - pass : mật khẩu ("12345678")
 * - (char *) "1" : channel (1 = 2.4GHz channel 1)
 * 
 * AP Mode là gì?
 * - Access Point = phát WiFi (như WiFi router)
 * - Thay vì kết nối vào WiFi có sẵn
 * - Board tạo 1 WiFi riêng
 * - Các thiết bị khác kết nối vào board
 * 
 * Kết quả:
 * - Board phát WiFi "BW16_HelloWorld"
 * - Các thiết bị có thể tìm thấy và kết nối
 */

  server.begin();
/*
 * server.begin();
 * 
 * Giải thích:
 * - server.begin() : bắt đầu server
 * - Lắng nghe trên port 80
 * - Chờ client kết nối
 */
}

// ============================================================
// DÒNG 90-110: HÀM LOOP - CHẠY LẶP LẠI VÔ HẠN
// ============================================================

void loop() {
/*
 * void loop() {
 * 
 * Giải thích:
 * - Hàm Arduino chạy lặp lại vô hạn
 * - setup() chạy 1 lần
 * - loop() chạy liên tục
 */

  WiFiClient client = server.available();
/*
 * WiFiClient client = server.available();
 * 
 * Giải thích:
 * - server.available() : kiểm tra có client kết nối không
 *   - Có : trả về object WiFiClient
 *   - Không : trả về null
 * - client = : lưu vào biến client
 * 
 * Ví dụ:
 * - Nếu có client → client là object có thể dùng
 * - Nếu không có → client là null (không có gì)
 */

  if (client.connected()) {
/*
 * if (client.connected()) {
 * 
 * Giải thích:
 * - if : câu lệnh điều kiện
 * - client.connected() : kiểm tra xem client có kết nối không
 *   - true : có kết nối
 *   - false : không có
 * 
 * Chỉ thực hiện code trong {} nếu có client
 */

    String request = "";
/*
 * String request = "";
 * 
 * Giải thích:
 * - Biến lưu trữ HTTP request từ client
 * - "" : chuỗi rỗng (chưa có dữ liệu)
 */

    while (client.available()) {
/*
 * while (client.available()) {
 * 
 * Giải thích:
 * - while : vòng lặp (lặp lại khi điều kiện đúng)
 * - client.available() : kiểm tra có dữ liệu để đọc không
 *   - Có : continue (tiếp tục vòng lặp)
 *   - Không : break (thoát vòng lặp)
 */

      request += (char)client.read();
/*
 * request += (char)client.read();
 * 
 * Giải thích:
 * - client.read() : đọc 1 byte (ký tự)
 * - (char) : chuyển byte sang ký tự
 * - request += : thêm vào chuỗi request
 * 
 * Ví dụ:
 * - Lần 1: request = "G"
 * - Lần 2: request = "GE"
 * - Lần 3: request = "GET"
 * - Lần 4: request = "GET "
 * - ...
 * - Cuối: request = "GET / HTTP/1.1"
 */

      delay(1);
/*
 * delay(1);
 * 
 * Giải thích:
 * - Đợi 1 millisecond
 * - Cho board xử lý, đọc dữ liệu tiếp theo
 * - Tránh đọc data bị mất
 */
    }

    String path = parseRequest(request);
/*
 * String path = parseRequest(request);
 * 
 * Giải thích:
 * - Gọi hàm parseRequest()
 * - Truyền biến request
 * - Lấy đường dẫn (path)
 * 
 * Ví dụ:
 * - request = "GET / HTTP/1.1"
 * - path = "/"
 * 
 * - request = "GET /about HTTP/1.1"
 * - path = "/about"
 */

    if (path == "/") {
/*
 * if (path == "/") {
 * 
 * Giải thích:
 * - == : so sánh (bằng)
 * - Nếu path = "/" → thực hiện code trong {}
 * - "/" là trang chủ
 */

      handleRoot(client);
/*
 * handleRoot(client);
 * 
 * Giải thích:
 * - Gọi hàm handleRoot()
 * - Truyền object client
 * - Hàm gửi trang web về cho client
 */

    } else {
/*
 * } else {
 * 
 * Giải thích:
 * - Nếu path != "/" (không phải trang chủ)
 * - Thực hiện code trong else block
 */

      String response = makeResponse(404, "text/html");
/*
 * String response = makeResponse(404, "text/html");
 * 
 * Giải thích:
 * - Gọi hàm makeResponse()
 * - 404 : mã lỗi (Not Found)
 * - "text/html" : loại dữ liệu
 * - Tạo HTTP header cho lỗi 404
 */

      response += "<h1>404 Not Found</h1>";
/*
 * response += "<h1>404 Not Found</h1>";
 * 
 * Giải thích:
 * - Thêm nội dung HTML vào response
 * - "<h1>404 Not Found</h1>" : tiêu đề lỗi
 */

      client.write(response.c_str());
/*
 * client.write(response.c_str());
 * 
 * Giải thích:
 * - Gửi error response về client
 * - client nhận được error page
 */
    }

    client.stop();
/*
 * client.stop();
 * 
 * Giải thích:
 * - Đóng kết nối với client
 * - Giải phóng tài nguyên (memory)
 * - Cho phép client khác kết nối
 */
  }

  delay(10);
/*
 * delay(10);
 * 
 * Giải thích:
 * - Đợi 10 millisecond
 * - Cho board "thở" 1 chút
 * - Nếu không có delay:
 *   - Board chạy quá nhanh
 *   - Không đủ thời gian đọc WiFi data
 *   - Có thể gây lỗi
 * - delay(10) giúp cân bằng
 */
}

/*
 * ========================================================
 * TÓM TẮT LUỒNG HOẠT ĐỘNG
 * ========================================================
 * 
 * 1. setup() chạy 1 lần:
 *    - Cấu hình IP
 *    - Tạo WiFi "BW16_HelloWorld"
 *    - Bắt đầu server lắng nghe port 80
 * 
 * 2. loop() chạy lặp lại:
 *    - Chờ client kết nối
 *    - Đọc HTTP request
 *    - Phân tích đường dẫn
 *    - Nếu "/" → gửi trang HTML
 *    - Nếu khác → gửi lỗi 404
 *    - Đóng kết nối
 *    - Lặp lại
 * 
 * 3. Client (trình duyệt):
 *    - Nhập http://192.168.4.1
 *    - Gửi request: "GET / HTTP/1.1"
 *    - Nhận response: HTML + CSS + JS
 *    - Hiển thị trang web
 * 
 * 4. Tương tác:
 *    - Click nút "Xin Chào" → JavaScript chạy
 *    - Thay đổi nội dung text (toàn client-side)
 *    - Không cần gửi request về server
 */
