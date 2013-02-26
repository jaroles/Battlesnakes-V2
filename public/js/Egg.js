/**
* @author: Ryan Howard
* @author: Andrew Wagenheim
* Software Development II
* Battle Snakes
*/

/**
* Creates an egg object to be placed in the game environment
* @param x X coordinate of the bush
* @param y Y coordinate of the bush
* @param id ID of the newly created bush
*/

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