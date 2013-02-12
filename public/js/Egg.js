Egg.prototype = new GameObject();
function Egg(x,y,id)
{
	this.type = "egg";
	this.x = x;
	this.y = y;
	this.isCollidable = true;
	this.img = new Image();
	this.img.src = "img/egg.png";
	this.id = id;
	this.scale = 3;
}