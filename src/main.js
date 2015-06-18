// YOOO JAMMM

var net = require('net'); // TCP
var util = require('util'); // logging
var Player = require('./Player');

// Conveniences
var puts = console.log;

// App begins

var allPlayers = {};

function serveNewConnection (socket)
{
	puts("Client connected!");

	var newPlayer = new Player('asd', socket);

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
}

// Set up a TCP server socket and listen for incoming connections
var server = net.createServer(serveNewConnection);
// TODO: configure port
server.listen(5283, function () {
	puts("Awaiting client connections...");
});
