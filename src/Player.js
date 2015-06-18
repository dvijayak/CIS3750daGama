// For convenience
var puts = console.log; // Only for debugging

var Player = function (socket)
{
	// The 'this' keyword can conflict depending on a function's context object
	var self = this;

	// Makes no sense to have a player without a successful client connection
	if (!socket) throw new Error('Cannot instantiate a player without a valid incoming client socket.');

	var _id = new Date().valueOf(); // Should be random enough for our purposes
	this.GetID = function () { return _id; }

	var _name = "Unnamed" + _id; // Temporary default
	this.GetName = function () { return _name; }

	var _ship = {};
	this.GetShip = function () { return _ship; }

	var _socket = socket;
	// No public accessor for the socket
	// Or should there be one?

	var _dir = 0;
	this.GetDirection = function () { return _dir; }

	// Is the player ready to begin the game?
	var _isReady = false;
	this.GetReadyStatus = function () { return _isReady; }

	// Client disconnection
	_socket.on('end', function () {
		console.log("Player %s has disconnected :(", _name);
	});

	// Upon receiving a message from the client, process it as per the specification
	_socket.on('data', function (data) {		
		var command = self.ReadMessage(data.toString('ascii').trim()); // Strip out trailing and leading whitespace
		puts(command);

		// Now, let's process each of the commands
		switch (command.name)
		{
			case 'register':
			{
				// Get the player's name, if provided
				_name = String(command.args[1] || _name);

				// Upon success, we can inform the client that the player has been
				// successfully registered, and has been issued a unique ID
				self.SendMessage('registered', _id);

				break;
			}
			case 'ready':
			{
				// The player is ready to start the game
				_isReady = true;

				// TODO: Inform all other players?

				break;
			}
			default:
			{
				// Unsupported command
				console.log("Unsupported command '%s'.", command.name);
				// TODO: Let client know?
				// self.SendMessage();
			}
		}
	});


	// Serialize the message as per specification format and
	// send it to the client.
	this.SendMessage = function (name, args)
	{
		if (!name) return console.error("Missing command name.");

		if (args)
		{
			if (!Array.isArray(args) && 
				(typeof args === 'object' || typeof args === 'function'))
			{
				console.error("args must be either an array or a string/number.");
				return;
			}
			else if (!Array.isArray(args))
			{
				args = [args];
			}
		}

		var message = name;
		if (!args || args.length == 0) // No args were passed
		{
			message += ";";
		}
		else
		{
			message += ":";
			// A Reduce function to concatenate the args based on the specification format
			// Note: the empty string '' must be passed as the initialValue for this to
			// work as expected.
			message += args.reduce(function (prev, cur) {
				return String(prev).concat(cur, ";");
			}, '');
		}

		// Trailing newline is required by specification
		message += '\n';

		puts("message: " + new Buffer(message, 'ascii'));

		_socket.write(message, 'ascii');
	};

	// Parse the raw message from the socket as per specification format
	// and extract the command and its parameters.
	this.ReadMessage = function (message)
	{
		if (!message) return undefined;

		var result = {};

		// Enforce the specification format
		// TODO: regex probably won't work. Just write a parser...later
		// var specFormat = /^\w+(:|;)\n$/; 

		// Tokenize the message and process it
		var tokens = message.split(/[:;]/);

		result = {
			name: tokens[0]
		};
		if (tokens.length > 1)
		{
			result.args = tokens.slice(1) // Return a new array from the second element until the last element
		}

		return result;
	}
};

module.exports = Player;