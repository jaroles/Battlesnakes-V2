var Environment = require('./Environment');
var Point = require('../Point');

function Bush() {
	Environment.call(this);
}

Bush._extends(Environment);
Bush.prototype.extend({
	isCollidable: false,
	type: 5 // Type: 0: powerup, 1: hatchery, 2: egg, 3: tree, 4: Rock , 5: bush
});

module.exports = Bush;
