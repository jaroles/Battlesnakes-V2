

function Snake(id) {
    this.initialized = false;
	this.id = id;
	this.type = this.constructor.name
	this.isCollidable = true;
	this.isStationary = false;

	this.name = 'Guest' + id;
	this.team = Teams.Red;
	this._velocity = new Vector();
	this.numSegments = 1;
	this.grid = null;
	this.height = 20;
	this.width = 20;
	this.position = new Point();
	this.eggs = [];
	this.powerups = [];
	this.sprintObj = {
		current: 'regen',
		remaining: 3
	};
	this.segments = new CubicBezierSpline();
	this.addSegment();

	(function(snake) {
		snake.segments.vel = function() {
			return snake.velocity;
		};
	})(this);
	this.update = function() {};
	this.updateSprint = function() {};
    this.initialized = true;
}

Snake._extends(GameObject);
Snake.prototype.extend({
	wiggle: function() {
		this.segments.wiggle();
		return this;
	},
	move: function(pointOrX, y) {
		var point = (pointOrX instanceof Point) ? pointOrX : new Point(pointOrX, y),
			d = (new Point(point)).subtract(this.position);
		this.position = point;
		this.segments.move(d);
		return this;
	},
	relocate: function(pointOrX, y) {
		var point = (pointOrX instanceof Point) ? pointOrX : new Point(pointOrX, y);
		this.position = point;
		this.segments.relocate(point);
		return this;
	},
	collision: function(gameObject) {
		if (gameObject.id == this.id) {
			return false;
		}
		if (gameObject.isStationary) {
			return gameObject.collision(this);
		} else {
			var offset = gameObject.position,
				velocity = gameObject.velocity,
				angle = velocity.angle(),
				magnitude = velocity.magnitude(),
				segments = (new CubicBezierSpline(this.segments))
					.subtract(offset).rotate(-1 * angle);
			for (var i = 0; i < segments.length; ++i) {
				var hit = segments[i].isZero(magnitude);
				if (hit) {
					this.segments.splice(i);
					gameObject.score('bite', 1);
					return i;
				}
			}
		}
	},

	get: function() {
		return {
			id: this.id,
			position: this.position.toJSON(),
			velocity: this.velocity.toJSON(),
			currentPowerup: null,
			segments: this.segments.toJSON()
		};
	},

	toJSON: function() {
		return {
			position: this.position.toJSON(),
			velocity: this.velocity.toJSON(),
			segments: this.segments.toJSON()
		};
	},

	addSnakePacket: function() {
		return {
			id: this.id,
			team: this.team,
			position: this.position,
			velocity: this.velocity,
			segments: this.segments
		};
	},

	addSegment: function() {
		var last = this.segments.last(),
			lastFrom = (last) ? last.from : this.velocity.to,
			lastPoint = (last) ? last.to : this.position,
			v = (new Vector(lastPoint)).subtract(lastFrom),
			angle = v.angleRadians();
		if (!this.initialized) {
			angle = Math.PI;
		}
		var x = Math.cos(angle),
			y = Math.sin(angle),
            cp1 = (new Point(
				x * 2,
				y * 2
			)).add(lastPoint),
			cp2 = (new Point(
				x * 4,
				y * 4
			)).add(lastPoint),
			to = (new Point(
				x * 6,
				y * 6
			)).add(lastPoint);
		var segment = new CubicBezierSegment(
			lastPoint,
			cp1,
			cp2,
			to
		);
		this.segments.push(segment);
		return this;
	},

	//passes in a string "Red" or "Blue"
	changeTeam: function(team) {
		if (Teams.hasOwnProperty(team)) {
			this.team = Teams[team];
		}
		return this;
	},

	//passes in a Powerup Object
	usePowerup: function(powerup) {
		var index = this.powerups.indexOf(powerup);
		return (index > -1) ? this.powerups.splice(index, 1) : undefined;
	},

	hasEggs: function() {
		return (this.eggs.length > 0);
	},

	pickUpEgg: function(egg) {
		this.eggs.push(egg);
		this.addSegment();
	},

	pickUpPowerup: function(powerup) {
		this.powerups.push(powerup);
	},

	usePowerup: function(powerup) {
		var index = this.powerups.indexOf(powerup);
		if (index > -1) {
			return this.powerup.splice(index, 1);
		}
	},

	sprint: function(elapsedTime) {
		var sprintObj = this.sprintObj;
		switch (sprintObj.current) {
			case 'use':
				if (sprintObj.remaining <= 0) {
					sprintObj.remaining = 0;
					sprintObj.current = 'regen';
					this.updateSprint();
					this.velocity = (new Vector(this.velocity)).divide(2);
					return;
				}
				// Drain at 1 unit per second
				sprintObj.remaining -= 1 * elapsedTime;
				break;
			case 'regen':
				if (sprintObj.remaining >= 3) {
					sprintObj.remaining = 3;
					sprintObj.current = 'full';
					this.updateSprint();
					return;
				}
				// Regen at 1/3 unit per second
				sprintObj.remaining += 1 / 3 * elapsedTime;
				break;
			case 'full':
				break;
		}
	},

	handleSprint: function(startStop) {
		sprintObj = this.sprintObj;
		switch (startStop) {
			case 'start':
				if (sprintObj.current != 'use' && sprintObj.remaining >= 1) {
					sprintObj.current = 'use';
					this.updateSprint();
					this.velocity = (new Vector(this.velocity)).multiply(2);
				}
				break;
			case 'stop':
				if (sprintObj.current == 'use') {
					sprintObj.current = 'regen';
					this.updateSprint();
					this.velocity = (new Vector(this.velocity)).divide(2);
				}
				break;
		}
	},

	get velocity() {
		return this._velocity;
	},

	set velocity(vel) {
		var oldVelocity = this._velocity;
		this._velocity = new Vector(vel);
		if (!oldVelocity.equals(this._velocity)) {
			this.update();
		}
	}
});
