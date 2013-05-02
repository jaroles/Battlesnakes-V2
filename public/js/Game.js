/**
* @author: Ryan Howard 
* @author: Andrew Wagenheim
* Software Development II
* Battle Snakes
*/

/**
* Creates a Game object, which is the basis for the snakes, environment objects, and display.
* @param aSettings The settings for the game
* @param aCanvas The canvas on which the objects (snake, bush, ect.) are drawn
*
*/
var Game = function(aSettings, aCanvas) {
	var game = this;
	var canvas,
		context,
		//webSocket,
		webSocketService,
		mouse = {x: 0, y: 0,set:0},
		keyNav = {x:0,y:0},
		message,
		ticksForMessage = 0
	;
	
	this.userSnake;
	this.snakes = new Array();
	this.environment = new Array();
	this.miniSnakes = new Array();
	this.score;
	this.time;
	this.started = false;
	this.sprint = 0;
	this.slowingRatio = 0.9;
	this.minRatio = 0.1;
	this.time = 0;
	this.stepsTillStrait = 3;
	this.scaleWindow = window.innerWidth/250;

	this.gametime;
	this.lasttime = new Date().getTime();
	this.wasSprinting = false;

	/**
	* Updates the game based on changes sent by the server
	*/
	game.update = function() 
	{
		if (this.started)
		{		
			var last = (this.lasttime) ? this.lasttime : (new Date()).getTime();
			var currentTime = (new Date()).getTime();
			var dt = (currentTime - last) / 1000;
			this.lasttime = currentTime;
			
			var oldVelocity = this.userSnake.velocity;
			var oldAngle = this.userSnake.angle;
			var dx = 0;
			var dy = 0;
			var ang;
			var point = new Point(0,0);
			var magnitude = 0;
			
			if (mouse.set)
			{
				dy = (mouse.y - this.userSnake.y);
				dx = (mouse.x - this.userSnake.x);
				
				point = new Vector(dx, dy);
				magnitude = point.magnitude();
				
				dx /= magnitude;
				dy /= magnitude;
				
				dx *= dt;
				dy *= dt;
				
				ang = point.angleRadians();	
				
				if (this.userSnake.sprintTime > 0 && keyNav.y)
				{
					this.wasSprinting = true;
					var message = {
						type:"sprint",
						sprint:"start"};
					webSocketService.sendMessage(message);
				}
				else if (this.wasSprinting)
				{
					var message = {
						type:"sprint",
						sprint:"stop"};
					webSocketService.sendMessage(message);
					this.wasSprinting = false;
				}
				
				//console.log("targetVelo:" + this.userSnake.targetvelocity);
				//console.log("Velocity:" + this.userSnake.velocity);
				this.userSnake.angle = ang;
				//console.log("Angle:" + this.userSnake.angle);
				// this.userSnake.velocity = this.userSnake.targetvelocity;
				this.userSnake.requestvelocity = this.userSnake.targetvelocity;
				//console.log("requestVelo:" + this.userSnake.requestvelocity);

				

				// console.log('User Snake: ', this.userSnake.id);
				// if (oldVelocity == 0 || dt > .01)
				if (oldVelocity == 0 || dt > 0.01)
				{
					webSocketService.sendUpdate(this.userSnake);
				}
				
				// if(oldAngle != ang)
				// {
					// this.userSnake.rotate((180/Math.PI)*(ang-oldAngle));
				// }
				
				this.userSnake.update(dx,dy);
				this.updateOtherSnakes(dx,dy);	
			}
			else 
			{
				if (oldVelocity != 0)
				{
					this.userSnake.velocity = 0;	
					this.userSnake.requestvelocity = 0;	
					webSocketService.sendUpdate(this.userSnake);
				}
			}
			
			if (this.snakes.length > 0)
			{
				//console.log("updating other snakes" , this.snakes.length);
				for (var i = 0;i<this.snakes.length;i++)
				{
					var s = this.snakes[i];
					if (s.velocity >0)
					{
						//console.log(Math.cos(s.angle)*s.velocity/s.targetvelocity,Math.sin(s.angle)*s.velocity/s.targetvelocity);
						var dx = ((Math.cos(s.angle)*s.velocity)/s.targetvelocity) * dt;
						var dy = ((Math.sin(s.angle)*s.velocity)/s.targetvelocity) * dt;
						s.update(dx,dy);
					}
				}
			}
			
			if(this.miniSnakes.length > 0) {
				for(var i = 0; i < this.miniSnakes.length; i++) {
					var miniSnake = this.miniSnakes[i];
				}
			}
		}
	};
	
	game.updateOtherSnakes = function(dx,dy)
	{
		if (this.userSnake.velocity)
		{
			var m = paper.Matrix.getTranslateInstance(-dx*this.userSnake.targetvelocity*this.scaleWindow,-dy*this.userSnake.targetvelocity*this.scaleWindow);
			for (var i = 0;i<this.snakes.length;i++)
			{
				this.snakes[i].body.transform(m);
				this.snakes[i].head.transform(m);
				//this.snakes[i].eye1.transform(m);
				//this.snakes[i].eye2.transform(m);
			}
		}
	};
	
	/**
	* Draws the game based on the passed in canvas size, snakes, and environment objects
	*/
	game.draw = function() 
	{		
		if (this.started)
		{
			paper.view.draw();
			var img,
			    scale,
			    dx,
			    dy,
			    envObj;
			var x = this.userSnake.worldPos.x;
			var y = this.userSnake.worldPos.y;
			//var scx = canvas.width/2;
			//var scy = canvas.height/2;
			
			for (var e = 0; e < this.environment.length; e++)
			{
				envObj = this.environment[e];
				img = envObj.img;
				scale = envObj.scale;

				dx = envObj.x - x;
				dy = envObj.y - y;

				dx -= (scale/2);
				dy -= (scale/2);
				
				dx *= this.scaleWindow;
				dy *= this.scaleWindow;
				
				dx += this.userSnake.x;
				dy += this.userSnake.y;
				
				scale *= this.scaleWindow;
				context.drawImage(img,dx,dy,scale,scale);
			}
			//console.log(ticksForMessage);
			if(ticksForMessage)
			{
				ticksForMessage-=1;
				context.fillText(message,canvas.width/2,canvas.height/2);
			}
		}
	};
	
	this.addMessage = function(text)
	{
		message = text;
		ticksForMessage = 200;
	};
	
	/**
	* Updates objects drawn in the game based on info sent out by the server
	*/
	game.updateGameObjects = function(dx,dy)
	{
		for (var e = 0;e<this.environment.length;e++)
		{
			this.environment[e].x -= dx;
			this.environment[e].y -= dy;
		}
	};
	
	/**
	* Starts the game
	*/
	this.start = function(snake)
	{
		this.userSnake = snake;
		this.started = true;
	};
	
	
	/**
	* Socket functions, used for server communication
	*/
	game.onSocketOpen = function(e) 
	{
		console.log('Socket opened!', e);
	};
	
	game.onSocketClose = function(e) 
	{
		webSocketService.connectionClosed();
	};
	
	game.onSocketMessage = function(e) 
	{
		//console.log("Socket Message: ", e);
		webSocketService.processMessage(e);
	};
	
	game.sendMessage = function(msg) 
	{
	    webSocketService.sendMessage(msg);
	};
	
	/**
	* Mouse functions, to determine if and where the mouse is clicked
	*/
	game.mousedown = function(e) 
	{
		mouse.set = 1;
		mouse.x = e.clientX;
		mouse.y = e.clientY;
	};
	
	game.mouseup = function(e) 
	{
		mouse.set = 0;
	};
	
	game.mousemove = function(e) 
	{
		mouse.x = e.clientX;
		mouse.y = e.clientY;
	};

	/**
	* Keyboard functions, to determine if a key is pressed, and which one
	*/
	game.keydown = function(e) 
	{
		if(e.keyCode == keys.up) 
		{
			keyNav.y = -1;
			e.preventDefault();
		}
		else if(e.keyCode == keys.down) 
		{
			this.sprint = 1;
			keyNav.y = 1;
			e.preventDefault();
		}
		else if(e.keyCode == keys.left) 
		{
			keyNav.x = -1;
			e.preventDefault();
		}
		else if(e.keyCode == keys.right) 
		{
			keyNav.x = 1;
			e.preventDefault();
		}
		else if(e.keyCode == keys.space)
		{
			
		}
	};
	game.keyup = function(e) 
	{
		if(e.keyCode == keys.up || e.keyCode == keys.down)
		{
			keyNav.y = 0;
			e.preventDefault();
		}
		else if(e.keyCode == keys.left || e.keyCode == keys.right) 
		{
			keyNav.x = 0;
			e.preventDefault();
		}
		else if(e.keyCode == keys.space)
		{
			this.sprint = 0;
		}
	};
	
	/**
	* Touchpad functions, to give the game touchscreen capabilities
	*/
	game.touchstart = function(e) 
	{
	  e.preventDefault();
	  mouse.set =1 ;		
	  var touch = e.changedTouches.item(0);
	  if (touch)
	  {
		  mouse.x = touch.clientX;
  		  mouse.y = touch.clientY;      
	  }    
	};
	
	game.touchend = function(e) 
	{
		mouse.set = 0;
	};
	
	game.touchmove = function(e) 
	{
		e.preventDefault();
		var touch = e.changedTouches.item(0);
		if (touch) 
		{
			mouse.x = touch.clientX;
			mouse.y = touch.clientY;      
		}			
	};
	
	/**
	* Calls the resizeCanvas method
	*/
	game.resize = function(e) 
	{
		resizeCanvas();
	};
	
	var getMouseWorldPosition = function() 
	{

	};
	
	/**
	* Resizes the canvas according to new dimensions being passed in
	*/
	var resizeCanvas = function() 
	{
		canvas.width = window.innerWidth;
		canvas.height = window.innerHeight;
	};
	
	/**
	* Updates MiniSnakes according to server values
	* @param id ID of the minisnake being updated
	* @param mini A Minisnake object being passed in
	*/
	this.updateMiniSnake = function(id,mini)
	{
		var i = 0;
		while (this.miniSnakes[i].id != id)
			{i++;}
		this.miniSnakes[i] = mini;
		this.view.draw(this);
	};
	
	/**
	* Updates the current score
	* @param score The new score
	*/
	this.updateScore = function(score)
	{
		this.score = score;
	};
	
	/**
	* Updates the server time for the game to move
	* @param time The time being passed in
	*/
	this.updateTime = function(time)
	{
		this.time = time;
	};
	/**
	* Constructor
	*/
	(function()
	{
		canvas = aCanvas;
		context = canvas.getContext('2d');
		paper.setup(canvas); // setup paper canvas
		resizeCanvas();
		
		this.ScoreBar = new ScoreBar();	
		
		var socket = io.connect(aSettings.socketServer);
		socket.on('connect', game.onSocketOpen);
		socket.on('message', game.onSocketMessage);
		socket.on('disconnect', game.onSocketClose);
		
		webSocketService = new WebSocketService(socket, this);

	})();
};

//////////old draw////////////////

			/*
			for (var e = 0;e<this.environment.length;e++)
			{
				var img = this.environment[e].img;
				var scale;
				if (this.environment[e].type == "Rock" || this.environment[e].type == "Tree" || this.environment[e].type == "Bush")
				{
					scale = 24*this.scaleSpeed;
				}
				else if(this.environment[e].type == "Hatchery")
				{
					scale = 48*this.scaleSpeed;
				}
				else
				{
					scale = 3*this.scaleSpeed;
				}
				dx = scale/2;
				dy = scale/2;
				context.drawImage(img,this.environment[e].x+dx,this.environment[e].y-dy,scale,scale);	
			}
			*/


//////////////////////Old update/////////////

			/*
			var oldVelocity = this.userSnake.velocity;
			var oldAngle = this.userSnake.angle;
			var dx = 0;
			var dy = 0;
			var ang;
			var point = new paper.Point([0,0]);
			var magnitude = 0;
			

			if (mouse.set ==1)
			{	
				dy = (mouse.y - this.userSnake.y);
				dx = mouse.x - this.userSnake.x;
				point = new paper.Point([dx, dy]);
				magnitude = point.length;
				dx /= magnitude * (this.scaleSpeed/this.userSnake.targetvelocity);
				dy /= magnitude * (this.scaleSpeed/this.userSnake.targetvelocity);
				magnitude = this.scaleSpeed;
				ang = point.angle;			
				if (ang < 0) 
				{
					ang = 360 + ang;
				}
				ang *= Math.PI / 180;
				//magnitude *= this.scaleSpeed;
				this.userSnake.velocity = magnitude;
				this.userSnake.angle = ang;
				
				var x = this.userSnake.x+(dx*0.3);
				var y = this.userSnake.y+(dy*0.3);
				var type;
				for (var i = 0;i<this.environment.length;i++)
				{	
					type = this.environment[i].type;
					
					if (type == "Rock" || type == "Bush" || type == "Tree" || type == "Hatchery")
					{
						if ((x >= this.environment[i].x && x <= this.environment[i].x+24*this.scaleSpeed) && (y >= this.environment[i].y && y <= this.environment[i].y+24*this.scaleSpeed))
						{
							this.userSnake.velocity = 0;
							magnitude = 0;
						}
					}					
				}
				
				if (oldVelocity != magnitude || Math.abs(oldAngle-ang)>0)
				{
					webSocketService.sendUpdate(this.userSnake);
				}
				if(oldAngle != ang)
				{
					this.userSnake.rotate((180/Math.PI)*(ang-oldAngle));
				}
				
				this.userSnake.move(dx,dy);
				this.updateGameObjects(dx,dy);	
			}
			else
			{
				if (oldVelocity != 0)
				{
					this.userSnake.velocity = 0;
					webSocketService.sendUpdate(this.userSnake);
				}
			}
			
			for (var s = 0;s<this.snakes.length;s++)
			{
				this.snakes[s].move();
			}
		}
*/		
	//};//
