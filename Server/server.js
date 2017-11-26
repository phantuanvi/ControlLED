
var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io')(server);
var port = process.env.PORT || 3000;

server.listen(port, function () {
    console.log('Server listening at port %d', port);
});

let clientID;
let espID;

io.sockets.on('connection', function (socket) {

	clientID = socket.id
    console.log('New client connect: ', clientID);

    // cứ mỗi 2s yêu câu ESP cập nhật trạng thái của LED
    setInterval(function() {
    	socket.emit('server2esp-autoUpdateStatus');
    }, 2000);

    // server lắng nghe từ ESP và nhận biết ID của ESP, khi ESP kết nối tới server
    socket.on('ImESP8266', function(){
    	espID = socket.id;
    	console.log('esdID: ', espID);
    })

    // server lắng nghe từ ESP
    socket.on('esp2server-autoUpdateStatus', function(data){
    	console.log('esp2server-autoUpdateStatus: ',data);
    	// gửi tới cho client mới connect trạng thái của LED
    	socket.broadcast.emit('server2mobile-led-display', data);
    });

	// server lắng nghe từ ESP
	socket.on('esp2server-led-status', function(data) {
		console.log("esp2server: ", data);
		// server phát cho mọi client (ngoại trừ ESP)
		socket.broadcast.emit('server2mobile-led-display', data);
	});

	// server lắng nghe từ iOS
	socket.on('mobile2server-control-led', function(data) {
		console.log("mobile2server: ", data);
		// server phát để điểu khiển ESP
		let json = { 'led': data }
		io.sockets.to(espID).emit('server2esp-change-led', json)
	})


    socket.on('disconnect', function() {
        console.log('Client disconnect');
    });
});
