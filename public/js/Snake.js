/**
* @author: Ryan Howard
* @author: Andrew Wagenheim
* Software Development II
* Battle Snakes
*/

Snake.prototype = new GameObject();

/**
* Creates a snake object for the game
* @param name The name of the snake
* @param team Which team the snake is on
* @param color The snake's color (dependant on team)
* @param velocity The snake's current velocity
* @param angle The angle at which the snake is moving
* @param currentPowerUp The snake's current powerup
* @param numSegments Number of segments a snake has
* @param segments The snake's segments
* @param id The ID number of a snake
* @param worldPos The position of the snake
* @param scaleSize The scale of the snake relative to the world
* @param drawPosition Drawing the sneak dependant on where it is
* @param isUserName Username associated to the snake
*/		
function Snake(name,team,color,velocity,angle,currentPowerUp,numSegments,segments,id,worldPos,scaleSize,drawPosition,isUserSnake)
{	
	var zero = new paper.Point(0,0);
	this.worldPos = worldPos;
	this.scaleSize = scaleSize;
	this.name = name;
	this.team = team;
	this.velocity = velocity;
	this.angle = angle;
	this.sprintTime;
	this.isUserSnake = isUserSnake;
	
	this.requestvelocity = -1;
	
	this.x = drawPosition.x;
	this.y = drawPosition.y;
	
	this.color ="#"+ color;
	this.numSegments = numSegments;
	this.id = id;
	this.powerUp = currentPowerUp;

	this.bSegments = segments;
	this.wiggleHeight = 10;	
	this.targetvelocity = 20;	
	this.maxSegments = 20;
	
	this.body = new paper.Path();
	window.snakebody = this.body;
	this.body.strokeColor = this.color;
	this.body.strokeWidth = 2*this.scaleSize;
	
	var point = new paper.Point(this.x - 200, this.y - 200);
	var size = new paper.Size(400, 400);
	this.head = new paper.Path.Rectangle(point, size);
	this.head.fillColor = this.color;
	this.head.strokeWidth = "1";
	this.head.strokeColor = this.color;
	this.head.mitterLimit = "10";		
	// this.head.add(new paper.Point(this.x,this.y));
	//this.head.lineTo(new paper.Point(79.8+this.x, this.y));
	// this.head.lineTo(new paper.Point(119.19+this.x,this.y - 84.85));
	// this.head.lineTo(new paper.Point(48.3+this.x, this.y-356.68));
	// this.head.lineTo(new paper.Point(-48.3+this.x, this.y-356.68));
	// this.head.lineTo(new paper.Point(-119.19+this.x, this.y - 84.85));
	// this.head.lineTo(new paper.Point(-79.8+this.x, this.y)); //433.68));
	this.head.closePath();
	this.head.smooth();
	
	// this.eye1 = new paper.Path();
	// this.eye1.fillColor = "pink";
	// this.eye1.strokeColor = "pink";
	// this.eye1.mitterLimit = "10";
	// this.eye1.add(new paper.Point(79.79+this.x, this.y - 247.8));
	// this.eye1.lineTo(new paper.Point(63.945+this.x, this.y-214.93));
	// this.eye1.lineTo(new paper.Point(48.1+this.x, this.y - 247.8));
	// this.eye1.lineTo(new paper.Point(63.945+this.x, this.y-289.5));
	// this.eye1.closePath();
	// this.eye1.smooth();
	
	// this.eye2 = new paper.Path();
	// this.eye2.fillColor = "pink";
	// this.eye2.strokeColor = "pink";
	// this.eye2.mitterLimit = "10";
	// this.eye2.add(new paper.Point(-79.79+this.x, this.y - 247.8));
	// this.eye2.lineTo(new paper.Point(-63.945+this.x, this.y-214.93));
	// this.eye2.lineTo(new paper.Point(-48.1+this.x, this.y - 247.8));
	// this.eye2.lineTo(new paper.Point(-63.945+this.x, this.y-289.5));
	// this.eye2.closePath();
	// this.eye2.smooth();
	
	var matrix = paper.Matrix.getRotateInstance((57.295*(Math.PI/2)),this.x,this.y);
	this.head.transform(matrix);
	// this.eye1.transform(matrix);
	// this.eye2.transform(matrix);
	
	var scaleSize = this.scaleSize/100;
	matrix = paper.Matrix.getScaleInstance(scaleSize,scaleSize);
	this.head.transform(matrix);
	// this.eye1.transform(matrix);
	// this.eye2.transform(matrix);
	
	matrix = paper.Matrix.getTranslateInstance(this.x-(this.x*scaleSize)-4,this.y-(this.y*scaleSize));
	this.head.transform(matrix);
	// this.eye1.transform(matrix);
	// this.eye2.transform(matrix);
	
	/**
	* Initiates the snake
	*/
	this.init = function()
	{
		
		//console.log(this.bSegments.length,this.bSegments);
		var relative = (new Point(this.x,this.y)).subtract(new Point(worldPos));

		this.mySpline = new CubicBezierSpline(this.bSegments).relocate(this.x,this.y);
		(function(snake) {
			snake.mySpline.vel = function() {
				return new Vector(Math.cos(snake.angle)*snake.velocity,Math.sin(snake.angle)*snake.velocity); 
			}
		})(this);		
		
		var l = this.mySpline.bezierSegments.length;
		this.mySpline.breakUp();
		var lastSeg = undefined;
		for (var i = l - 1; i >= 0; --i) {
			var seg = this.mySpline.bezierSegments[i];
			var from = seg.from,
				c1 = seg.control1,
				c2 = seg.control2,
				to = seg.to;
			c1.subtract(from).multiply(this.scaleSize).add(from);
			c2.subtract(from).multiply(this.scaleSize).add(from);
			to.subtract(from).multiply(this.scaleSize).add(from);

			if (lastSeg) {
				lastSeg.relocate(to);
			}
			lastSeg = seg;
		}
		this.mySpline.rejoin();

		if (l) {
			this.body.moveTo(this.mySpline.bezierSegments[0].from);
		}
		
		for (var i = 0; i < l; ++i) {
			var seg = this.mySpline.bezierSegments[i];
			var from = seg.from;
			var c1 = seg.control1,
				c2 = seg.control2,
				to = seg.to;
			//console.log(from, c1 , c2, to);
			this.body.cubicCurveTo(c1, c2, to);
		}
		
		var segments = this.body.segments;
		var dx = this.x - segments[0].point.x;
		var dy = this.y - segments[0].point.y;
		var m = paper.Matrix.getTranslateInstance(dx,dy);
		this.body.transform(m);
		
		//
		// Custom code
		//
		
		/*this.mySpline = new CubicBezierSpline(this.bSegments).relocate(this.x,this.y);
		
		(function(snake) {
			snake.mySpline.vel = function() {
				return new Vector(Math.cos(snake.angle)*snake.velocity,Math.sin(snake.angle)*snake.velocity); 
			}
		})(this);

		var seg = this.mySpline.bezierSegments[0];
		var from = seg.from,
			c1 = seg.control1,
			c2 = seg.control2,
			to = seg.to;
		c1.subtract(from).multiply(this.scaleSize).add(from);
		c2.subtract(from).multiply(this.scaleSize).add(from);
		to.subtract(from).multiply(this.scaleSize).add(from);
		
		if (lastSeg)
		{
			lastSeg.relocate(to);
		}
		lastSeg = seg;
		
		this.mySpline.rejoin();
		
		if (l) 
		{
			this.body.moveTo(this.mySpline.bezierSegments[0].from);
		}
		
		var seg = this.mySpline.bezierSegments[0];
		var from = seg.from,
		var c1 = seg.control1;
		var c2 = seg.control2;
		var to = seg.to;
		//console.log(from, c1 , c2, to);
		this.body.cubicCurveTo(c1, c2, to);

		var segments = this.body.segments;
		var dx = this.x - segments[0].point.x;
		var dy = this.y - segments[0].point.y;
		var m = paper.Matrix.getTranslateInstance(dx, dy);
		this.body.transform(m);*/
	};
	
	/**
	* Rotates the snake head to where the play designates
	* @param da Angle of rotation
	*/
	this.rotate = function(da)
	{
		var rotationMatrix = paper.Matrix.getRotateInstance(da,this.x,this.y);
		this.head.transform(rotationMatrix);
		// this.eye1.transform(rotationMatrix);
		// this.eye2.transform(rotationMatrix);
		// var dx = this.x - this.head.segments[0].point.x;
		// var dy = this.y - this.head.segments[0].point.y;
		// var trans = paper.Matrix.getTranslateInstance(dx,dy);
		//console.log(dx,dy);
		matrix = paper.Matrix.getTranslateInstance(this.x-this.x,this.y-this.y);
		this.head.transform(matrix);
		// this.eye1.transform(trans);
		// this.eye2.transform(trans);
		// var translation;
	};
	
	/**
	* Collision detection for the snake
	* @param GameObject The object the snake collides with
	*/   
	this.collision = function(GameObject)
	{
	};
	
	/**
	* Changes the snake's team
	* @param team The snake's new team
	*/
	this.changeTeam = function(team)
	{
		this.team = team;
	};
	
	/**
	* Updates the snake's position relative to the server
	* @param dx Change in the snake's X position
	* @param dy Change in the snake's Y position
	*/
	this.update = function(dx,dy)
	{
		if(this.velocity != 0)
		{
			dx *= this.targetvelocity;
			dy *= this.targetvelocity;
	
			this.worldPos.x += dx;
			this.worldPos.y += dy;
			
			dx *= this.scaleSize;
			dy *= this.scaleSize;
				
			if(!this.isUserSnake)
			{
				this.x+=dx;
				this.y+=dy;
				var trans = paper.Matrix.getTranslateInstance(dx,dy);
				this.head.transform(trans);
				// this.eye1.transform(trans);
				// this.eye2.transform(trans);
			}	
				
			var d = new paper.Point(dx, dy);
			
			var segments = this.body.segments;
			// find length;
			var length = (this.body.segments[0].point).subtract(this.body.segments[1].point).length;
	
			this.body.segments[0].point = this.body.segments[0].point.add(d);
			// apply velocity
			for (var i = 0; i < this.body.segments.length - 1; i++) {
				var nextSegment = this.body.segments[i + 1];
				var position = this.body.segments[i].point;
				var angle = (position.subtract(nextSegment.point)).angle;
				var vector = new paper.Point({ angle: angle, length: 12*this.scaleSize});
				nextSegment.point = position.subtract(vector);
			}
			// remove old control points
			for (var i = 0;i<this.body.segments.length;++i)
			{
				this.body.segments[i].handleIn = zero.clone();
				this.body.segments[i].handleOut = zero.clone();
			}
			
			this.body.smooth();
			// move to drawPostion
			var dx = this.x - this.body.segments[0].point.x;
			var dy = this.y - this.body.segments[0].point.y;
			var m = paper.Matrix.getTranslateInstance(dx,dy);
			this.body.transform(m);
		}
	};
	
	this.init();
}


























	/*var segments = [];
	this.body =  new paper.Path(new paper.Segment(new paper.Point(this.x,this.y)));
	var from,cont1,cont2,to,x,y;
	var center = new paper.Point(this.x,this.y);
	for (var i = 0;i<this.mySpline.bezierSegments.length;i++)
	{
		var bs = this.mySpline.bezierSegments[i];
		from = new paper.Point(this.mySpline.bezierSegments[i].from.x, this.mySpline.bezierSegments[i].from.y).multiply(this.scaleSize);
		this.mySpline.bezierSegments[i].from.reference(from);
		cont1 = new paper.Point(this.mySpline.bezierSegments[i].control1.x, this.mySpline.bezierSegments[i].control1.y).multiply(this.scaleSize);
		cont2 = new paper.Point(this.mySpline.bezierSegments[i].control2.x, this.mySpline.bezierSegments[i].control2.y).multiply(this.scaleSize);
		to = new paper.Point(this.mySpline.bezierSegments[i].to.x, this.mySpline.bezierSegments[i].to.y).multiply(this.scaleSize);
		this.body.cubicCurveTo(cont1,cont2,to);
	}
	*/
	/*this.tail = new paper.Path();
	this.tail.fillColor = this.color;
	this.tail.strokeWidth = "1";
	this.tail.strokeColor = this.color;
	this.tail.mitterLimit = "10";
	this.tail.add(new paper.Point(100, 100));
	this.tail.lineTo(new paper.Point(200, 100));
	this.tail.lineTo(new paper.Point(150, 300));
	this.tail.lineTo(new paper.Point(100, 100));
	this.tail.closePath();
	*/
	/*
	var matrix2 = paper.Matrix.getRotateInstance((57.295*(Math.PI/2)),this.x,this.y);
	this.tail.transform(matrix);
	
	var scaleSize = 0.05;
	matrix2 = paper.Matrix.getScaleInstance(scaleSize,scaleSize);
	this.tail.transform(matrix);
	
	var compoundPath = new paper.CompoundPath([this.body.segments[this.body.segments.length-1], this.tail]);
	compoundPath.fillColor = this.color;
    
	
	/*var startTail = new paper.Point(segments[segments.length-1].point);
	var tailStep = new paper.Point(-20,0);
	this.tail = new Array();
	for (var t = 0;t<(numSegments/10)+1;t++)
	{
		var segments = [new paper.Segment(new paper.Point(startTail.x+(-20*t),startTail.y)),new paper.Segment(new paper.Point(startTail.x*(-20*(t+1)),startTail.y))];
		this.tail[t] = new paper.Path(segments);
		this.tail[t].fillColor = "blue";
		this.tail[t].smooth();
	}*///
	/*
	this.updateVelocity = function()
	{
		if(this.targetvelocity != this.velocity) 
		{
			this.velocity += (this.targetvelocity - this.velocity)*this.gametime;
		}
		if(this.velocity < 0) 
		{
			this.velocity = 0;
		}
	};

	this.move = function(dx,dy)
	{
		var last = (this.lasttime) ? this.lasttime : (new Date()).getTime();
		var currentTime = (new Date()).getTime();
		var dt = (currentTime - last) / 1000;
		this.worldPos.x += dx * dt;
		this.worldPos.y += dy * dt;
		this.updatePath(dx * dt, dy * dt);
		this.lasttime = currentTime;
	};
		*/

	/*
	this.updatePath = function(dx, dy)
	{
		this.mySpline.move({x:dx,y:dy});
		
		this.body =  new paper.Path(new paper.Segment(new paper.Point(this.x,this.y)));
		var cont1,cont2,to,x,y;
		var center = new paper.Point(this.x,this.y);
		for (var i = 0;i<this.mySpline.bezierSegments.length;i++)
		{
			console.log(this.mySpline.bezierSegments[i]);
			cont1 = new paper.Point(this.mySpline.bezierSegments[i].control1.x, this.mySpline.bezierSegments[i].control1.y).multiply(this.scaleSize);
			cont2 = new paper.Point(this.mySpline.bezierSegments[i].control2.x, this.mySpline.bezierSegments[i].control2.y).multiply(this.scaleSize);
			to = new paper.Point(this.mySpline.bezierSegments[i].to.x, this.mySpline.bezierSegments[i].to.y).multiply(this.scaleSize);
			this.body.cubicCurveTo(cont1,cont2,to);//cont1.subtract(center),cont2.subtract(center),to.subtract(center));
		}
	/*
		var xStep = Math.cos(this.angle)*6*this.scaleSize;
		var yStep = Math.sin(this.angle)*6*this.scaleSize;

		this.body.insert(1,new paper.Point(this.x+dx,this.y+dy));
		if (this.body.segments.length > this.numSegments)
		{
			this.body.removeSegments(this.numSegments+1,this.body.segments.length);
		}
		for (var i = 1;i<this.body.segments.length;i++)
		{
			this.body.segments[i].point.x -= xStep;
			this.body.segments[i].point.y -= yStep;
		}
		this.body.smooth();
	
	};
	*/
//}