var Environment = require('./Environment');
var Point = require('../Point');

function Rock() {
	Environment.call(this);
}

Rock._extends(Environment);
Rock.prototype.extend({
	isCollidable: true,
	type: 4 // Type: 0: powerup, 1: hatchery, 2: egg, 3: tree, 4: Rock , 5: bush
});

module.exports = Rock;
