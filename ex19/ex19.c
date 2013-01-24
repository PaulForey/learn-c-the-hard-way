#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "game.h"

void *Map_move(void *self, Direction direction)
{
	assert(self);
	assert(direction >= 0);

	Map *map = self;
	Room *location = map->location;
	Room *next = NULL;
	next = location->_(move)(location, direction);
	if(next) {
		map->location = next;
	}
	return next;
}
int Map_attack(void *self, int damage)
{
	assert(self);
	assert(damage >= 0);

	Map *map = self;
	Room *location = map->location;
	return location->_(attack)(location, damage);
}
int Map_init(void *self)
{
	assert(self);
	
	Map *map = self;
	// Make some rooms for a small map:
	Room *hall = NEW(Room, "great hall");
	Room *throne = NEW(Room, "throne room");
	Room *arena = NEW(Room, "arena, with the minotaur");
	Room *kitchen = NEW(Room, "kitchen, you have the knife now");
	// Extra Credit: More rooms:
	Room *alley = NEW(Room, "alleyway. It stinks here");
	Room *outhouse = NEW(Room, "outhouse, and it's not a clean one");
	Room *bedroom = NEW(Room, "royal bedchambers. It stinks here");

	// Put the bad guy in the arena:
	arena->bad_guy = NEW(Monster, "evil minotaur");
	// Extra Credit: More bad guys:
	alley->bad_guy = NEW(Monster, "sly thief");
	bedroom->bad_guy = NEW(Monster, "winking serpent");

	// Set up the map rooms:
	hall->north = throne;
	throne->south = hall;

	throne->west = arena;
	arena->east = throne;

	throne->east = kitchen;
	kitchen->west = throne;

	kitchen->north = alley;
	alley->south = kitchen;

	alley->east = outhouse;
	outhouse->west = alley;

	throne->north = bedroom;
	bedroom->south = throne;

	bedroom->west = outhouse;
	outhouse->east = bedroom;
	
	// Start the map and the character off in the hall:
	map->start = hall;
	map->location = hall;
	return 1;
}
Object MapProto = {
	.init = Map_init,
	.move = Map_move,
	.attack = Map_attack
};

int main(int argc, char *argv[])
{
	// Simple way to set up the randomness:
	srand(time(NULL));
	// Make our map:
	Map *game = NEW(Map, "THE HALL OF THE MINOTAUR");
	printf("Welcome to... %s!\n", game->_(description));
	printf("You enter the ");
	game->location->_(describe)(game->location);
	while(process_input(game)) {
	}
	return 0;
}
