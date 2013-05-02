var Grid = require('./Grid');
var GridSection = require('./GridSection');
var Environment = require('./all/Environment/Environment');
var Hatchery = require('./all/Environment/Hatchery');
var Egg = require('./all/Environment/Egg');
var Bush = require('./all/Environment/Bush');
var Rock = require('./all/Environment/Rock');
var Tree = require('./all/Environment/Tree');
var GameObject = require('./all/GameObject');
var Snake = require('./all/Snake');
var MiniSnake = require('./all/MiniSnake');
var Teams = require('./all/Teams');
var Point = require('./all/Point');
var Vector = require('./all/Vector');
var Debug = require('./Debug');
var MiniSnakeAddon = require('./node/x64/Debug/MiniSnakeAddon');
var d = new Debug();
var dt = 0;

function World()
{
	var grid;
	var redHatchery;
	var blueHatchery;
	var a = 0;
	var storedTime = (new Date()).getTime();
	var scale = 25;
	var world = this;
	var once = 0;
	
	// MiniSnake variables
	var miniSnakeGrid;
	var miniSnakeController; // MiniSnake Grid Controller
	var miniSnakes = new Array();
	var miniSnakeID = 10000;

	function init()
	{
		grid = new Grid();
		grid.init();
		
		// Create the MiniSnake Controller
		miniSnakeGrid = new MiniSnakeAddon.Grid();
		miniSnakeController = new MiniSnakeAddon.GridController(miniSnakeGrid);

		InitEnvironment();
	};

	function InitEnvironment()
	{
		var bounds;
		// set up Hatcherys

		var g = grid.getGrid(0, 1);
		redHatchery = new Hatchery(Teams.Red);
		g.addGameObject(redHatchery);
		g.hasHatchery = Teams.Red;
		bounds = grid.getBoundsOfGrid(g);
		g.gameObjects[0].position.set(bounds.x + bounds.width/2, bounds.y + bounds.height/2);
		
		// Add Red Hatchery to MiniSnakes
		var redHatchPoint = new MiniSnakeAddon.Point(
				bounds.x + bounds.width/2, 
				bounds.y + bounds.height/2);
		var redMiniSnakeHatchery = new MiniSnakeAddon.Hatchery(redHatchPoint, 0);
		miniSnakeController.addObject(redMiniSnakeHatchery);

		g = grid.getGrid(2, 1);
		blueHatchery = new Hatchery(Teams.Blue);
		g.addGameObject(blueHatchery);
		g.hasHatchery = Teams.Blue;
		bounds = grid.getBoundsOfGrid(g);
		g.gameObjects[0].position.set(bounds.x + bounds.width/2, bounds.y + bounds.height/2);

		// Add Blue Hatchery to MiniSnakes
		var blueHatchPoint = new MiniSnakeAddon.Point(
				bounds.x + bounds.width/2, 
				bounds.y + bounds.height/2);
		var blueMiniSnakeHatchery = new MiniSnakeAddon.Hatchery(blueHatchPoint, 1);
		miniSnakeController.addObject(blueMiniSnakeHatchery);
		
		// Surround the world with rocks
		console.log("Surrounding world with rocks ...");
		SurroundWorld();

		// populate random environment
		console.log("Populating world with environment ...");
		PopulateEnvironment(grid.getGrid(0, 0));
		PopulateEnvironment(grid.getGrid(0, 2));
		PopulateEnvironment(grid.getGrid(1, 0));
		PopulateEnvironment(grid.getGrid(1, 1));
		PopulateEnvironment(grid.getGrid(1, 2));
		PopulateEnvironment(grid.getGrid(2, 0));
		PopulateEnvironment(grid.getGrid(2, 2));
	};

	function SurroundWorld() {
		function genRock(x, y, id) {
			var r = new Rock();
			r.id = id;
			r.position.set(x, y);
			return r;
		}
		var bottomRow = grid.rows - 1,
			rightColumn = grid.columns - 1,
			rock = new Rock(),
			rH = rock.height,
			rW = rock.width,
			rH2 = rH / 2,
			rW2 = rW / 2;
		// Top of world
		for (var i = 0, l = grid.columns; i < l; ++i) {
			var g = grid.getGrid(0, i),
				gX = grid.getBoundsOfGrid(g).x;
			for (var j = rW2 + gX, w = gX + g.width; j < w; j += rW) {
				var r = genRock(j, rH2, a++);
				g.addGameObject(r);
			}
		}
		// Bottom of world
		for (var i = 0, l = grid.columns; i < l; ++i) {
			var g = grid.getGrid(bottomRow, i),
				gB = grid.getBoundsOfGrid(g),
				gX = gB.x,
				y = gB.y + gB.width - rH2;
			for (var j = rW2 + gX, w = gX + g.width; j < w; j += rW) {
				var r = genRock(j, y, a++);
				g.addGameObject(r);
			}
		}
		// Left of world
		for (var i = 0, l = grid.columns; i < l; ++i) {
			var g = grid.getGrid(i, 0)
				gY = grid.getBoundsOfGrid(g).y;
			for (var j = rH2 + gY, h = gY + g.height; j < h; j += rH) {
				var r = genRock(rW2, j, a++);
				g.addGameObject(r);
			}
		}
		// Right of world
		for (var i = 0, l = grid.columns; i < l; ++i) {
			var g = grid.getGrid(i, rightColumn),
				gB = grid.getBoundsOfGrid(g),
				gY = gB.y,
				x = gB.x + gB.width - rW2;
			for (var j = rH2 + gY, h = gY + g.height; j < h; j += rH) {
				var r = genRock(x, j, a++);
				g.addGameObject(r);
			}
		}
	};

	function PopulateEnvironment(grid) {
		var objects = [Bush, Tree, Rock, Egg],
			num = objects.length,
			randomNumOfObjects = Math.floor((Math.random() * num));
		for (var i = 0; i < randomNumOfObjects; ++i) {
			/*
			 * 1 = Bush
			 * 2 = Tree
			 * 3 = Rock
			 * 4 = Egg
			 */
			var type = Math.floor((Math.random() * num)),
				obj = new objects[type];
			grid.addGameObject(obj);
			
			FindNewPosition(obj, type, grid)
		}
	};

	function FindNewPosition(obj, type, g) {
		var gridsObj = g.getGameObjects();
		var gb = grid.getBoundsOfGrid(g);
		var found = false;

		if (!obj.id) {
			//TODO: this will collide eventually
			obj.id = a++;
		}

		var found = false;
		var attempt = 0;
		while (!found) {
			//console.log('finding position attempt ' + attempt);
			if (attempt > 100) {
				// Couldn't find a spot in this grid.
				// Drop it.
				g.removeGameObject(obj);
				// Reset, and exit out of the loop;
				found = false;
				break;
			}
			found = true;
			var x = Math.floor(Math.random()*(gb.width - obj.width) + obj.width / 2);
			var y = Math.floor(Math.random()*(gb.height - obj.height) + obj.height / 2);
			x += gb.x;
			y += gb.y;
			obj.relocate(x, y);

			for(var i = 0, l = gridsObj.length; i < l; ++i) {
				var c = gridsObj[i].collision(obj)
				if(c || c === 0) {
					found = false;
					++attempt;
				}
			}
		}
		
		// Create an environment object for MiniSnakes
		if(found && typeof type == 'number') {
			var position = new MiniSnakeAddon.Point(x, y);
			var msObj = new MiniSnakeAddon.EnvironmentObject(type, position);
			miniSnakeController.addObject(msObj);
		}
	};

	function GetHatcheryGrid(team) {
		for (var i = 0; i < grid.rows; ++i) {
			for (var j = 0; j < grid.columns; ++j) {
				var g = grid.getGrid(i, j);
				if (g.hasHatchery == team) {
					return g;
				}
			}
		}
	}

	this.AddUser = function(user) {
		var size = this.GetCurrentSize(),
            snake = user.getSnake();
		// User function that must be aware of the World
		(function(world) {
			user.surroundingGridRooms = function() {
				return world.surroundingGridIds(snake);
			};
            user.reset = function (snake) {
                snake.grid.removeGameObject(snake);
                world.AddSnake(snake);
            };
            user.removeGameObject = function(gObj) {
                gObj.grid.removeGameObject(gObj);
            };
		})(this);
        this.AddSnake(snake);
		user.joinGridRoom(snake.grid.id)

		return size;
	};
    this.AddSnake = function (snake) {
		var g;
		switch(snake.team)
		{
			case Teams.Red:
				g = GetHatcheryGrid(Teams.Red);
				break;
			case Teams.Blue:
				g = GetHatcheryGrid(Teams.Blue);
				break;
		}
		g.addGameObject(snake);
		FindNewPosition(snake, 'snake', g);
    };

	this.GetCurrentSize = function() {
		return grid.getSize();
	};

	this.update = function(users) 
	{
		if(dt > 10000)
		{
			dt = 0;
			var curTime = (new Date()).getTime();
			var elapsedTime = (curTime - storedTime) / 1000;
		// if (elapsedTime !== 0) {
			storedTime = curTime;
		// }
			
			if(miniSnakes.length > 0) {
				miniSnakeController.update(); // Update MiniSnakes
			}

			for (var u = 0, U = users.length; u < U; ++u) 
			{
				var user = users[u],
					snake = user.getSnake(),
					velocity = snake.velocity.to,
					oldX = snake.position.x,
					oldY = snake.position.y,
					newX = oldX + (velocity.x * elapsedTime),
					newY = oldY + (velocity.y * elapsedTime),
					collision = false,
					OoB = false;
				
				if(miniSnakes.length > 0) {
					var miniSnakeData = this.getMiniSnakes();
					user.sendMiniSnakes(miniSnakeData);
					this.removeMiniSnakes();
				}
				
				if(user.request)
				{
					//console.log('user request: ', user.request);
					
					snake.move(newX, newY);
					snake.wiggle();
					snake.sprint(elapsedTime);
					var g = updateSnakeGrid(snake, velocity);
					if (g) {
						if (g != snake.grid) {
							changeGrid(snake, g, user);
						}
					} else {
						OoB = true;
					}
		
		            var colObj = undefined;
					if (!OoB) {
						var gObjs = snake.grid.getGameObjects();
						for (var i = 0, l = gObjs.length; i < l; ++i) 
						{
							var gObj = gObjs[i];
		                    if (!gObj) {continue;}
							collision = snake.collision(gObj);
							if(collision == 'egg' && gObj.type == 2)
							{
								collision = false;
								//console.log("ADDED NEW EGG");
								var newEgg = new Egg;
								gridSec = grid.getRandomGrid();
								gridSec.addGameObject(newEgg);
								FindNewPosition(newEgg, 'egg', gridSec);
							}
							
							// If collision with Hatchery, spawn MiniSnakes
							if(collision != undefined && collision.hasOwnProperty('type') && collision.type == 'hatch')
							{
								if(collision.eggs > 0)
									{console.log('Spawning ' + collision.eggs + ' minisnake(s)');}
								for(var i = 0; i < collision.eggs; i++)
								{
									 var miniSnake = this.createMiniSnake(collision.team);
									 miniSnakes.push(miniSnake);
									 miniSnakeController.addMiniSnake(miniSnake);
								}
								collision = true;
								colObj = gObj;
								break;
							}
							
							if (collision || collision === 0) {
								console.log(snake.id, collision);
		                        colObj = gObj;
								break;
							}
						}
					}
					
					if (typeof collision == 'number') {
						//console.log('collision == number')
						user.sendCollisionPacket(colObj);
						//user.broadcastPlayerUpdate();
					} else if (collision) {
						//console.log('collision == true')
						snake.move(oldX, oldY);
						snake.velocity.set(0, 0);
						user.sendCollisionPacket(colObj);
						//user.broadcastPlayerUpdate();
		            } else if (OoB) {
						//console.log('OoB ' + OoB);
						snake.move(oldX, oldY);
						snake.velocity.set(0, 0);
						//user.sendUpdatePacket();
						//user.broadcastPlayerUpdate();
					}
					
					if(!collision)
					{
						//console.log('no collision');
						user.request = true;
						user.sendUpdatePacket();
						user.broadcastPlayerUpdate();
					}
				}
			}
		}
		dt ++;

	};

	function updateSnakeGrid(snake, vector) {
		var g = snake.grid,
			row = g.row,
			column = g.column;

		if (grid.positionInsideGrid(snake.position, g)) {
			return g;
		}
		// console.log(g.id);
        for (var r = -1; r < 2; ++r) {
            for (var c = -1; c < 2; ++c) {
                g = grid.getGrid(row + r, column + c);
                if (g && grid.positionInsideGrid(snake.position, g)) {
                    return g;
                }
            }
        }
		return null;
	}

	function changeGrid(snake, newGrid, user) {
		var oldGrids = surroundingGrids(snake),
			newGrids = surroundingGrids(newGrid);

        snake.grid.removeGameObject(snake);
        newGrid.addGameObject(snake);
		user.leaveGridRoom(snake.grid.id);
		user.joinGridRoom(newGrid.id);

		var remove = oldGrids.complement(newGrids),
			add = newGrids.complement(oldGrids),
			removeEnvironment = world.surroundingEnvironment(remove),
			addEnvironment = world.surroundingEnvironment(add),
			removeSnake = world.surroundingSnakes(remove),
			addSnake = world.surroundingSnakes(add);

		user.sendRemoveEnvironmentPacket(removeEnvironment);
		user.sendAddEnvironmentPacket(addEnvironment);
		user.sendRemoveSnakePacket(removeSnake);
		user.sendAddSnakePacket(addSnake);
		
		// console.log('once: ' + once);
		if(once <= 3)
		{
			// user.broadcastRemoveSnake(remove);
			user.broadcastAddSnake(add);
		}
		
        once++;
		if(once == 15)
		{
			once = 0;
		}
	}

	function environment(gridsOrGameObject) {
		var env = [],
			grids;
		if (Array.isArray(gridsOrGameObject)) {
			grids = gridsOrGameObject;
		} else {
			grids = surroundingGrids(gridsOrGameObject);
		}
		for (var i = 0, l = grids.length; i < l; ++i) {
			if (grids[i] instanceof Object) {
				env = env.concat(grids[i].getGameObjects());
			}
		}
		return env;
	};

	function surroundingGrids(gridOrGameObject) {
		if (gridOrGameObject instanceof GameObject) {
			var g = gridOrGameObject.grid;
		} else {
			var g = gridOrGameObject;
		}
        if (!g) {return [];} // JUST BECAUSE I DON'T CARE ANYMORE
		var grids = g.surrounding.slice(0);
		grids.push(g);
		return grids;
	};

	this.surroundingGridIds = function(gridOrGameObject) {
		var grids = surroundingGrids(gridOrGameObject);

		var gids = grids.map(function(grid) {
			return grid.id;
		});
		return gids;
	};

	this.surroundingSnakes = function(gridsOrGameObject) {
		var env = environment(gridsOrGameObject).filter(function(obj) {
			return (obj instanceof Snake && obj != gridsOrGameObject);
		});
		return env;
	};

	this.surroundingEnvironment = function(gridsOrGameObject) {
		var env = environment(gridsOrGameObject).filter(function(obj) {
			return (obj instanceof Environment);
		});

		return env;
	};
	
	this.createMiniSnake = function(team) {
		var hatcheryPosition;
		var width;
		var height;
		
		if(team == Teams.Red) {
			hatcheryPosition = redHatchery.position;
			width = redHatchery.width;
			height = redHatchery.height;
		}
		else if (team == Teams.Blue) {
			hatcheryPosition = blueHatchery.position;
			width = blueHatchery.width;
			height = blueHatchery.height;
		}
		
		var xh = hatcheryPosition.x - width / 2;
		var yh = hatcheryPosition.y - height / 2;
		
		var x = (Math.random() * width) + xh;
		var y = (Math.random() * height) + xh - height;
		
		var position = new MiniSnakeAddon.Point(x, y);
		
		var miniSnake = new MiniSnakeAddon.MiniSnake(
				miniSnakeID++, 
				position, 
				team, 
				miniSnakeGrid);
					
		return miniSnake;
	};
	
	this.getMiniSnakes = function() {
		//var miniSnakes = miniSnakeController.getMiniSnakes();
		var miniSnakesArray = new Array();
		
		for(i = 0; i < miniSnakes.length; i++) {
			var miniSnake = miniSnakes[i];
			
			var position = miniSnake.getPosition();
			var posX = position[0];
			var posY = position[1];
			var positionPoint = new Point(posX, posY);
			
			var velocity = miniSnake.getVelocity();
			var toX = velocity[0];
			var toY = velocity[1];
			var velocityVector = new Vector(toX, toY);
			velocityVector.computeAngle();
			
			var miniSnakeObj = {
					id: miniSnake.getID(),
					team: miniSnake.getTeam(),
					position: positionPoint.toJSON(),
					velocity: velocityVector.toJSON(),
					state: miniSnake.getState()
			}
			
			miniSnakesArray.push(miniSnakeObj);
		}
		
		//console.log(miniSnakesArray);
		
		return miniSnakesArray;
	};
	
	this.removeMiniSnakes = function () {
		for(i = 0; i < miniSnakes.length; i++) {
			var miniSnake = miniSnakes[i];
			
			var state = miniSnake.getState();
			
			if(state == 1 || state == -1) {
				var id = miniSnake.getID();
				//miniSnakeController.removeMiniSnake(id);
				miniSnakes.splice(miniSnakes.indexOf(miniSnake), 1);
				miniSnakeController.removeMiniSnake(id);
			}
		}
	}

	init();
}

module.exports = World;
