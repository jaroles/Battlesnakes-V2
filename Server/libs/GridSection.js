var Debug = require('./Debug');
var d = new Debug();


function GridSection(row, column, width, height) {
	this.gameObjects = [];
	this.hasHatchery = -1; // 0 for red 1 for blue -1 for no
	this.width = width || 96;
	this.height = height || 96;
	this.x = 0;
	this.y = 0;
	this.row = (row != undefined) ? row : 0;
	this.column = (column != undefined) ? column : 0;
	this.surrounding = [];
}

GridSection.prototype.extend({
	addGameObject: function(gobj) {
        gobj.grid = this;
		this.gameObjects.push(gobj);
	},

	removeGameObject: function(gobj) {
        return this.gameObjects.remove(gobj);
	},

	getGameObjects: function() {
		return this.gameObjects;
	},

	getBounds: function() {
		return {
			x: this.x,
			y: this.y,
			height: this.height,
			width: this.width
		};
	},

	get id() {
		return '' + this.row + 'x' + this.column;
	},

	valueOf: function() {
		return {
			hasHatchery: this.hasHatchery,
			width: this.width,
			height: this.height,
			row: this.row,
			column: this.column
		};
	}
});
module.exports = GridSection;
