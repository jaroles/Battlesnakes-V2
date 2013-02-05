var settings = new Settings();
var debug = true;
var isStatsOn = false;
var authWindow;
var game;
var runLoop = function() {
	game.update();
	game.draw();
};

var initApp = function() 
{
	if (game!=null) { return; }
	game = new Game(settings, document.getElementById('canvas'));

	//window.addEventListener('resize',game.resize, false);
	document.addEventListener('mousemove',game.mousemove, false);
	document.addEventListener('mousedown',game.mousedown, false);
	document.addEventListener('mouseup',game.mouseup, false);
	
	document.addEventListener('touchstart',game.touchstart, false);
	document.addEventListener('touchend',  game.touchend, false);
	document.addEventListener('touchcancel',game.touchend, false);
	document.addEventListener('touchmove',game.touchmove, false);	
	document.addEventListener('keydown', game.keydown, false);
	document.addEventListener('keyup', game.keyup, false);
	
	setInterval(runLoop,30);
};

var forceInit = function() 
{
	initApp();
	document.getElementById('unsupported-browser').style.display = "none";
	return false;
};

if(Modernizr.canvas && Modernizr.websockets) 
{
	initApp();
} 
else 
{
	document.getElementById('unsupported-browser').style.display = "none";	
	document.getElementById('force-init-button').addEventListener('click', forceInit, false);
}

var addStats = function() 
{
	if (isStatsOn) { return; }
	// Draw fps
	var stats = new Stats();
	document.getElementById('fps').appendChild(stats.domElement);

	setInterval(function () 
	{
	    stats.update();
	}, 1000/60);

	Array.remove = function(array, from, to) 
	{
	  var rest = array.slice((to || from) + 1 || array.length);
	  array.length = from < 0 ? array.length + from : from;
	  return array.push.apply(array, rest);
	};
	isStatsOn = true;
};

if(debug) { addStats(); }

$(function() 
{
	$('a[rel=external]').click(function(e) {
		e.preventDefault();
		window.open($(this).attr('href'));
	});
});

document.body.onselectstart = function() { return false; };
