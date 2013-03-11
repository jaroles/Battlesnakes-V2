/**
* @author: Ryan Howard
* @author: Andrew Wagenheim
* Software Development II
* Battle Snakes
*/

/**
* Creates a rock object to be placed in the game environment
* @param x X coordinate of the rock
* @param y Y coordinate of the rock
* @param id ID of the newly created rock
*/
Rock.prototype = new GameObject();
function Rock(x,y,id)
{
	this.type = "Rock";
	this.x = x;
	this.y = y;
	this.isCollidable = true;
	this.img = new Image();
	this.img.src = "img/rock_2.png";
	this.id = id;
	this.scale = 24;
}