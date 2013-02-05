var Settings = require('./Settings');
function Debug() {
	Debug.prototype.log = function(level) {
		var args = Array.prototype.slice.call(arguments, 1);
		if(level <= Settings.DEBUGLEVEL) {
			for (var i = 0; i < args.length; ++i) {
				console.log('DEBUG LEVEL '+level+':', args[i]);
			}
		}
	};
}
module.exports = Debug;
