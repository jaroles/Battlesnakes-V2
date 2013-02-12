Rock.prototype = new GameObject();
function Rock(x,y,id)
{
	this.type = "Rock";
	this.x = x;
	this.y = y;
	this.isCollidable = true;
	this.img = new Image();
	this.img.src = "img/rock.png";
	this.id = id;
	this.scale = 24;
}