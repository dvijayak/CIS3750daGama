var net = require('net'); // TCP
var util = require('util'); // logging
var Player = require('./Player');

// Conveniences
var puts = console.log;

// App begins

var allPlayers = {};

function serveNewConnection (socket)
{
	puts("A new client has connected...");

	var newPlayer = new Player(socket);
	allPlayers[newPlayer.GetID()] = newPlayer;
}

// Set up a TCP server socket and listen for incoming connections
var server = net.createServer(serveNewConnection);
// TODO: configure port
server.listen(5283, function () {
	puts("Awaiting client connections...");
});
