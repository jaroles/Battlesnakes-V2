var Powerup = require('./Powerup');

function Sprint() {
	Powerup.call(this);
}

Sprint._extends(Powerup);
Sprint.prototype.extend({
	powerupType: 1 // Type: 0: powerup, 1: hatchery, 2: egg, 3: tree, 4: Rock , 5: bush
});

module.exports = Rock;
