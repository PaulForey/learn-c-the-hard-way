#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "game.h"

int Monster_attack (void *self, int damage)
{
	assert(self);
	assert(damage >= 0);

	Monster *monster = self;

	if(monster->hit_points <= 0) {
		printf("The %s is already dead.\n",
				monster->_(description));
		return 1;
	}

	if(damage == 0) {
		printf("You attack the %s, but you miss!\n",
				monster->_(description));
	} else {
		printf("You attack the %s for %d damage!\n",
				monster->_(description), damage);
		monster->hit_points -= damage;
	}

	if(monster->hit_points > 0) {
		printf("It is still alive, with %d hit points left.\n",
				monster->hit_points);
		return 0;
	} else {
		printf("It is dead!\n");
		return 1;
	}
}
int Monster_init (void *self)
{
	assert(self);

	Monster *monster = self;
	monster->hit_points = 10;
	return 1;
}
Object MonsterProto = {
	.init = Monster_init,
	.attack = Monster_attack
};

void *Room_move(void *self, Direction direction)
{
	assert(self);
	assert(direction >= 0);

	Room *room = self;
	Room *next = NULL;
	if(direction == NORTH && room->north) {
		printf("You go north, into the ");
		next = room->north;
	} else if(direction == SOUTH && room->south) {
		printf("You go south, into the ");
		next = room->south;
	} else if(direction == EAST && room->east) {
		printf("You go east, into the ");
		next = room->east;
	} else if(direction == WEST && room->west) {
		printf("You go west, into the ");
		next = room->west;
	} else {
		printf("You can't go that direction.\n");
		next = NULL;
	}
	if(next) {
		next->_(describe)(next);
	}
	return next;
}
int Room_attack(void *self, int damage)
{
	assert(self);
	assert(damage >= 0);

	Room *room = self;
	Monster *monster = room->bad_guy;
	if(monster) {
		monster->_(attack)(monster, damage);
		return 1;
	} else {
		printf("You flail in the air at nothing. Idiot.\n");
		return 0;
	}
}
Object RoomProto = {
	.move = Room_move,
	.attack = Room_attack
};

char get_one_char()
{
	char ch;
	char first;

	ch = getchar();
	first = ch;
	while (ch != '\n' && ch != EOF) {
		ch = getchar();
	}

	return first;
}

void print_possible_rooms(Room* location)
{
	// Hopefully there's a better way to do this,
	// but grammar is annoying so maybe there isn't.
	int room_count = 0;

	// First I count how many rooms I've got:
	if(location->north) room_count += 1;
	if(location->south) room_count += 1;
	if(location->east) room_count += 1;
	if(location->west) room_count += 1;

	// The opening statement:
	printf("You can go ");

	// If there's only one room, it's easy:
	if(room_count == 1) {
		if(location->north) printf("NORTH.\n");
		else if(location->south) printf("SOUTH.\n");
		else if(location->east) printf("EAST.\n");
		else if(location->west) printf("WEST.\n");
	}
	// More than one room is where the fun starts:
	else {
		if(location->north) {
			if(room_count == 2) printf("NORTH or ");
			else printf("NORTH, ");
			room_count--;
		}
		if(location->south) {
			if(room_count == 1) printf("SOUTH.\n");
			else if(room_count == 2) printf("SOUTH or ");
			else printf("SOUTH, ");
			room_count--;
		}
		if(location->east) {
			if(room_count == 1) printf("EAST.\n");
			else printf("EAST or ");
			room_count--;
		}
		if(location->west) {
			printf("WEST.\n");
			room_count--;
		}
		// This seems like a sensible idea:
		assert(room_count == 0);
	}
}

void print_current_monsters(Room* location)
{
	if(location->bad_guy) {
		if(location->bad_guy->proto.description[0] == 'a' ||
		  location->bad_guy->proto.description[0] == 'e' ||
		  location->bad_guy->proto.description[0] == 'i' ||
		  location->bad_guy->proto.description[0] == 'o' ||
		  location->bad_guy->proto.description[0] == 'u') {
			printf("There's an %s in here!\n",
						location->bad_guy->proto.description);
		} else {
			printf("There's a %s in here!\n",
						location->bad_guy->proto.description);
		}
	}
}

void look_around(Room* location)
{
	// First describe the room:
	printf("You are in the %s.\n", location->proto.description);
	// Then mention any monsters that are around:
	print_current_monsters(location);
	// Finally, print the possible locations:
	print_possible_rooms(location);
}

		




int process_input(Player *game)
{
	printf("> ");
	char ch = get_one_char();
	int damage = rand() % 4;
	switch(ch) {
		case 'q':
			printf("Giving up? You suck.\n");
			return 0;
			break;
		case 'n':
			game->_(move)(game, NORTH);
			break;
		case 's':
			game->_(move)(game, SOUTH);
			break;
		case 'e':
			game->_(move)(game, EAST);
			break;
		case 'w':
			game->_(move)(game, WEST);
			break;
		case 'a':
			game->_(attack)(game, damage);
			break;
		case 'l':
			look_around(game->location);
			break;
		case 'h':
			printf("Available actions:\n");
			printf("\tl: Look around.\n");
			printf("\tn: Go NORTH.\n");
			printf("\ts: Go SOUTH.\n");
			printf("\te: Go EAST.\n");
			printf("\tw: Go WEST.\n");
			printf("\ta: Attack whatever's in the room.\n");
			printf("\th: Show this help screen.\n");
			printf("\tq: Quit the game.\n");
			break;
		default:
			printf("What? (%d)\n(type 'h' for help)\n", ch);
	}
	return 1;
}
