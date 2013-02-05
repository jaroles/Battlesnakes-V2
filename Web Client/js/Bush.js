Bush.prototype = new GameObject();
function Bush(x,y,id)
{
	this.type = "Bush";
	this.x = x;
	this.y = y;
	this.isCollidable = true;
	this.img = new Image();
	this.img.src = "img/bush.png";
	this.id = id;
	this.dot = new paper.Path.Circle(new paper.Point(x,y),2);
	this.scale = 2
}