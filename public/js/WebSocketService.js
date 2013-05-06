/*
* @author: Ryan Howard
* @author: Andrew Wagenheim
* Software Development II
* Battle Snakes
*/

/**
* Initializes the WebSocketService
* @param webSocket The web socket used for server communication
* @param game The current game communicating with the server
*/

var WebSocketService = function(webSocket,game) 
{
	var webSocketService = this;
	var webSocket = webSocket;
	window.webSocket = webSocket;
	this.hasConnection = false;
	this.game = game;
	var powerup = 0;
	var hatchery = 1;
	var egg = 2;
	var tree = 3;
	var rock = 4;
	var bush =  5;
	var checkOtherSegments = new Array();
	var temp = new Array();
	/**
	* Handles the intro packet
	* @param data The data contained within the intro packet
	*/
	this.introHandler = function(data) 
	{
		//console.log(data);
		
		//console.log(data);
		
		var canvas = document.getElementById('canvas');
		var width = canvas.width;
		var height = canvas.height;
		this.centerX = width/2;
		this.centerY = height/2;
		webSocketService.hasConnection = true;
		$('#chat').initChat();
		var worldPos = 
			{ 
				x: data.position.x / 1000,
			    y: data.position.y / 1000
			};
		//var worldPos = data.position;
		var point = new paper.Point(this.centerX,this.centerY); 
		
	
		var segments = new Array();
		
		for(var i = 0; i < data.segments.length; i++)
		{
			var tempSegments = 
				{
					from: 
					{
						x: data.segments[0].from.x / 1000,
						y: data.segments[0].from.y / 1000,
					},
					control1: 
					{
						x: data.segments[0].control1.x / 1000,
						y: data.segments[0].control1.y / 1000,
					},
					control2: 
					{
						x: data.segments[0].control2.x / 1000,
						y: data.segments[0].control2.y / 1000,
					},
					to: 
					{
						x: data.segments[0].to.x / 1000,
						y: data.segments[0].to.y / 1000,
					}
				}
			
			segments[i] = tempSegments;
		}

		//console.log(data.segments[0].from);
		//console.log(segments);
		
		var velocity = data.velocity.magnitude;

		var angle = data.velocity.angle;

		var color = data.team == 0 ? 'ff0000':'0000ff';
		
		//console.log(data.name,data.team,color,velocity,angle,-1,segments.length,segments,data.id,worldPos,this.game.scaleWindow,{x:this.centerX,y:this.centerY},true);
		var snake = new Snake(data.name,data.team,color,velocity,angle,-1,segments.length,segments,data.id,worldPos,this.game.scaleWindow,{x:this.centerX,y:this.centerY},true);
		
		this.game.start(snake);	
	};
	
	/**
	* Updates the snake based on client info, essentially checking the two are the same
	* @param data The server data passed into the client regarding the player's snake
	*/
	this.updateHandler = function(data) 
	{
		/*
			we may have to redo this
		*/
		//console.log(data);
		
		// convert the packet integers to floats
		var x = data.position.x / 1000;
		var y = data.position.y / 1000;
		
		/*var x = data.position.x,
		    y = data.position.y;*/
		
		var dx = x-this.game.userSnake.worldPos.x;
		var dy = y-this.game.userSnake.worldPos.y;
		
		this.game.userSnake.worldPos.x = x;
		this.game.userSnake.worldPos.y = y;
		//this.game.userSnake.update(dx, dy);
		
		var angle = data.velocity.angle*(Math.PI/180);	
		var oldAngle = this.game.userSnake.angle;
		this.game.userSnake.angle = angle;
		// this.game.userSnake.rotate((180/Math.PI)*(angle-oldAngle));
		this.game.userSnake.velocity = data.velocity.magnitude;
	};
	
	/**
	* Stores the text entered into the chat
	* @param data The chat data entered into the client
	*/
	this.chatHandler = function(data)
	{
		//this.game.addMessage(data.message);
	};
	
	/**
	* Stores data of eggs
	* @param data The eggs to be drawn in the environment
	*/
	this.eggHandler = function(data) 
	{	
		console.log(data);
		
		if (data.eggs[0].length)
		{
			// User collision with Egg
			//console.log("EggHandler: adding ", data.eggs[0].length);
			
			this.game.userSnake.numSegments = 2+data.eggs.length;
		}
		else
		{
			// User collision with Hatchery
			//console.log("EggHandler: Else");
			
			// TODO
			// Segments initially long and incorrect after returning to hatchery
			this.game.userSnake.body.removeSegments();
			this.game.userSnake.body.add(new paper.Point(this.game.userSnake.x,this.game.userSnake.y));
			this.game.userSnake.numSegments = 2;
		}
		
		this.game.userSnake.init(); 
	};
	
	this.closedHandler = function(data) 
	{
		//console.log(data);
	};
	
	/**
	* Stores data of environment objects
	* @param data The various environment objects and their coordinates
	*/
	this.addEnvironmentHandler = function(data)
	{		
		//console.log(data);
		var items = data.items;
		var environment = new Array();
		var x,y;
		for (var i = 0;i< items.length;i++)
		{
			// convert the integer positions to floats
			var x = items[i].position.x / 1000;
			var y = items[i].position.y / 1000;
			
			if (items[i].type == rock)
			{
				environment[i] = new Rock(x,y,items[i].id);
			}
			else if (items[i].type == tree)
			{
				environment[i] = new Tree(x,y,items[i].id);
			}
			else if (items[i].type == bush)
			{
				environment[i] = new Bush(x,y,items[i].id);
			}
			else if (items[i].type == egg)
			{
				environment[i] = new Egg(x,y,items[i].id);
			}
			else if(items[i].type == hatchery)
			{
				environment[i] = new Hatchery(x,y,items[i].id);
			}			
		}
		this.game.environment = this.game.environment.concat(environment);
		
	};
	
	/**
	* Stores data of the snakes on the server
	* @param data The information about the selected snake
	*/
	this.addSnakeHandler = function(data)
	{
		console.log(data);
		var snakes = data.snakes;
		//console.log(data.snakes);
		
		var US = this.game.userSnake;
		var currentPowerUp = 0;
		var scaleSize = this.game.scaleWindow;
		var id,team,color,angle,velocit,worldPos,drawpos,drawx,drawy;
		var segments = new Array();
		var tmpS;
		for (var i = 0;i<snakes.length;i++)
		{
			tmpS = data.snakes[i];
			id = tmpS.id;
			if (id != US.id)
			{	
				team = tmpS.team;
				color = team == 0 ? 'ff0000':'0000ff';
				angle = tmpS.velocity.angle;
				velocity = tmpS.velocity.magnitude;
				worldPos = 
					{
						x: tmpS.position.x / 1000,
						y: tmpS.position.y / 1000
					};
				
				drawx = worldPos.x-US.worldPos.x;
				drawy = worldPos.y-US.worldPos.y;
				
				drawx *= scaleSize;
				drawy *= scaleSize;
				
				drawx += US.x;
				drawy += US.y;
				
				drawPos = {x: drawx,y: drawy};
				
				for(var i = 0; i < tmpS.segments.length; i++)
				{
					var tempSegment = 
					{
						from: 
						{
							x: tmpS.segments[i].from.x / 1000,
							y: tmpS.segments[i].from.y / 1000,
						},
						control1: 
						{
							x: tmpS.segments[i].control1.x / 1000,
							y: tmpS.segments[i].control1.y / 1000,
						},
						control2: 
						{
							x: tmpS.segments[i].control2.x / 1000,
							y: tmpS.segments[i].control2.y / 1000,
						},
						to: 
						{
							x: tmpS.segments[i].to.x / 1000,
							y: tmpS.segments[i].to.y / 1000,
						}
					}
					//console.log(tempSegment);
					
					segments[i] = tempSegment;
				}
				//console.log(segments);	
				
				this.game.snakes.push(new Snake(id,team,color,velocity,angle,currentPowerUp,segments.length,segments,id,worldPos,this.game.scaleWindow,drawPos,false));
			}
		};
		
	
	};
	
	/**
	* Updates the player based on info stored on the server
	* @param data Player related data
	*/
	this.playerUpdateHandler = function(data)
	{
		//console.log(data);
		var snakes = data.snakes;
	//	console.log(data.snakes);	
		var US = this.game.userSnake;
		var currentPowerUp = 0;
		var scaleSize = this.game.scaleWindow;
		var id,team,color,angle,velocity,worldPos,drawpos,s;
		var segments = new Array();
		var tmpS;
		for (var i = 0;i<snakes.length;i++)
		{
			tmpS = data.snakes[i];
			id = tmpS.id;
			if (this.game.userSnake.id != id)
			{
				// s = 0;
				// while (s<this.game.snakes.length && this.game.snakes[s].id != id) {s++;}
				
				// if (s <this.game.snakes.length)
				// {
					//console.log(s);
					team = tmpS.team;
					color = team == 0 ? 'ff0000':'0000ff';
					angle = tmpS.velocity.angle;
					velocity = tmpS.velocity.magnitude;
					worldPos = 
						{
							x: tmpS.position.x / 1000,
							y: tmpS.position.y / 1000
						};
					
					drawx = worldPos.x-US.worldPos.x;
					drawy = worldPos.y-US.worldPos.y;
					
					drawx *= scaleSize;
					drawy *= scaleSize;
					
					drawx += US.x;
					drawy += US.y;
					
					drawPos = {x: drawx,y: drawy};
					
					//segments = tmpS.segments;
					
					for(var j = 0; j < tmpS.segments.length; j++)
					{
						var tempSegment = 
						{
							from: 
							{
								x: tmpS.segments[j].from.x / 1000,
								y: tmpS.segments[j].from.y / 1000,
							},
							control1: 
							{
								x: tmpS.segments[j].control1.x / 1000,
								y: tmpS.segments[j].control1.y / 1000,
							},
							control2: 
							{
								x: tmpS.segments[j].control2.x / 1000,
								y: tmpS.segments[j].control2.y / 1000,
							},
							to: 
							{
								x: tmpS.segments[j].to.x / 1000,
								y: tmpS.segments[j].to.y / 1000,
							}
						}
						
						//console.log(tempSegment);
						
						segments[j] = tempSegment;
					}
					// for (var k = 0;k < this.game.snakes.length;k++)
					// {
						// this.game.snakes.pop();
					// }
					// this.game.snakes.push(new Snake(id,team,color,velocity,angle,currentPowerUp,segments.length,segments,id,worldPos,this.game.scaleWindow,drawPos,false));
					var oldAng = this.game.snakes[i].angle;
					angle /= (180/Math.PI);
					// this.game.snakes[i].body.remove();
					// this.game.snakes[i].body = new paper.Path();
					this.game.snakes[i].id = id;
					this.game.snakes[i].team = team;
					this.game.snakes[i].color = color;
					this.game.snakes[i].velocity = velocity;
					this.game.snakes[i].angle = angle;
					this.game.snakes[i].currentPowerUp = currentPowerUp;
					this.game.snakes[i].numSegments = (segments.length) + 2;
					this.game.snakes[i].bSegments = segments;
					this.game.snakes[i].worldPos = worldPos;
					this.game.snakes[i].x = drawPos.x;
					this.game.snakes[i].y = drawPos.y;
					// console.log("this.game.snakes[i].numSegments: " + this.game.snakes[i].numSegments);
					// console.log("this.game.snakes[s].bSegments.length: " + this.game.snakes[s].bSegments.length);
					// console.log("segments.length: " + segments.length);
					checkOtherSegments[i] = segments.length;
					// console.log("checking other segments length: " + checkOtherSegments[s]);
					// console.log("temp: " + temp[s]);
					if(checkOtherSegments[i] != temp[i])
					{
						// if(checkOtherSegments[i] < temp[i])
						// {
							// this.game.snakes[i].body.removeSegments();
							// this.game.snakes[i].body.add(new paper.Point(this.game.snakes[i].x,this.game.snakes[i].y));
							// this.game.snakes[i].numSegments = 2;
						// }
						// this.game.snakes[i].body.remove();
						// this.game.snakes[i].body = new paper.Path();
						// this.game.snakes[i].id = id;
						// this.game.snakes[i].team = team;
						// this.game.snakes[i].color = color;
						// this.game.snakes[i].velocity = velocity;
						// this.game.snakes[i].angle = angle;
						// this.game.snakes[i].currentPowerUp = currentPowerUp;
						// this.game.snakes[i].numSegments = segments.length;
						// this.game.snakes[i].bSegments = segments;
						// this.game.snakes[i].worldPos = worldPos;
						// this.game.snakes[i].x = drawPos.x;
						// this.game.snakes[i].y = drawPos.y;
						// if(checkOtherSegments[i] != 1)
						// {
						this.game.snakes[i].body.removeSegments();
							this.game.snakes[i].body.add(new paper.Point(this.game.snakes[i].x,this.game.snakes[i].y));
							this.game.snakes[i].numSegments = segments.length;
							this.game.snakes[i].init();	
							console.log("Performed init()");
						// }
					}
					temp[i] = checkOtherSegments[i];
					//this.game.snakes[s].init();
					// this.game.snakes[s].rotate((180/Math.PI)*(angle-oldAng));
					//console.log(angle);
				// }
				// else 
				// {
					// team = tmpS.team;
					// color = team == 0 ? 'ff0000':'0000ff';
					// angle = tmpS.velocity.angle;
					// velocity = tmpS.velocity.magnitude;
					// worldPos = tmpS.position;
					
					// drawx = worldPos.x-US.worldPos.x;
					// drawy = worldPos.y-US.worldPos.y;
					
					// drawx *= scaleSize;
					// drawy *= scaleSize;
					
					// drawx += US.x;
					// drawy += US.y;
					
					// drawPos = {x: drawx,y: drawy};
					// segments = tmpS.segments;			
					// this.game.snakes.push(new Snake(id,team,color,velocity,angle,currentPowerUp,segments.length,segments,id,worldPos,this.game.scaleWindow,drawPos,false));
				// }
			}
		}
		
	};
	/**
	* Removes a disconnected snake from the game
	* @param data The information of the removed snake
	*/
	this.removeSnakeHandler = function(data)
	{	
		//console.log(data);
		//console.log(this.game.snakes);
		for (var s = 0;s<data.snakes.length;s++)
		{
			var i = 0;
			while (i<this.game.snakes.length && this.game.snakes[i].id != data.snakes[s])
			{i++;}
			if (i < this.game.snakes.length)
			{
				this.game.snakes[i].body.remove();
				this.game.snakes[i].head.remove();	
				// this.game.snakes[i].eye1.remove();
				// this.game.snakes[i].eye2.remove();
				this.game.snakes.splice(i,1);
			}
		}
	//	console.log(this.game.snakes.length);
	};
	
	/**
	* Updates the snake's velocity dependant on if they are sprinting or not
	* @param data The snake's sprinting values
	*/
	this.sprintHandler = function(data)
	{
		this.game.userSnake.sprintTime = data.sprint.remaining;
		if (data.sprint.current == 'use')
		{
			this.game.userSnake.targetvelocity = 40;
		}
		else
		{
			this.game.userSnake.targetvelocity = 20;
		}
	};
	
	/**
	* Removes an object from the environment
	* @param data The deleted object
	*/
	
	this.removeEnvironmentHandler = function(data)
	{
		//console.log(data.items);
		var items = data.items;
		var enviro = this.game.environment;
		for (var i = 0;i<items.length;i++)
		{
			var e = 0;
			while (e<enviro.length && enviro[e].id != items[i]) {e++;}
			if(enviro[e].type != "Hatchery")
			{
				enviro.splice(e,1);
			}

		}
	};
	
	this.miniSnakesHandler = function(data)
	{
		// console.log(data);
		/* WARNING: We don't have minisnakes set up exactly yet. This is psuedocode for when we do
		 * You should be able to just switch variable names around though...or pretty close
		 */

		miniSnakeArray = this.game.miniSnakes;
		//console.log(miniSnakeArray);	

		for(var j = 0; j < data.minisnakes.length; j++)
		{
			var dataSnake = data.minisnakes[j];
			var miniSnake = 0;
			
			for(var i = 0; i < miniSnakeArray.length; i++) {
				if(miniSnakeArray[i].id == dataSnake.id) {
					miniSnake = miniSnakeArray[i];
					break;
				}
			}
			
			// If the MiniSnake already exists for the user
			if(miniSnake) {
				//miniSnake.velocity = dataSnake.velocity;
				
				// If the MiniSnake is moving, update it
				if(dataSnake.state == 0) {
					var to = {
							x: dataSnake.position.x / 1000,
							y: dataSnake.position.y / 1000
					}
					
					miniSnake.velocity = dataSnake.velocity;
					miniSnake.update(to);
				}
				// Else, the MiniSnake is dead
				else {
					miniSnake.head.remove();
					miniSnakeArray.splice(i,1);
				}

			}
			
			/*var inSnake = data.minisnakes[j];
			console.log(inSnake);
			var thisSnake = 0;
			for( var i =0; i < miniSnakeArray.length; i++)
			{
				if(inSnake.id == miniSnakeArray[i].id)
				{
					thisSnake = miniSnakeArray[i];
					break;
				}
			}
			if(thisSnake != 0)
			{
				if(inSnake.state == 1)//for attack
				{
					miniSnakeArray.splice(miniSnakeArray.indexOf(thisSnake), 1);
				}
				if(inSnake.state == -1) // for death
				{
					miniSnakeArray.splice(miniSnakeArray.indexOf(thisSnake), 1);
				}
				if(inSnake.state == 0)//for moving
				{
					thisSnake.move(inSnake.position.x/1000, inSnake.position.y/1000);
					thisSnake.updateVelocity(inSnake.velocity);
				}
			}
			else*/
			
			// If not, create a new MiniSnake for the user
			else {
				//team,color,state,pos,velocity
				//new Snake('',1,000000,0,0,-1,1,null,1,null,this.game.scaleWindow,{x:this.centerX,y:this.centerY},true);
				
				var inSnake = data.minisnakes[j];
				
				var user = this.game.userSnake;
				var scaleSize = this.game.scaleWindow;
				
				var position = {
						x: inSnake.position.x / 1000,
						y: inSnake.position.y / 1000
				};
				
				var drawx = position.x - user.worldPos.x;
				var drawy = position.y - user.worldPos.y;
				
				drawx *= scaleSize;
				drawy *= scaleSize;
				
				drawx += user.x;
				drawy += user.y;
				
				drawPos = {
					x: drawx,
					y: drawy
				};
				
				var miniS = new MiniSnake(
						inSnake.id, 
						inSnake.team, 
						inSnake.team == 0 ? 'ff0000' : '0000ff', 
						inSnake.state, 
						position,
						drawPos,
						inSnake.velocity,
						this.game.scaleWindow);
				
				miniSnakeArray.push(miniS);
			}	
		}
	}
	
	
	/**
	* Determines which type of message is being sent
	* @param data The server message
	*/
	this.processMessage = function(data) 
	{
		//console.log("Process Message: ", data.type);
		
		/*function stringer(obj, num) {
			var string = '';
			string += obj + "\n";
			if (obj instanceof Object) {
				for (var x in obj) {
					for (var j = 0; j < num; ++j) {
						string += "\t";
					}
					string += "" + x + ': ';
					string += stringer(obj[x], num + 1);
				}
			}
		
			return string;
		}
		var string = stringer(data, 0);
		console.log(string);*/
		
		var fn = this[data.type + 'Handler'];
		if (fn) 
		{			
			fn(data);
		}		
	};
	
	/**
	* Disconnects client from server if they are disconnected
	*/
	this.connectionClosed = function() 
	{
		webSocketService.hasConnection = false;
		$('#cant-connect').fadeIn(300);
	};
	
	/**
	* Sends an update to the client
	* @param snake The client's snake
	*/
	this.sendUpdate = function(snake) 
	{
		var message = {
				type:'update',
				id:snake.id,
				position:{x:parseInt(snake.worldPos.x),y:parseInt(snake.worldPos.y)},
				//velocity:{angle:parseInt(snake.angle*(180/Math.PI)),magnitude: snake.velocity}
				velocity:{angle:parseInt(snake.angle*(180/Math.PI)),magnitude: snake.requestvelocity}
				};
		//console.log("magSent:" + snake.requestvelocity);
		this.sendMessage(message);
	};
	
	/**
	* Sends a server message to the client
	* @param msg The message being sent
	*/
	this.sendMessage = function(msg) 
	{
		//console.log("sending ");
		//console.log(msg);
		webSocket.emit('message',msg);
	};
	
};

