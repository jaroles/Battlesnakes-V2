var Point = require('./Point');
var Vector = require('./Vector');
var CubicBezierSegment = require('./CubicBezierSegment');
/* CubicBezierSpline class
 * Handles an array of CubicBezierSegments
 */
function CubicBezierSpline(bezierSegments /*Array[CubicBezierSegments]*/) {
	this.bezierSegments = [];
	this.set(bezierSegments);
	this.vel = function() {return new Vector()};
}

CubicBezierSpline.prototype.extend({
	isStraight: function() {
		var lastVector = this.vel();
		var straight = true;
		for (var i = 0, l = this.bezierSegments.length; i < l; ++i) {
			var segment = this.bezierSegments[i],
				v = (new Vector(segment.from)).subtract(segment.to);
			if (!lastVector.angleEquals(v)) {
				straight = false;
				break;
			}
			lastVector = v;
		}
		return straight;
	},
	shouldWiggle: function() {
		return (this.bezierSegments.length && this.vel().magnitude() && this.isStraight());
	},
	wiggle: function() {
		if (this.shouldWiggle()) {
			var now = Date.now() / 1000;
			var cp = 0,
				height = 5,
				vector = this.vel(),
				angle = vector.angle(),
				spline = (new CubicBezierSpline(this)).rotate(angle * -1);
			for (var i = 0, l = spline.bezierSegments.length; i < l; ++i) {
				//http://paperjs.org/tutorials/animation/creating-animations/
				var seg = spline.bezierSegments[i],
					sinus = Math.sin(now * 3.2 + cp);
				seg.control1.y = sinus * height;
				++cp;

				sinus = Math.sin(now * 3.2 + cp);
				seg.control2.y = sinus * height;
				++cp;
			}
			this.bezierSegments = spline.rotate(angle).bezierSegments;
		}
		return this;
	},
	approximate: function(segments /*= 100 */) {
		segments = (typeof segments == 'number') ? Math.round(segments) : 100;
		var points = [],
			l = this.bezierSegments.length;

		if (l) {
			points.push(
				new Point(this.bezierSegments[0].from)
			);
			for (var i = 0; i < l; ++i) {
				points = this.bezierSegments[i].approximate(points, segments);
			}
		}

		return points;
	},
	rotate: function(theta /*degrees*/) {
		this.breakUp();
		for (var i = 0, l = this.bezierSegments.length; i < l; ++i) {
			this.bezierSegments[i].rotate(theta);
		}
		this.rejoin();
		return this;
	},
	move: function(offset) {
		if (this.isStraight()) {
			this.add(offset);
		} else {
			var l = this.bezierSegments.length;
			if (l) {
				var bS = this.bezierSegments[0],
					length = (new Vector(bS.from)).subtract(bS.to).magnitude();
				bS.from.add(offset);
				for (var i = 0; i < l; ++i) {
					var bezierSegment = this.bezierSegments[i],
						from = new Point(bezierSegment.from),
						to = bezierSegment.to;
						d = (new Vector(from)).subtract(to);
					d.normalize().multiply(length);
					to.set(from.subtract(d.to));
				}
				this.smooth();
			}
		}
		return this;
	},
	relocate: function(point) {
		if (this.bezierSegments.length) {
			var from = this.bezierSegments[0].from,
				d = (new Point(point)).subtract(from);
			this.add(d);
		}
		return this;
	},
	add: function(offset) {
		this.breakUp();
		for (var i = 0, l = this.bezierSegments.length; i < l; ++i) {
			this.bezierSegments[i].add(offset);
		}
		this.rejoin();
		return this;
	},
	subtract: function(offset) {
		this.breakUp();
		for (var i = 0, l = this.bezierSegments.length; i < l; ++i) {
			this.bezierSegments[i].subtract(offset);
		}
		this.rejoin();
		return this;
	},
	breakUp: function() {
		for (var i = 1, l = this.bezierSegments.length; i < l; ++i) {
			var bezierSegment = this.bezierSegments[i];
			bezierSegment.from = new Point(bezierSegment.from);
		}
		return this;
	},
	rejoin: function() {
		for (var i = 1, l = this.bezierSegments.length; i < l; ++i) {
			var bezierSegment = this.bezierSegments[i],
				prevSegment = this.bezierSegments[i - 1];
			bezierSegment.from = prevSegment.to;
		}
		return this;
	},
	x: function(t) {
		return this.coordinate('x', t);
	},
	y: function(t) {
		return this.coordinate('y', t);
	},
	xPrime: function(t) {
		return this.coordinatePrime('x', t);
	},
	yPrime: function(t) {
		return this.coordinatePrime('y', t);
	},
	coordinate: function(xOrY /*string, 'x' or 'y'*/, t) {
		var index = Math.floor(t),
			segment = this.bezierSegments[index];
		return segment ? segment.coordinate(xOrY, t) : undefined;
	},
	coordinatePrime: function(xOrY /*string, 'x' or 'y'*/, t) {
		var index = Math.floor(t),
			segment = this.bezierSegments[index];
		return segment ? segment.coordinatePrime(xOrY, t) : undefined;
	},
	clone: function() {
		return new CubicBezierSpline(this);
	},
	toJSON: function() {
		var spline = [];
		for (var i = 0, l = this.bezierSegments.length; i < l; ++i) {
			spline[i] = this.bezierSegments[i].toJSON();
		}
		return spline;
	},
	push: function(segment) {
		segment = (segment instanceof CubicBezierSegment) ? segment : new CubicBezierSegment(segment);
		var l = this.bezierSegments.length;
		if (l) {
			segment.from = this.bezierSegments[l - 1].to;
		}
		this.bezierSegments[l] = segment;
		return this;
	},
	pop: function() {
		return this.bezierSegments.pop();
	},
	last: function() {
		return this.bezierSegments[this.bezierSegments.length - 1];
	},
	splice: function(index, howMany) {
		if (howMany) {
			return this.bezierSegments.splice(index, howMany);
		} else {
			return this.bezierSegments.splice(index);
		}
	},
	smooth: function() {
		function getFirstControlPoints(rhs) {
			var n = rhs.length,
				x = [], // Solution vector.
				tmp = [], // Temporary workspace.
				b = 2;
			x[0] = rhs[0] / b;
			// Decomposition and forward substitution.
			for (var i = 1; i < n; i++) {
				tmp[i] = 1 / b;
				b = (i < n - 1 ? 4 : 2) - tmp[i];
				x[i] = (rhs[i] - x[i - 1]) / b;
			}
			// Back-substitution.
			for (var i = 1; i < n; i++) {
				x[n - i - 1] -= tmp[n - i] * x[n - i];
			}
			return x;
		}

		var segments = this.bezierSegments,
			size = segments.length;

		// If there's only one segment,
		// it's already smooth-ed.
		if (size == 1) {
			return;
		}

		var knots = [];
		for (var i = 0; i < size; ++i) {
			knots.push(segments[i].from)
		}
		knots.push(segments[size - 1].to);

		var controlPoints = {
			x: [],
			y: []
		};
		for (var coord in controlPoints) {
			var rhs = [];
			for (var i = 1; i < size - 1; ++i) {
				rhs[i] = 4 * knots[i][coord] + 2 * knots[i + 1][coord];
			}
			rhs[0] = knots[0][coord] + 2*knots[1][coord];
			rhs[size - 1] = 3*knots[size - 1][coord];

			controlPoints[coord] = getFirstControlPoints(rhs);
		}

		for (var i = 0; i < size; ++i) {
			var segment = segments[i];
			if (i < size) {
				segment.control1 = new Point (controlPoints.x[i], controlPoints.y[i]);
				if (i < size - 1) {
					segment.control2 = new Point(
						2 * knots[i + 1].x - controlPoints.x[i + 1],
						2 * knots[i + 1].y - controlPoints.y[i + 1]
					);
				} else {
					segment.control2 = new Point(
						(knots[size].x - controlPoints.x[size - 1]) / 2,
						(knots[size].y - controlPoints.y[size - 1]) / 2
					);
				}
			}
		}
	},
	set: function(bezierSegments) {
		if (bezierSegments instanceof CubicBezierSpline) {
			bezierSegments = bezierSegments.bezierSegments;
		}
		if (Array.isArray(bezierSegments)) {
			for (var i = 0, l = bezierSegments.length; i < l; ++i) {
				var c = new CubicBezierSegment(bezierSegments[i]);
				this.push(c);
			}
		} else if (bezierSegments) {
			var c = new CubicBezierSegment(bezierSegments);
			this.push(c);
		}
		return this;
	}
});

module.exports = CubicBezierSpline;
