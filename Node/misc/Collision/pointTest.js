var addon = require('./build/Release/addon');

var obj1 = new addon.Point(2,3);
//console.log(obj.set(8,5));
console.log('Creating new Point obj1 (2,3)');
console.log('obj1.x: ', obj1.getX(), '   obj1.y: ', obj1.getY());
console.log(obj1.get());

console.log();

var obj2 = new addon.Point(0,5);
var obj3 = new addon.Point(5,0);
console.log('Creating new Point obj2 (0,5)');
console.log('Creating new Point obj3 (5,0)');
console.log('obj1 is inside obj2 and obj3: ', obj1.inside(obj2, obj3));
console.log('obj2 is inside obj1 and obj3: ', obj2.inside(obj1, obj3));

console.log();

console.log('Translate obj1 by obj2');
obj1.translate(obj2);
console.log(obj1.get());

console.log();

console.log('Rotation matrix for obj1 of theta 90');
console.log(obj1.rotationMatrix(90));
console.log('Rotate obj1');
console.log(obj1.rotate(90));

console.log();

console.log('Clone obj1 into obj4');
var obj4 = obj1.clone();
console.log('obj4: ', obj4.get());