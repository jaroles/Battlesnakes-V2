var collision = require('./build/Release/collision');

var point = new collision.Point();

var vectorTo = new collision.Point();
var vector = new collision.Vector(vectorTo);

var from = new collision.Point();
var control1 = new collision.Point();
var control2 = new collision.Point();
var to = new collision.Point();
var cubicBezierSegment0 = new collision.CubicBezierSegment(from, control1, control2, to);
var cubicBezierSegment1 = new collision.CubicBezierSegment(from, control1, control2, to);
var cubicBezierSegment3 = new collision.CubicBezierSegment(from, control1, control2, to);

console.log("Collision Testing:\n");

// Test point(0, 0), vector(0, 0) and cubicBezierSegment0((0, 0), (0, 0), (0, 0), (0, 0))
// Return 0
console.log("Test 1");
console.log("Collision at segment 0: ", collision.collide(point, vector, [cubicBezierSegment0]), "\n");

// Test point(-5, 0), vector(0, 0) and  cubicBezierSegment0((0, 0), (0, 0), (0, 0), (0, 0))
// Return -1
console.log("Test 2");
point = new collision.Point(-5, 0);
console.log("No collision: ", collision.collide(point, vector, [cubicBezierSegment0]), "\n");

// Test point(0, 0), vector(0, 0) and 
// cubicBezierSegment0((0, 5), (0, 4), (0, 3), (0, 2)) and
// cubicBezierSegment1((0, 1), (0, 0), (0, -1), (0, -2))
// Return 1
console.log("Test 3");
point = new collision.Point(0, 0);

from = new collision.Point(0, 5);
control1 = new collision.Point(0, 4);
control2 = new collision.Point(0, 3);
to = new collision.Point(0, 2);
cubicBezierSegment0 = new collision.CubicBezierSegment(from, control1, control2, to);

from = new collision.Point(0, 2);
control1 = new collision.Point(0, 1);
control2 = new collision.Point(0, -1);
to = new collision.Point(0, -2);
cubicBezierSegment1 = new collision.CubicBezierSegment(from, control1, control2, to);

var segments = [cubicBezierSegment0, cubicBezierSegment1];

console.log("Collision at segment 1: ", collision.collide(point, vector, segments), "\n");

//Test point(0, 0), vector(0, 0) and 
//cubicBezierSegment0((1, 2), (3, 1), (3, -1), (1, -2))
//Return -1
console.log("Test 4");
point = new collision.Point(0, 0);

from = new collision.Point(1, 2);
control1 = new collision.Point(3, 1);
control2 = new collision.Point(3, -1);
to = new collision.Point(1, -2);
cubicBezierSegment0 = new collision.CubicBezierSegment(from, control1, control2, to);

console.log("No collision: ", collision.collide(point, vector, [cubicBezierSegment0]), "\n");

//Test point(2.5, 0), vector(0, 0) and 
//cubicBezierSegment0((1, 2), (3, 1), (3, -1), (1, -2))
//Return 0
console.log("Test 5");
point = new collision.Point(2.5, 0);

from = new collision.Point(1, 2);
control1 = new collision.Point(3, 1);
control2 = new collision.Point(3, -1);
to = new collision.Point(1, -2);
cubicBezierSegment0 = new collision.CubicBezierSegment(from, control1, control2, to);

console.log("Collision at segment 0: ", collision.collide(point, vector, [cubicBezierSegment0]), "\n");

//Test point(-2, 0), vector(0, 0) and
// cubicBezierSegment0((1, 5), (3, 4), (3, 3), (1, 2))
// cubicBezierSegment1((1, 2), (-3, 1), (-3, -1), (1, -2))
// cubicBezierSegment2((1, -2), (3, -3), (3, -4), (1, -5))
// Return 1
console.log("Test 6");

point = new collision.Point(-2, 0);

from = new collision.Point(1, 5);
control1 = new collision.Point(3, 4);
control2 = new collision.Point(3, 3);
to = new collision.Point(1, 2);
cubicBezierSegment0 = new collision.CubicBezierSegment(from, control1, control2, to);

from = new collision.Point(1, 2);
control1 = new collision.Point(-3, 1);
control2 = new collision.Point(-3, -1);
to = new collision.Point(1, -2);
cubicBezierSegment1 = new collision.CubicBezierSegment(from, control1, control2, to);

from = new collision.Point(1, -2);
control1 = new collision.Point(3, -3);
control2 = new collision.Point(3, -4);
to = new collision.Point(1, -5);
cubicBezierSegment2 = new collision.CubicBezierSegment(from, control1, control2, to);

console.log("Collision at segment 1: ", collision.collide(point, vector, [cubicBezierSegment0, cubicBezierSegment1, cubicBezierSegment2]), "\n");

//Test point(0, 0), vector(1, 1) and
//cubicBezierSegment0((1.5, 2.5), (2.5, 2.5), (3.5, 1.5), (3.5, 0.5))
//Return -1
console.log("Test 7")

point = new collision.Point(0, 0);

vectorTo = new collision.Point(1, 1);
vector = new collision.Vector(vectorTo);

from = new collision.Point(1.5, 2.5);
control1 = new collision.Point(2.5, 2.5);
control2 = new collision.Point(3.5, 1.5);
to = new collision.Point(3.5, 0.5);
cubicBezierSegment0 = new collision.CubicBezierSegment(from, control1, control2, to);

console.log("No collision: ", collision.collide(point, vector, [cubicBezierSegment0]), "\n");

//Test point(2.2974, 2.2974), vector(0, 0) and
//cubicBezierSegment0((1.5, 2.5), (2.5, 2.5), (3.5, 1.5), (3.5, 0.5))
//Return 0
console.log("Test 8")

point = new collision.Point(2.2974, 2.2974);

from = new collision.Point(1.5, 2.5);
control1 = new collision.Point(2.5, 2.5);
control2 = new collision.Point(3.5, 1.5);
to = new collision.Point(3.5, 0.5);
cubicBezierSegment0 = new collision.CubicBezierSegment(from, control1, control2, to);

console.log("Collision at segment 0: ", collision.collide(point, vector, [cubicBezierSegment0]), "\n");

//Test point(2, 2), vector(1, 1) and
//cubicBezierSegment0((1.5, 2.5), (2.5, 2.5), (3.5, 1.5), (3.5, 0.5))
//Return 0
console.log("Test 9")

point = new collision.Point(2, 2);

vectorTo = new collision.Point(1, 1);
vector = new collision.Vector(vectorTo);

from = new collision.Point(1.5, 2.5);
control1 = new collision.Point(2.5, 2.5);
control2 = new collision.Point(3.5, 1.5);
to = new collision.Point(3.5, 0.5);
cubicBezierSegment0 = new collision.CubicBezierSegment(from, control1, control2, to);

console.log("Collision at segment 0: ", collision.collide(point, vector, [cubicBezierSegment0]), "\n");