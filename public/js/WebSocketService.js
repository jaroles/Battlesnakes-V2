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
		
	
		var segments = 
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
		this.game.userSnake.rotate((180/Math.PI)*(angle-oldAngle));
		this.game.userSnake.velocity = data.velocity.magnitude;
	
		console.log("fuck");
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
			
			/*this.game.userSnake.body.removeSegments();
			this.game.userSnake.body.add(new paper.Point(this.game.userSnake.x,this.game.userSnake.y));
			this.game.userSnake.numSegments = 2;*/
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
		//console.log(data);
		var snakes = data.snakes;
		//console.log(data.snakes);
		
		var US = this.game.userSnake;
		var currentPowerUp = 0;
		var scaleSize = this.game.scaleWindow;
		var id,team,color,angle,velocity,segments,worldPos,drawpos,drawx,drawy;
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
				segments = tmpS.segments;			
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
		var id,team,color,angle,velocity,segments,worldPos,drawpos,s;
		var tmpS;
		for (var i = 0;i<snakes.length;i++)
		{
			tmpS = data.snakes[i];
			id = tmpS.id;
			if (this.game.userSnake.id != id)
			{
				s = 0;
				while (s<this.game.snakes.length && this.game.snakes[s].id != id) {s++;}
				
				if (s <this.game.snakes.length)
				{
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
					
					segments = tmpS.segments;
					
					var oldAng = this.game.snakes[s].angle;
					angle /= (180/Math.PI);
					//this.game.snakes[s].body.remove();
					//this.game.snakes[s].body = new paper.Path();
					this.game.snakes[s].id = id;
					this.game.snakes[s].team = team;
					this.game.snakes[s].color = color;
					this.game.snakes[s].velocity = velocity;
					this.game.snakes[s].angle = angle;
					this.game.snakes[s].currentPowerUp = currentPowerUp;
					this.game.snakes[s].numSegments = segments.length;
					this.game.snakes[s].bSegments = segments;
					this.game.snakes[s].worldPos = worldPos;
					this.game.snakes[s].x = drawPos.x;
					this.game.snakes[s].y = drawPos.y;
					
					//this.game.snakes[s].init();
					this.game.snakes[s].rotate((180/Math.PI)*(angle-oldAng));
					//console.log(angle);
				}
				else 
				{
					team = tmpS.team;
					color = team == 0 ? 'ff0000':'0000ff';
					angle = tmpS.velocity.angle;
					velocity = tmpS.velocity.magnitude;
					worldPos = tmpS.position;
					
					drawx = worldPos.x-US.worldPos.x;
					drawy = worldPos.y-US.worldPos.y;
					
					drawx *= scaleSize;
					drawy *= scaleSize;
					
					drawx += US.x;
					drawy += US.y;
					
					drawPos = {x: drawx,y: drawy};
					segments = tmpS.segments;			
					this.game.snakes.push(new Snake(id,team,color,velocity,angle,currentPowerUp,segments.length,segments,id,worldPos,this.game.scaleWindow,drawPos,false));
				}
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
			enviro.splice(e,1);
		}
	};
	
	
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

