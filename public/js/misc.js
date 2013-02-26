/**
* @author: Ryan Howard
* @author: Andrew Wagenheim
* Software Development II
* Battle Snakes
*/

/**
* Math Library
*/

var DONTENUMERATE = function(Obj, property, val) {
	Object.defineProperty(Obj, property, {
		value: val,
		configurable: true,
		enumerable: false,
		writable: true
	});
}

DONTENUMERATE(Array.prototype, 'clone', function() {
	return Array.prototype.slice.call(this);
});

DONTENUMERATE(Array.prototype, 'remove', function(element) {
    var index = this.indexOf(element);
    if (index > -1) {
        return this.splice(index, 1);
    }
    return undefined;
});

DONTENUMERATE(Array.prototype, 'complement', function(array) {
	var elements = this.slice(0);
	elements = elements.filter(function(element) {
		return (array.indexOf(element) != -1);
	});
	return elements;
});

DONTENUMERATE(Object.prototype, '_extends', function(source) {
	if (this instanceof Function && source instanceof Function) {
		this.prototype = Object.create(source.prototype);
		DONTENUMERATE(this.prototype, 'constructor', this);
	}
});

DONTENUMERATE(Object.prototype, 'extend', function(source) {
	for (var prop in source) {
		var propertyDescriptor = Object.getOwnPropertyDescriptor(source, prop),
			get = propertyDescriptor.get,
			set = propertyDescriptor.set,
			val = propertyDescriptor.value,
			writable = propertyDescriptor.writable,
			configurable = propertyDescriptor.configurable,
			enumerable = propertyDescriptor.enumerable;
		if (get || set) {
			Object.defineProperty(this, prop, {
				get: get,
				set: set,
				enumerable: enumerable,
				configurable: configurable
			});
		} else {
			if (val instanceof Object && 'clone' in val && val.clone instanceof Function) {
				val = val.clone();
			}
			Object.defineProperty(this, prop, {
				value: val,
				writable: writable,
				enumerable: enumerable,
				configurable: configurable
			});
		}
	}
	return this;
});
