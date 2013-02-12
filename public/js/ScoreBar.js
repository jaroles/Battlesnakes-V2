function ScoreBar()
{
	var canvas = document.getElementById('canvas');
	var height = canvas.height/20;
	
	this.leftPoint = new paper.Point((canvas.width/20),height);
	
	this.rightPoint = new paper.Point((canvas.width * (19/20)),height);
	
	this.meetingPoint = new paper.Point((canvas.width/2),height);
	
	var blueSide = new paper.Path();
	
	blueSide.add(this.leftPoint);

	blueSide.lineTo(this.meetingPoint);
	
	var redSide = new paper.Path([new paper.Segment(this.meetingPoint),new paper.Segment(this.rightPoint)]); 

	blueSide.strokeColor = 'blue';
	
	blueSide.strokeWidth = '20';
	
	redSide.strokeColor = 'red';
	
	redSide.strokeWidth = '20';
	
	blueSide.smooth();

	redSide.smooth();
	
}
		