Troll.prototype = new GameObject();
function Troll(x,y,id)
{
	this.type = "Troll";
	this.x = x;
	this.y = y;
	this.isCollidable = true;
	this.img = new Image();
	this.img.src = "img/troll.png";
	this.id = id;
}