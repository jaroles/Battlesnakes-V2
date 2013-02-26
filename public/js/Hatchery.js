/**
* @author: Ryan Howard
* @author: Andrew Wagenheim
* Software Development II
* Battle Snakes
*/

/**
* Creates a hatchery object to be placed in the game environment
* @param x X coordinate of the hatchery
* @param y Y coordinate of the hatchery
* @param id ID of the newly created hatchery
*/

Hatchery.prototype = new GameObject();
function Hatchery(x,y,id)
{
	this.type = "Hatchery";
	this.x = x;
	this.y = y;
	this.isCollidable = true;
	this.img = new Image();
	this.img.src = "img/hatchery.png";
	this.id = id;
	this.scale = 48
}