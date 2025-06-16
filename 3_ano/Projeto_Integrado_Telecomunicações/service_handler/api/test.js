


var net = require('net');

var client = new net.Socket();
client.connect(6666, '127.0.0.1', function() {
	console.log('Connected');
	client.write('Hello, server! Love, Client.');
});

client.on('data', function(data) {
	console.log('Received: ' + data);
	client.destroy(); // kill client after server's response
});

client.on('close', function() {
	console.log('Connection closed');
});


 



// var dict = {
// 	0: {nome:"sam"},
// 	1: {nome:"boss"}
// }

// var arr = []

// for (const [key, value] of Object.entries(dict)) {
// 	arr.push(value)
//   }

// console.log(arr)