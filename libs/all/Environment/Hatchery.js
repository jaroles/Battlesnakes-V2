var Environment = require('./Environment');
var Point = require('../Point');

function Hatchery(team) {
	Environment.call(this);
	this.color = team;
	var playersEggs = [];
}

Hatchery._extends(Environment);
Hatchery.prototype.extend({
	isCollidable: true,
	type: 1, // Type: 0: powerup, 1: hatchery, 2: egg, 3: tree, 4: Rock , 5: bush
	width: 48,
	height: 48,

	collision: function(gameObject) {
		if (gameObject.id == this.id) {
			return;
		}
		var topLeft = new Point(
				this.position.x - this.width/2,
				this.position.y + this.height/2
			),
			bottomRight = new Point(
				this.position.x + this.width/2,
				this.position.y - this.height/2
			);
		var eggNums = 0;
		if (gameObject.isStationary) {
			return gameObject.position.x + gameObject.width >= this.position.x
				&& gameObject.position.y + gameObject.height >= this.position.y
				&& gameObject.position.x <= this.position.x + this.width
				&& gameObject.position.y <= this.position.y + this.height;
		} else {
			var col = gameObject.position.inside(topLeft, bottomRight);
			if (col && gameObject.hasEggs()) {
				eggNums = gameObject.eggs.length;
				gameObject.dropOffEggs(this);
			}
			var hatch = {
					type:'hatch',
					eggs:eggNums,
					team:this.color
			};
			var toReturn = (col) ? hatch : false;
			return toReturn;
		}
	}

});

module.exports = Hatchery;
