
var Game = function(aSettings, aCanvas) {
	var game = this;
	var canvas,
		context,
		webSocket,
		webSocketService,
		mouse = {x: 0, y: 0,set:0},
		keyNav = {x:0,y:0}
	;
	
	
	this.userSnake;
	this.snakes = new Array();
	this.environment = new Array();
	this.miniSnakes = new Array();
	this.score;
	this.time;
	this.started = false;
	this.makeBigger = 0;
	this.slowingRatio = 0.9;
	this.minRatio = 0.1;
	this.time = 0;
	this.stepsTillStrait = 3;
	this.scaleWindow = window.innerWidth/300;

	this.gametime;
	this.lasttime = new Date().getTime();
	
	game.update = function() 
	{
		if (this.started)
		{
			
			var last = (this.lasttime) ? this.lasttime : (new Date()).getTime();
			var currentTime = (new Date()).getTime();
			var dt = (currentTime - last) / 1000;
			
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
				dx /= magnitude*dt;
				dy /= magnitude*dt;
				ang = point.angle;			
				if (ang < 0) 
				{
					ang = 360 + ang;
				}
				ang *= Math.PI / 180;							
			}
			
			this.lasttime = new Date().getTime();
		};
	
	game.draw = function() 
	{		
		if (this.started);
		{
			paper.view.draw();
			for (var e = 0;e<this.environment.length;e++)
			{
				var img = this.environment[e].img;
				var scale = this.environment[e].scale;
			}
		}
	};
	game.updateGameObjects = function(dx,dy)
	{
		var translationMatrix = paper.Matrix.getTranslateInstance(-dx,-dy);
		for (var e = 0;e<this.environment.length;e++)
		{
			this.environment[e].x -= dx;
			this.environment[e].y -= dy;
		}
		for (var s = 0;s<this.snakes.length;s++)
		{	
			this.snakes[s].body = this.adjust(this.snakes[s].body,dx,dy);
			this.snakes[s].head.transform(translationMatrix);
			this.snakes[s].eye1.transform(translationMatrix);
			this.snakes[s].eye2.transform(translationMatrix);
		}
		
		for (var m = 0;m<this.miniSnakes.length;m++)
		{
			this.miniSnakes[m].x -= dx;
			this.miniSnakes[m].y -= dy;
		}
	}//
	this.start = function(snake)
	{
		this.userSnake = snake;
		this.started = true;
	};
	this.adjust = function(path,dx,dy)
	{
		for (var i = 0;i<path.segments.length;i++)
		{
			path.segments[i].point.x -=dx;
			path.segments[i].point.y -=dy;
		}
		return path;
	};
	
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
		webSocketService.processMessage(e);
	};
	
	game.sendMessage = function(msg) 
	{
	    webSocketService.sendMessage(msg);
	};
	
	
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

	game.keydown = function(e) 
	{
		if(e.keyCode == keys.up) 
		{
			keyNav.y = -1;
			e.preventDefault();
		}
		else if(e.keyCode == keys.down) 
		{
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
			this.makeBigger = 1;
		}
	};
	game.keyup = function(e) 
	{
		if(e.keyCode == keys.up || e.keyCode == keys.down)
		{
			keyNav.y = 0;
			if(keyNav.x == 0 && keyNav.y == 0) 
			{
				
			}
			e.preventDefault();
		}
		else if(e.keyCode == keys.left || e.keyCode == keys.right) 
		{
			keyNav.x = 0;
			if(keyNav.x == 0 && keyNav.y == 0) 
			{
				
			}
			e.preventDefault();
		}
		else if(e.keyCode == keys.space)
		{
			this.makeBigger = 0;
		}
	};
	
	game.touchstart = function(e) 
	{
	  e.preventDefault();
	  mouse.clicking = true;		
	  var touch = e.changedTouches.item(0);
	  if (touch)
	  {
		  mouse.x = touch.clientX;
  		  mouse.y = touch.clientY;      
	  }    
	};
	
	game.touchend = function(e) 
	{

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
	
	
	game.resize = function(e) 
	{
		resizeCanvas();
	};
	
	var getMouseWorldPosition = function() 
	{

	};
	
	var resizeCanvas = function() 
	{
		canvas.width = window.innerWidth;
		canvas.height = window.innerHeight;
	};
	
	
	this.updateMiniSnake = function(id,mini)
	{
		var i = 0;
		while (this.miniSnakes[i].id != id)
			{i++;}
		this.miniSnakes[i] = mini;
		this.view.draw(this)
	};
	
	this.updateScore = function(score)
	{
		this.score = score;
	};
	
	this.updateTime = function(time)
	{
		this.time = time;
	};
	// Constructor
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
		
		webSocketService = new WebSocketService(socket);

	})();
}

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
