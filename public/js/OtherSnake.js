OtherSnake.prototype = new GameObject();
function OtherSnake(id,team,velocity,angle,currentPowerUp,segments,position)
{
	this.worldPos = position;
	this.id = id;
	this.team = team;
	this.color = team == 0 ? "#ff0000" : "#0000ff";
	this.velocity = velocity;
	this.angle = angle;
	
	
	this.body = new paper.Path();
	this.body.strokeColor = this.color;
	this.body.strokeWidth = 10;
	
	this.body.add(new paper.Point(segments[0].from));
	for (var i = 0;i<segments.length;i++)
	{
		this.body.lineTo(segments[i].to);
	}
	this.body.smooth();
	
	this.wiggleHeight = 10;	
	this.targetvelocity = 1;	
	this.maxSegments = 20;
	this.head = new paper.Path();
	this.head.fillColor = this.color;
	this.head.strokeWidth = "1";
	this.head.strokeColor = this.color;
	this.head.mitterLimit = "10";		
	this.head.add(new paper.Point(79.8+this.x, this.y));
	this.head.lineTo(new paper.Point(119.19+this.x,this.y - 84.85));
	this.head.lineTo(new paper.Point(48.3+this.x, this.y-356.68));
	this.head.lineTo(new paper.Point(-48.3+this.x, this.y-356.68));
	this.head.lineTo(new paper.Point(-119.19+this.x, this.y - 84.85));
	this.head.lineTo(new paper.Point(-79.8+this.x, this.y)); //433.68));
	this.head.closePath();
	this.head.smooth();
	
	this.eye1 = new paper.Path();
	this.eye1.fillColor = "white";
	this.eye1.strokeColor = "white"
	this.eye1.mitterLimit = "10";
	63.945
	this.eye1.add(new paper.Point(79.79+this.x, this.y - 247.8));
	this.eye1.lineTo(new paper.Point(63.945+this.x, this.y-214.93));
	this.eye1.lineTo(new paper.Point(48.1+this.x, this.y - 247.8));
	this.eye1.lineTo(new paper.Point(63.945+this.x, this.y-289.5));
	this.eye1.closePath();
	this.eye1.smooth();
	
	this.eye2 = new paper.Path();
	this.eye2.fillColor = "white";
	this.eye2.strokeColor = "white"
	this.eye2.mitterLimit = "10";
	this.eye2.add(new paper.Point(-79.79+this.x, this.y - 247.8));
	this.eye2.lineTo(new paper.Point(-63.945+this.x, this.y-214.93));
	this.eye2.lineTo(new paper.Point(-48.1+this.x, this.y - 247.8));
	this.eye2.lineTo(new paper.Point(-63.945+this.x, this.y-289.5));
	this.eye2.closePath();
	this.eye2.smooth();
	
	var matrix = paper.Matrix.getRotateInstance((57.295*(Math.PI/2)),this.x,this.y);
	this.head.transform(matrix);
	this.eye1.transform(matrix);
	this.eye2.transform(matrix);
	
	var scaleSize = 0.05;
	matrix = paper.Matrix.getScaleInstance(scaleSize,scaleSize);
	this.head.transform(matrix);
	this.eye1.transform(matrix);
	this.eye2.transform(matrix);
	
	matrix = paper.Matrix.getTranslateInstance(this.x-(this.x*scaleSize),this.y-(this.y*scaleSize));
	this.head.transform(matrix);
	this.eye1.transform(matrix);
	this.eye2.transform(matrix);
	
	this.updateVelocity = function()
	{
		if(this.targetvelocity != this.velocity) 
		{
			this.velocity += (this.targetvelocity - this.velocity)*this.gametime ;
		}
		if(this.velocity < 0) 
		{
			this.velocity = 0;
		}
	};
	
	this.move = function()
	{
		var dx = Math.cos(this.angle)*this.magnitude;
		var dy = Math.sin(this.angle)*this.magnitude;
		
		var last = (this.lasttime) ? this.lasttime : (new Date()).getTime();
		var currentTime = (new Date()).getTime();
		var dt = (currentTime - last) / 1000;

		this.worldPos.x += dx * dt;
		this.worldPos.y += dy * dt;
		this.x += (dx*dt);
		this.y += (dy*dt);
		this.updatePath(dx * dt, dy * dt);
		this.lasttime = currentTime;
		
		this.updatePath(dx,dy);
	};
	
	this.rotate = function(da)
	{
		var rotationMatrix = paper.Matrix.getRotateInstance(da,this.x,this.y);
		this.head.transform(rotationMatrix);
		this.eye1.transform(rotationMatrix);
		this.eye2.transform(rotationMatrix);
	};
	
	this.updatePath = function(dx, dy)
	{	
			var xStep = Math.cos(this.angle)*18;
			var yStep = Math.sin(this.angle)*18;
			
			this.body.insert(0,new paper.Point(this.x + dx,this.y+dy));
			if (this.body.segments.length > this.numSegments)
			{
				this.body.removeSegments(this.numSegments+1,this.body.segments.length);
			}
			for (var i = 1;i<this.body.segments.length;i++)
			{
				this.body.segments[i].point.x  -= xStep;
				this.body.segments[i].point.y -= yStep;
			}
			this.body.smooth();
	};
	
}