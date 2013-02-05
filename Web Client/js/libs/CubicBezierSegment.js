
/* CubicBezierSegment class
 *
 */
function CubicBezierSegment(from /*Point*/, control1 /*Point*/, control2 /*Point*/, to /*Point*/) {
	this.set(from, control1, control2, to);
}

CubicBezierSegment.prototype.extend({
	approximate: function(previousPts /* Array[Points] */, segments /*= 100 */) {
		/* Based on code from:
		 * https://github.com/WebKit/webkit/blob/master/Source/WebCore/platform/graphics/wince/PlatformPathWinCE.cpp#L77-118
		 */
		// Enforce types
		if (!Array.isArray(previousPts)) {
			previousPts = [];
		}
		segments = (typeof segments == 'number') ? Math.round(segments) : 100;

		var step = 1.0 / segments,
			t = 0.0;
		var pp = new Point();

		for (var i = 1; i <= segments; ++i) {
			t += step;
			pp.set(
				this.x(t),
				this.y(t)
			);
			previousPts.push(new Point(pp));
		}

		return previousPts;
	},
	rotate: function(theta /*degrees*/) {
		this.from.rotate(theta),
		this.control1.rotate(theta),
		this.control2.rotate(theta),
		this.to.rotate(theta)
		return this;
	},
	multiply: function(scalar) {
		this.from.multiply(scalar)
		this.control1.multiply(scalar);
		this.control2.multiply(scalar);
		this.to.multiply(scalar);
		return this;
	},
	divide: function(scalar) {
		this.from.divide(scalar)
		this.control1.divide(scalar);
		this.control2.divide(scalar);
		this.to.divide(scalar);
		return this;
	},
	add: function(offset) {
		this.from.add(offset)
		this.control1.add(offset);
		this.control2.add(offset);
		this.to.add(offset);
		return this;
	},
	subtract: function(offset) {
		this.from.subtract(offset)
		this.control1.subtract(offset);
		this.control2.subtract(offset);
		this.to.subtract(offset);
		return this;
	},
	isZero: function(x) {
		var roots = this.roots();
		roots = roots.filter(function(t) {
			return (t >= 0 && t <= 1);
		});
		for (var i = 0; i < roots.length; ++i) {
			var root = roots[i];
			var xt = this.x(root);
			if (xt >= 0 && xt <= x + 1) {
				return true;
			}
		}
		return false;
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
		xOrY = (xOrY == 'y') ? xOrY : 'x';
		var p1 = this.from[xOrY];
		var p2 = this.control1[xOrY];
		var p3 = this.control2[xOrY];
		var p4 = this.to[xOrY];
		return ((-p1+3*p2-3*p3+p4)*t*t*t + (3*p1-6*p2+3*p3)*t*t + (-3*p1+3*p2)*t + p1);
	},
	coordinatePrime: function(xOrY /*string, 'x' or 'y'*/, t) {
		xOrY = (xOrY == 'y') ? xOrY : 'x';
		var p1 = this.from[xOrY];
		var p2 = this.control1[xOrY];
		var p3 = this.control2[xOrY];
		var p4 = this.to[xOrY];
		return (3*(-p1+3*p2-3*p3+p4)*t*t + 2*(3*p1-6*p2+3*p3)*t + (-3*p1+3*p2));
	},
	roots: function() {
		var epsilon = 1e-6;
		// Pure genius
		// http://www.kevlindev.com/gui/math/polynomial/Polynomial.js
		// TODO: Code can probably be optimized.
		// TODO: We are focused on a very select group of roots. D:[0, 1]
		var results = [];

		var p1 = this.from.y;
		var p2 = this.control1.y;
		var p3 = this.control2.y;
		var p4 = this.to.y;

		var c3 = (-p1+3*p2-3*p3+p4); //t^3

		if (Math.abs(c3) <= epsilon) {
			var bottom = (2*(p1-2*p2+p3));
			if (Math.abs(bottom) <= epsilon) {
				var t = .5;
				for (var i = 0; i < 10; ++i) {
					var y = this.y(t);
					if (Math.abs(y) <= epsilon) {
						break;
					}
					t -= y/this.yPrime(t);
				}
				if (Math.abs(this.y(t)) <= epsilon) {
					results.push(t);
				}
			} else {
				results.push((p1-p2) / bottom);
			}
			return results;
		}

		var c2 = (3*p1-6*p2+3*p3) / c3;//t^2
		var c1 = (-3*p1+3*p2) / c3; //t^1
		var c0 = p1 / c3; //t^0

		var a = (3*c1 - c2*c2) / 3;
		var b = (2*c2*c2*c2 - 9*c1*c2 + 27*c0) / 27;
		var offset = c2 / 3;
		var discrim = b*b/4 + a*a*a/27;
		var halfB = b / 2;

		if ( Math.abs(discrim) <= epsilon ) disrim = 0;

		if ( discrim > 0 ) {
			var e = Math.sqrt(discrim);
			var tmp;
			var root;

			tmp = -halfB + e;
			if ( tmp >= 0 )
				root = Math.pow(tmp, 1/3);
			else
				root = -Math.pow(-tmp, 1/3);

			tmp = -halfB - e;
			if ( tmp >= 0 )
				root += Math.pow(tmp, 1/3);
			else
				root -= Math.pow(-tmp, 1/3);

			results.push( root - offset );
		} else if ( discrim < 0 ) {
			var distance = Math.sqrt(-a/3);
			var angle = Math.atan2( Math.sqrt(-discrim), -halfB) / 3;
			var cos = Math.cos(angle);
			var sin = Math.sin(angle);
			var sqrt3 = Math.sqrt(3);

			results.push( 2*distance*cos - offset );
			results.push( -distance * (cos + sqrt3 * sin) - offset);
			results.push( -distance * (cos - sqrt3 * sin) - offset);
		} else {
			var tmp;

			if ( halfB >= 0 )
				tmp = -Math.pow(halfB, 1/3);
			else
				tmp = Math.pow(-halfB, 1/3);

			results.push( 2*tmp - offset );
			// really should return next root twice, but we return only one
			results.push( -tmp - offset );
		}
		return results;
	},
	get: function() {
		return {
			from: this.from,
			control1: this.control1,
			control2: this.control2,
			to: this.to
		};
	},
	set: function(from, control1, control2, to) {
		if (from instanceof CubicBezierSegment) {
			this.set(from.from, from.control1, from.control2, from.to);
		} else if (Array.isArray(from)) {
			this.set(from[0], from[1], from[2], from[3]);
		} else {
			this.from = new Point(from);
			this.control1 = new Point(control1);
			this.control2 = new Point(control2);
			this.to = new Point(to);
		}
		return this;
	},
	clone: function() {
		return new CubicBezierSegment(this);
	},
	toJSON: function() {
		return this.get();
	}
});
