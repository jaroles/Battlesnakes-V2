var HOST = 'localhost',
	PORT = '8000',
	MONGO = 'mongodb://127.0.0.1/data/db';

/*
 * 0 = No Debug
 * 1 = Minimal Debug
 * 2 = Normal Debug
 * 3 = Verbose Debug (This shows every packet)
 */
var DEBUG = 3;


/*
 * DONT TOUCH
 */
module.exports.HOST = HOST;
module.exports.PORT = PORT;
module.exports.MONGO = MONGO;
module.exports.DEBUGLEVEL = DEBUG;
