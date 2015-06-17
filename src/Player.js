var Player = function (name, socket)
{
	var _id = new Date().valueOf(); // Should be random enough for our purposes
	var _name = name;
	var _ship = {};
	var _socket = socket;

	this.GetID = function () { return _id; }
	this.GetName = function () { return _name; }
	this.GetDirection = function () { return _dir; }
	this.GetShip = function () { return _ship; }

	this.SendMessage = function (name, args)
	{
		if (_socket && args instanceof Array && args.length > 0)
		{
			var message = name + ":";

			message = args.reduce(function (prev, cur) {
				return prev + cur + ";";
			});

			_socket.write(message + '\n', 'ascii');
		}
	};
};

module.exports = Player;