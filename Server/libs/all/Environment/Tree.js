var Environment = require('./Environment');
var Point = require('../Point');

function Tree() {
	Environment.call(this);
}

Tree._extends(Environment);
Tree.prototype.extend({
	isCollidable: true,
	type: 3 // Type: 0: powerup, 1: hatchery, 2: egg, 3: tree, 4: Rock , 5: bush
});

module.exports = Tree;
