var Environment = require('../Environment');
var Point = require('../../Point');

function Powerup() {
	Environment.call(this);
}

Powerup._extends(Environment);
Powerup.prototype.extend({
	isCollidable: false,
	type: 0, // Type: 0: powerup, 1: hatchery, 2: egg, 3: tree, 4: Rock , 5: bush
	collision: function(gameObject) {
		if (gameObject.id == this.id) {
			return false;
		}
		var topLeft = new Point(
				this.position.x - this.width/2,
				this.position.y + this.height/2
			),
			bottomRight = new Point(
				this.position.x + this.width/2,
				this.position.y - this.height/2
			);
		if (gameObject.isStationary) {
			return gameObject.position.x + gameObject.width >= this.position.x
				&& gameObject.position.y + gameObject.height >= this.position.y
				&& gameObject.position.x <= this.position.x + this.width
				&& gameObject.position.y <= this.position.y + this.height;
		} else {
			var col = gameObject.position.inside(topLeft, bottomRight);
			if (col) {
				gameObject.pickUpPowerup(this)
			}
			return (this.isCollidable) ? col : false;
		}
	}
});

module.exports = Rock;
