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
	
	
	
	this.introHandler = function(data) 
	{
		console.log(data);
		var canvas = document.getElementById('canvas');
		var width = canvas.width;
		var height = canvas.height;
		this.centerX = width/2;
		this.centerY = height/2;
		webSocketService.hasConnection = true;
		$('#chat').initChat();
		var worldPos = data.position;
		var point = new paper.Point(this.centerX,this.centerY); 
		/*
		/*var segments = [];
		segments[0] = new paper.Segment(point);
		for (var i = 0;i<data.segments.length;i++)
		{
			point = new paper.Point(this.centerX-this.game.scaleSpeed*(worldPos.x-data.segments[i].to.x),centerY);
			segments[i+1] = new paper.Segment(point);
		}
		
		var segments = [];
		for (var i = 0;i<data.segments.length;i++)
		{
			segments[i] = new CubicBezierSegment(data.segments[i].from,data.segments[i].control1,data.segments[i].control2,data.segments[i].to);
		}
		
		var velocity = data.velocity.magnitude;
		var angle = data.velocity.angle;
		var color = data.team == 0 ? 'ff0000':'0000ff';
		var snake = new Snake(data.name,data.team,color,velocity,angle,-1,segments.length,segments,data.id,worldPos,this.game.scaleSpeed,data.position,{x:this.centerX,y:this.centerY});
		*/
		this.game.start(snake);	
	};
	
	this.updateHandler = function(data) 
	{
		//console.log("recieving");
		console.log(data);
		var dx = data.position.x-this.game.userSnake.worldPos.x; 
		var dy = data.position.y-this.game.userSnake.worldPos.y;
		this.game.userSnake.worldPos.x = data.position.x;
		this.game.userSnake.worldPos.y = data.position.y;
		var angle = data.velocity.angle*(Math.PI/180);	
		var oldAngle = this.game.userSnake.angle;
		this.game.userSnake.angle = angle;
		this.game.userSnake.rotate((180/Math.PI)*(angle-oldAngle));
		this.game.userSnake.velocity = data.velocity.magnitude;
	};
	
	this.chatHandler = function(data)
	{
			console.log(data);
	};
	
	this.eggHandler = function(data) 
	{
		this.game.userSnake.numSegments = 2+eggs.length;
	};
	
	this.closedHandler = function(data) 
	{
		//console.log(data);
	};
	
	this.addEnvironmentHandler = function(data)
	{
		/*//console.log(data);
		var items = data.items;
		var environment = new Array();
		var xDif = this.centerX-(this.game.userSnake.worldPos.x*this.game.scaleSpeed);
		var yDif = this.centerY-(this.game.userSnake.worldPos.y*this.game.scaleSpeed);	
		var x,y;
		for (var i = 0;i< items.length;i++)
		{
			
			x = (items[i].position.x*this.game.scaleSpeed)+xDif;
			y = (items[i].position.y*this.game.scaleSpeed);//+yDif;//+yDif;
			
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
				console.log(this.game.userSnake.worldPos,items[i].position);
				environment[i] = new Hatchery(x,y,items[i].id);
			}
			else
			{
				console.log(items[i].type);
				environment[i] = new Troll(x,y,items[i].id);
			}
		}*/
		
		var items = data.items;
		var environment = new Array();
		var x,y;
		for (var i = 0;i< items.length;i++)
		{
			var x = items[i].position.x;
			var y = items[i].position.y;
			//adujst x,y to scale to screen
			
			if (items[i].type == rock){
			}
			else if (items[i].type == tree){
			}
			else if (items[i].type == bush){
			}
			else if (items[i].type == egg){
			}
			else if(items[i].type == hatchery){
			}
			
		}
		this.game.environment = environment;
	};
	this.playerUpdateHandler = function(data)
	{
		//console.log(data);
		var snakes = data.snakes;
		//var xDif = this.game.userSnake.worldPos.x-this.centerX;
		//var yDif = this.game.userSnake.worldPos.y-this.centerY;
		var s;
		var snake;
		for (var i = 0;i<snakes.length;i++)
		{
			s = 0;
			while (s<this.game.snakes.length && snakes[i].id == this.game.snakes[s].id) {s++;}
			snake = this.game.snakes[s];
			var oldAngle = snake.angle;
			snake.worldPos = snakes[i].position;
			snake.velocity = snakes[i].velocity.magnitude*this.game.scaleSpeed;
			snake.angle = snakes[i].angle;
			snake.rotate((180/Math.PI)*(snake.angle-oldAngle));
			this.game.snakes[s] = snake;
			//this.game.snakes[s].move(
		}
		
	};
	this.removeSnakeHandler = function(data)
	{	
		for (var s = 0;s<this.game.snakes.length;s++)
		{
			var i = 0;
			while (i<data.snakes.length && this.game.snakes[s].id != data.snakes[i].id)
			{i++;}
			if (i < data.snakes.length)
			{
				for (var x = s+1;x<this.game.snakes.length;x++)
				{
					this.game.snakes[x-1] = this.game.snakes[x];
				}
			}
		}
	};
	this.sprintHandler = function(data)
	{
		console.log(data);
	};
	this.removeEnvironmentHandler = function(data)
	{
		console.log(data);
	};
	
	this.addSnakeHandler = function(data)
	{
		//console.log(data);
		var snakes = data.snakes;
		for (var i = 0;i<snakes.length;i++)
		{
			console.log(i);
			var newSnake = new OtherSnake(snakes[i].id,snakes[i].team,snakes[i].velocity.magnitude,snakes[i].velocity.angle,snakes[i].currentPowerUp,snakes[i].segments,snakes[i].position);
			this.game.snakes.push(newSnake);
		};
		
	};
	
	this.processMessage = function(data) 
	{
		console.log(data.type);
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
		var fn = webSocketService[data.type + 'Handler'];
		if (fn) 
		{			
			fn(data);
		}		
	};
	
	this.connectionClosed = function() 
	{
		webSocketService.hasConnection = false;
		$('#cant-connect').fadeIn(300);
	};
	
	this.sendUpdate = function(snake) 
	{
		var message = {
				type:'update',
				id:snake.id,
				position:{x:parseInt(snake.worldPos.x),y:parseInt(snake.worldPos.y)},
				velocity:{angle:parseInt(snake.angle*(180/Math.PI)),magnitude: snake.velocity>0?snake.targetvelocity:0}
				};
		this.sendMessage(message);
	};
	
	this.sendMessage = function(msg) 
	{
		//console.log("sending ");
		console.log(msg);
		//*/
		webSocket.emit('message',msg);
	};
	
};