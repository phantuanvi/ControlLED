# ControlLED

### Cách thức hoạt động
1. Server nhận tín hiệu điều khiển từ website hoặc mobile app.
2. Server gửi tin nhắn điều khiển đến ESP8266.
3. ESP8266 thực hiện lệnh điều khiển LED.
4. ESP8266 cập nhật trạng thái của LED lên server.
5. Server nhận phản hồi và cập nhật trạng thái của LED cho client (gồm website và mobile app).
6. Khi ESP có vấn đề (mất điện, tái kết nối) thì server sẽ gửi tin nhắn đến client (gồm website và mobile app).
7. Client (gồm website và mobile app) thực hiện điều khiển và hiểu thị trạng thái của LED đến User.

### Công nghệ sử dụng
#### Về Server

- Server sử dụng nền tảng fullstack MEAN + SocketIO. [Xem thêm ở đây](https://github.com/angular-fullstack/generator-angular-fullstack)
- Server được đặt tại [Heroku](https://heroku.com/).
- Database lưu tại [mLab](https://mlab.com/).

#### Về Client

- User có thể điều khiển trên website [click vào đây](http://esp2led.herokuapp.com/control) (cần đăng nhập).
- Hoặc sử dụng ứng dụng di động trên nền tảng iOS để điều khiển và giám sát thiết bị.
<br>

<p float="left">
<img src="https://github.com/phantuanvi/ControlLED/blob/master/images/ios1.png" width="200"/>
<img src="https://github.com/phantuanvi/ControlLED/blob/master/images/ios2.png" width="200"/>
<img src="https://github.com/phantuanvi/ControlLED/blob/master/images/ios3.png" width="200"/>
</p>

#### Về phần cứng

- Sử dụng IDE của [Arduino](https://www.arduino.cc/) và [Library này](https://github.com/timum-viw/socket.io-client) để lập trình cho [KIT ESP8266](https://www.google.com.vn/search?q=ESP8266+ESP-12E+NodeMCU&oq=ESP8266+ESP-12E+NodeMCU&aqs=chrome..69i57j69i60l2.157442j0j7&sourceid=chrome&ie=UTF-8).
- Sử dụng KIT ESP8266 kết nối với WiFi nhà bạn.
- Project này chỉ mới sử dụng 1 chân của KIT ESP8266 để điều khiển LED có sẵn trên board.
- Để điều khiển thiết bị điện trong nhà thì chỉ cần thêm module rơle nữa là OK.
- Còn rất nhiều chân trống khác bạn có thể sử dụng hoặc giao tiếp với board khác để mở rộng chức năng.

<img src="https://community.blynk.cc/uploads/default/original/2X/9/9157a5791bf5f3ca604cdd4206cf658f34a50b3b.jpg" width="400"/>


