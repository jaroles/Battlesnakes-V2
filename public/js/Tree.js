/**
* @author: Ryan Howard
* @author: Andrew Wagenheim
* Software Development II
* Battle Snakes
*/

/**
* Creates a tree object to be placed in the game environment
* @param x X coordinate of the tree
* @param y Y coordinate of the tree
* @param id ID of the newly created tree
*/

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