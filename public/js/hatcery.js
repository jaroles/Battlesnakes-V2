Hatchery.prototype = new GameObject();
function Hatchery(x,y,id)
{
	this.type = "Hatchery";
	this.x = x;
	this.y = y;
	this.isCollidable = true;
	this.img = new Image();
	this.img.src = "img/hatchery.png";
	this.id = id;//
}