#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include <assert.h>

void Object_destroy (void *self)
{
	assert(self);

	Object *obj = self;
	if(obj) {
		if(obj->description)
			free(obj->description);
		free(obj);
	}
}

void Object_describe (void *self)
{
	assert(self);
	
	Object *obj = self;
	printf("%s.\n", obj->description);
}

int Object_init (void *self)
{
	assert(self);

	// Do nothing, really.
	return 1;
}

void *Object_move(void *self, Direction direction)
{
	assert(self);
	assert(direction >= 0);

	printf("You can't go that direction.\n");
	return NULL;
}

int Object_attack(void *self, int damage)
{
	assert(self);
	assert(damage >= 0);

	printf("You can't attack that.\n");
	return 0;
}

void *Object_new(size_t size, Object proto, char *description)
{
	assert(size);
	assert(description);

	// Setup the default functions in case they aren't set:
	if(!proto.init) 	proto.init = Object_init;
	if(!proto.describe) proto.describe = Object_describe;
	if(!proto.destroy)	proto.destroy = Object_destroy;
	if(!proto.attack) 	proto.attack = Object_attack;
	if(!proto.move) 	proto.move = Object_move;

	// We create a struct of one size, and then use
	// another pointer to "cast" it. Weird.
	Object *el = calloc(1, size);
	assert(el);
	*el = proto;

	// Copy the description over:
	el->description = strdup(description);
	assert(el->description);

	// Initialize it with whatever init we were given:
	if(!el->init(el)) {
		// Return of 0 means it didn't init properly:
		el->destroy(el);
		return NULL;
	} else {
		// All done, we made an object of any type.
		assert(el);
		return el;
	}
}

