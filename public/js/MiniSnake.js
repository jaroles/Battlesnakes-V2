/**
* @author: Ryan Howard
* @author: Andrew Wagenheim
* Software Development II
* Battle Snakes
*/

/**
* Draws a Minisnake in the world
*/
MiniSnake.prototype = new GameObject();

function MiniSnake(id,team,color,state,pos,drawPos,velocity,scale)
{
	this.worldPos = pos;
	
	this.id = id
	this.team = team;
	this.color = "#" + color;
	this.state = state;
	this.x = drawPos.x;
	this.y = drawPos.y;
	this.velocity = velocity;
	
	this.scale = scale;
	
	var position = new paper.Point(this.x, this.y);
	this.head = new paper.Path.Rectangle(position, 16);
	this.head.fillColor = this.color;
	this.head.strokeWidth = "1";
	this.head.strokeColor = this.color;
	this.head.mitterLimit = "10";		
	
	/*this.gametime;
	this.lasttime = new Date().getTime();*/
	
	/**
	* Updates the velocity of the Minisnakes according to a value sent by the server
	*/
	this.updateVelocity = function()
	{
	
		/*if(this.targetvelocity != this.velocity) 
		{
			this.velocity += (this.targetvelocity - this.velocity)*this.gametime ;
		}
		if(this.velocity < 0) 
		{
			this.velocity = 0;
		}*/
	};
	
	/**
	* Moves the snake relative to their current velocity and angle of trejectory
	* @param dx Change in X position of the minisnake
	* @param dy Change in Y position of the minisnake
	*/
	this.update = function(to)
	{
		//var dx = this.worldPos.x - to.x;
		//var dy = this.worldPos.y - to.y;
		
		var dx = to.x - this.worldPos.x;
		var dy = to.y - this.worldPos.y;
		
		dx *= this.scale;
		dy *= this.scale;
		
		var m = paper.Matrix.getTranslateInstance(dx,dy);
		this.head.transform(m);
		
		this.worldPos.x = to.x;
		this.worldPos.y = to.y;
	};
	
	this.move = function(dx, dy)
	{
		dx *= this.scale;
		dy *= this.scale;
		
		var m = paper.Matrix.getTranslateInstance(dx,dy);
		this.head.transform(m);
		
		this.worldPos.x = to.x;
		this.worldPos.y = to.y;
	};
	
	/**
	* Rotates the head of the Minisnake based on its movement
	* @param da Angle of rotation
	*/
	this.rotate = function(da)
	{
		/*var rotationMatrix = paper.Matrix.getRotateInstance(da,this.x,this.y);
		this.head.transform(rotationMatrix);*/
	};
	
	/**
	* Changes the path 
	* @param dx Change in X position of the minisnake
	* @param dy Change in Y position of the minisnake
	*/
	this.updatePath = function(x, y)
	{	
		/*var xStep = Math.cos(this.angle)*18;
		var yStep = Math.sin(this.angle)*18;
		
		
		this.body.insert(1,new paper.Point(x,y));
		if (this.body.segments.length > this.numSegments)
		{
			this.body.removeSegments(this.numSegments+1,this.body.segments.length);
		}
		for (var i = 1;i<this.body.segments.length;i++)
		{
			this.body.segments[i].point.x  -= xStep;
			this.body.segments[i].point.y -= yStep;
		}
		this.body.smooth();*/
	};
}
