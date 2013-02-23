#ifndef _game_h
#define _game_h
#include "object.h"

typedef struct Monster {
	Object proto;
	int hit_points;
} Monster;
int Monster_attack(void* self, int damage);
int Monster_init(void* self);
Object MonsterProto;

typedef struct Room {
	Object proto;
	Monster* bad_guy;
	struct Room* north;
	struct Room* south;
	struct Room* east;
	struct Room* west;
} Room;
void* Room_move(void* self, Direction direction);
int Room_attack(void* self, int damage);
int Room_init(void* self);
Object RoomProto;

typedef struct Player {
	Object proto;
	Room* start;
	Room* location;
	char** inventory;
} Player;
void* Player_move(void* self, Direction direction);
int Player_attack(void* self, int damage);
int Player_init(void* self);
Object PlayerProto;

char get_one_char();
int process_input(Player* game);

#endif // _game_h
