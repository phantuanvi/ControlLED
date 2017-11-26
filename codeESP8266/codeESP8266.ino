#include <ArduinoJson.h>
#include <SocketIoClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

// tên wifi và password nơi mà ESP cài đặt (ví dụ: nhà bạn)
const char* ssid = "vizi";
const char* password = "0000coco";

// host và post của SocketIO Server
//char host[] = "192.168.1.2";
//int port = 3000;
char domain[] = "esp2led.herokuapp.com";

// Khởi tạo socket
SocketIoClient socket;

ESP8266WiFiMulti WiFiMulti;
// trên module ESP8266, chân của LED là 16
const int LED = 16;
bool statusOfLED;

// Tạo JSON file để gửi đến SocketIO server
char* createJSON(bool verify) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();
  root["led"] = verify;
  char JSONmessage[200];
  root.printTo(JSONmessage, sizeof(JSONmessage));
  Serial.print("JSONmessage: ");
  Serial.println(JSONmessage);
  return JSONmessage;
}

// Xử lý khi nhận được JSON từ SocketIO server
void event(const char * payLoad, size_t length) {
  Serial.printf("got message from SocketIO server: %s\n", payLoad);

  // parse json
  StaticJsonBuffer<80> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(payLoad);
  if (!root.success()){
    Serial.print("parseObject(");
    Serial.print(payLoad);
    Serial.println(") failed");
  }

  // Xử lý tín hiệu điều khiển từ SocketIO server
  String LED_STATUS = root["led"];

  if (LED_STATUS == "false") {
      digitalWrite(LED, HIGH);
      // đọc trạng thái chân 16 của LED để gửi đến SocketIO server
      statusOfLED = (digitalRead(LED)==1) ? false : true;
      char* JSON = createJSON(statusOfLED);
      socket.emit("esp2server-led-status", JSON);
  } else {
      digitalWrite(LED, LOW);
      statusOfLED = (digitalRead(LED)==1) ? false : true;
      char* JSON = createJSON(statusOfLED);
      socket.emit("esp2server-led-status", JSON);
  }
}

void setupNetwork() {
    //Kết nối vào mạng Wifi
    WiFiMulti.addAP(ssid, password);
 
    //WiFi.disconnect();
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(500);
        Serial.print('.');
    }
    
    Serial.println("Wifi connected!");
    Serial.println(F("Di chi IP cua ESP8266 (Socket Client ESP8266): "));
    Serial.println(WiFi.localIP());
    
    // Kết nối đến SocketIO server
    socket.begin(domain);
//    socket.begin(host, port);
}

// update trạng thái của LED khi SocketIO server yêu cầu
void updateStatusOfLED(const char * payLoad, size_t length) {
    Serial.println("updateStatusOfLED is called");
    statusOfLED = (digitalRead(LED)==1) ? false : true;
    char* JSON = createJSON(statusOfLED);
    socket.emit("esp2server-autoUpdateStatus", JSON);
}

void setup() {

    // Cài đặt chân LED là chân đầu ra tín hiệu
    pinMode(LED, OUTPUT);
    
    // Bắt đầu kết nối serial với tốc độ baud là 115200.
    // Khi mở serial monitor thì phải set đúng tốc độ baud.
    Serial.begin(115200);
    setupNetwork();

    // Khi kết nối đến SocketIO, báo danh và trạng thái của LED
    statusOfLED = (digitalRead(LED)==1) ? false : true;
    char* JSON = createJSON(statusOfLED);
    socket.emit("ImESP8266", JSON);
    
    socket.on("server2esp-autoUpdateStatus", updateStatusOfLED);
    socket.on("server2esp-change-led", event);
}

void loop() {
    socket.loop();
}
