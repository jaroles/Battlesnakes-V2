Snake.prototype = new GameObject();
function Snake(name,team,color,velocity,angle,currentPowerUp,numSegments,segments,id,worldPos,scaleSize,pos,center)
{	
	this.pos = pos;
	
	this.worldPos = worldPos;
	this.scaleSize = scaleSize;
	this.name = name;
	this.team = team;
	this.velocity = velocity;
	this.angle = angle;
	this.x = center.x;
	this.y = center.y;
	this.color ="#"+ color;
	this.numSegments = numSegments;
	this.id = id;
	this.powerUp = currentPowerUp;

	
	this.wiggleHeight = 10;	
	this.targetvelocity = 20;	
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
	this.eye2.smooth();//
	
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
	
	this.mySpline = new CubicBezierSpline(segments);
	
	
	this.body.strokeColor = this.color;
	this.body.strokeWidth = 2*this.scaleSize;

	
		this.rotate = function(da)
	{
		var rotationMatrix = paper.Matrix.getRotateInstance(da,this.x,this.y);
		this.head.transform(rotationMatrix);
		this.eye1.transform(rotationMatrix);
		this.eye2.transform(rotationMatrix);
	};
	
	this.collision = function(GameObject)
	{
	};
	
	this.changeTeam = function(team)
	{
		this.team = team;
	};
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