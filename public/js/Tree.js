Tree.prototype = new GameObject();
function Tree(x,y,id)
{
	this.type = "Tree";
	this.x = x;
	this.y = y;
	this.isCollidable = true;
	this.img = new Image();
	this.img.src = "img/tree.png";
	this.id = id;
	this.scale = 24;
}