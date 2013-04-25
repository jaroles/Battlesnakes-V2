function MiniSnake(id, position, team, velocity) 
{
	this.id = id;

	this.team = team;
	this.velocity = velocity;
	
	this.position = new Point(position.x, position.y);

	this.setPostition = function(pos)
	{
		this.position.x = pos.x;
		this.position.y = pos.y;
	};
	
	this.setTeam = function(tem)
	{
		this.team = tem;
	};
	
	this.setVelocity = function(velo)
	{
		this.velocity = velo;
	};
}