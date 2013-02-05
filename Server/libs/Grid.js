var Debug = require('./Debug');
require('./misc');
var GridSection = require('./GridSection');
var Point = require('./all/Point');
var d = new Debug();


function Grid(width, height) {
	this.grid = [];
	this.rows = 0;
	this.columns = 0;
}

Grid.prototype.extend({
	init: function() {
		for (var i = 0; i < 3; ++i) {
			this.increase();
		}
	},
	increase: function() {
		var ret = [],
			lastRow = [],
			size = this.getSize();
			x = size.width,
			y = 0;
		for (var i = 0, r = this.rows; i < r; ++i) {
			var g = new GridSection(i, this.columns);
			g.x = x;
			g.y = y;
			y += g.height;
			this.grid[i].push(g);
			ret.push(g);
		}
		x = 0;
		y = size.height;
		for (var i = 0, c = this.columns; i <= c; ++i) {
			var g = new GridSection(this.rows, i);
			g.x = x;
			g.y = y;
			x += g.width;
			lastRow.push(g);
			ret.push(g);
		}
		this.grid.push(lastRow);

		this.surrounding(ret);

		++this.rows;
		++this.columns;
		return ret;
	},
	decrease: function() {
		var ret = [],
			lastRow = this.grid[this.rows - 1];
		for (var i = 0, r = this.rows - 1; i < r; ++i) {
			ret.push(this.grid[i].pop());
		}
		for (var i = 0, c = this.columns; i < c; ++i) {
			ret.push(lastRow.shift());
		}
		this.grid.pop();

		--this.rows;
		--this.columns;

		this.surrounding(ret);

		return ret;
	},
	surrounding: function(newGrids) {
		var grids = newGrids.slice(0);
		for (var i = 0, l = grids.length; i < l; ++i) {
			var grid = grids[i];
			if (grid.column == this.columns) {
				var g = this.getGrid(grid.row, grid.column - 1);
				if (g && grids.indexOf(g) == -1) {
					grids.push(g);
				}
			}
			if (grid.row == this.rows) {
				var g = this.getGrid(grid.row - 1, grid.column);
				if (g && grids.indexOf(g) == -1) {
					grids.push(g);
				}
			}
		}
		for (var i = 0, l = grids.length; i < l; ++i) {
			var grid = grids[i],
				gridRow = grid.row,
				gridColumn = grid.column,
				surrounding = [];
			for (var row = -1; row <= 1; ++row) {
				for (var column = -1; column <= 1; ++column) {
					if (row == 0 && column == 0) {
						//prevent circular structure
						continue;
					}
					var g = this.getGrid(gridRow + row, gridColumn + column);
					if (g) {
						surrounding.push(g);
					}
				}
			}
			grid.surrounding = surrounding;
		}
	},
	getGrid: function(gridOrRow, column) {
		if (gridOrRow instanceof GridSection) {
			return gridOrRow;
		} else if (gridOrRow instanceof Object) {
			return this.getGrid(gridOrRow.row, gridOrRow.column);
		}
		var row = this.grid[gridOrRow];
		if (row) {
			return row[column];
		} else {
			return undefined;
		}
	},

	addGameObjectToGrid: function(gobj, gridOrRow, column) {
		var grid = this.getGrid(gridOrRow, column);
		return grid.addGameObject(gobj);
	},

	removeGameObjectFromGrid: function(gobj, gridOrRow, column) {
		var grid = this.getGrid(gridOrRow, column);
		return grid.removeGameObject(gobj);
	},

	getGameObjectsInGrid: function(gridOrRow, column) {
		var grid = this.getGrid(gridOrRow, column);
		return grid.getGameObjects();
	},

	getBoundsOfGrid: function(gridOrRow, column) {
		var grid = this.getGrid(gridOrRow, column),
			bounds = grid.getBounds();
		return bounds;
	},

	positionInsideGrid: function(position, gridOrRow, column) {
		var grid = this.getGrid(gridOrRow, column),
			bounds = this.getBoundsOfGrid(grid),
			topLeft = new Point(
				bounds.x,
				bounds.y + bounds.height
			),
			bottomRight = new Point(
				bounds.x + bounds.width,
				bounds.y
			);
		return position.inside(topLeft, bottomRight);
	},

	getSize: function() {
		var size = {
			height: 0,
			width: 0
		};

		if (!this.grid) {
			return size;
		}
		for (var i = 0; i < this.rows; ++i) {
			size.height += this.getGrid(i, 0).height;
		}
		for (var i = 0; i < this.columns; ++i) {
			size.width += this.getGrid(0, i).width;
		}
		return size;
	}
});
module.exports = Grid;
