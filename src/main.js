var net = require('net'); // TCP
var util = require('util'); // logging
var Player = require('./Player');

// Conveniences
var puts = console.log;

// var NavalNGage = function ()
// {

// }

var handlers = {
	newconn: function (socket) {
		puts("Client connected!");

		socket.on('end', function () {
			puts("Client disconnected :(");
		});

		// socket.on('data', handlers.newdata);
		socket.on('data', function (data) {
			// Strip out trailing and leading whitespace
			message = data.toString('ascii').trim();

			puts(message);
			if (message == 'ready;')
			{
				var p = new Player('asd', socket);
				p.SendMessage('registered')
			}
		});
	},

	// newdata: function (data) {
	// 	// Strip out trailing and leading whitespace
	// 	message = data.toString('ascii').trim();

	// 	puts(message);
	// 	if (message == 'ready;')
	// 	{
	// 		var p = new Player('asd')
	// 	}
	// },
};

// var message_handlers = {
// 	"register":,
// 	"ready":,

// };


// Set up a TCP server socket and listen for incoming connections
var server = net.createServer(handlers.newconn);
// TODO: configure port
server.listen(5283, function () {
	puts("Awaiting client connections...");
});
