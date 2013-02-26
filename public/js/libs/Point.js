
/* Point class
 *
 * Based on code from:
 * https://github.com/WebKit/webkit/blob/master/Source/WebCore/platform/graphics/wince/PlatformPathWinCE.h#L32-55
 */
var Point = function(x, y) {
	this.set(x, y);
};

Point.prototype.extend({
	rotationMatrix: function(theta /*degrees*/) {
		var r = theta * Math.PI / 180,
			cos = Math.cos(r),
			sin = Math.sin(r);
		return [
			[cos, -sin],
			[sin, cos]
		];
	},
	rotate: function(theta /*degrees*/) {
		var m = this.rotationMatrix(theta);
		this.x = (m[0][0] * this.x + m[0][1] * this.y),
		this.y = (m[1][0] * this.x + m[1][1] * this.y)
		return this;
	},
	set: function(x /* Float or {x: x, y: y} */, y /* Float */) {
		if (Array.isArray(x)) {
			this.set(x[0], x[1]);
		} else if (x instanceof Object) {
			this.set(x.x, x.y);
		} else {
			x = (typeof x == 'number') ? x : 0;
			y = (typeof y == 'number') ? y : 0;
			this.x = x;
			this.y = y;
		}
		return this;
	},
	get: function() {
		return {
			x: this.x,
			y: this.y
		};
	},
	inside: function(topLeft, bottomRight) {
		if (this.x >= topLeft.x && this.x <= bottomRight.x) {
			if (this.y <= topLeft.y && this.y >= bottomRight.y) {
				return true;
			}
		}
		return false;
	},
	clone: function() {
		return new Point(this);
	},
	toJSON: function() {
		return {
			x: parseInt(this.x, 10),
			y: parseInt(this.y, 10)
		};
	},
	multiply: function(scalar) {
		if (typeof scalar == 'number') {
			this.x *= scalar,
			this.y *= scalar
		} else if (scalar instanceof Object) {
			this.x *= scalar.x,
			this.y *= scalar.y
		}
		return this;
	},
	divide: function(scalar) {
		if (scalar instanceof Object) {
			s = {};
			s.x = 1 / scalar.x;
			s.y = 1 / scalar.y;
			scalar = s;
		} else {
			scalar = 1 / scalar;
		}
		return this.multiply(scalar);
	},
	add: function(offset) {
		if (typeof offset == 'number') {
			this.x += offset,
			this.y += offset
		} else if (offset instanceof Object) {
			this.x += offset.x,
			this.y += offset.y
		}
		return this;
	},
	subtract: function(offset) {
		if (offset instanceof Object) {
			o = {};
			o.x = -1 * offset.x;
			o.y = -1 * offset.y;
			offset = o;
		} else {
			offset *= -1;
		}
		return this.add(offset);
	},
	equals: function(other) {
		return ((this.x === other.x) && (this.y === other.y));
	},
	reference: function(paperPoint) {
		(function(me, point) {
			me.__defineGetter__('x', function() {
				return point.x;
			});
			me.__defineSetter__('x', function(newX) {
				point.x = newX;
			});
			me.__defineGetter__('y', function() {
				return point.y;
			});
			me.__defineSetter__('y', function(newY) {
				point.y = newY;
			});
			me.__defineGetter__('paperPoint', function() {
				return point;
			});
		})(this, paperPoint);
	}
});

