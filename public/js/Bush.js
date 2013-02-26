/**
* @author: Ryan Howard
* @author: Andrew Wagenheim
* Software Development II
* Battle Snakes
*/

//For restarting

/**
* Creates a bush object to be placed in the game environment
* @param x X coordinate of the bush
* @param y Y coordinate of the bush
* @param id ID of the newly created bush
*/
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
	//this.dot = new paper.Path.Circle(new paper.Point(x,y),2);
	this.scale = 24;
}