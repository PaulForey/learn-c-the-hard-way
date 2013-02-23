#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "dbg.h"

struct Person {
	char* name;
	int age;
	float height;
	float weight;
};

struct Person* Person_create(char *name, int age,
						float height, float weight) {
	check(name, "No name supplied to Person_create.");
	check(age >= 0, "Age shouldn't be negative.");
	check(height >= 0.0, "Height should be positive.");
	check(weight >= 0.0, "Weight should be positive.");

	struct Person* who = malloc(sizeof(struct Person));
	check(who, "Person not successfully allocated!");
	who->name = strdup(name);
	check(who->name, "Person's name not succesfully allocated.");
	who->age = age;
	who->height = height;
	who->weight = weight;
	return who;
error:
	return NULL;
}

int Person_destroy(struct Person* who) {
	check(who, "Can't destroy person: No person to destroy.");
	if(who->name) free(who->name);
	free(who);
	return 0;
error:
	return -1;
}

int Person_get_age(struct Person* who) {
	return who->age;
}

int Person_set_age(struct Person* who, int age) {
	check(age >= 0, "Age should be positive: %d", age);
	who->age = age;
	return 0;
error:
	return -1;
}

float Person_get_height(struct Person* who) {
	return who->height;
}

int Person_set_height(struct Person* who, float height) {
	check(height >= 0, "Height should be positive: %f", height);
	who->height = height;
	return 0;
error:
	return -1;
}

float Person_get_weight(struct Person* who) {
	return who->weight;
}

int Person_set_weight(struct Person* who, float weight) {
	check(weight >= 0, "Weight should be positive: %f", weight);
	who->weight = weight;
	return 0;
error:
	return -1;
}

void Person_print(struct Person* who) {
	printf("Name: %s\n", who->name);
	printf("\tAge: %d\n", who->age);
	printf("\tHeight: %f\n", who->height);
	printf("\tWeight: %f\n", who->weight);
}

int main(int argc, char* argv[]) {
	int rc = 0;
	// Make two people:
	struct Person* joe = Person_create(
			"Joe Alex", 32, 64, 90.6);
	check(joe, "Joe not succesfully made.");
	struct Person* frank = Person_create(
			"Frank Blank", 20, 72, 100);
	check(frank, "Frank not succesfully made.");
	// Print them out and where they are in memory:
	printf("Joe is at memory location: %p\n", joe);
	Person_print(joe);
	printf("Frank is at memory location: %p\n", frank);
	Person_print(frank);
	// Make everyone age 20 years and print them:
	rc = Person_set_age(joe, Person_get_age(joe) + 20);
	check(rc == 0, "Couldn't set Joe's age.");
	rc = Person_set_height(joe, Person_get_height(joe) - 15.5);
	check(rc == 0, "Couldn't set Joe's height.");
	rc = Person_set_weight(joe, Person_get_weight(joe) + 10.8);
	check(rc == 0, "Couldn't set Joe's weight.");
	Person_print(joe);
	rc = Person_set_age(frank, Person_get_age(frank) + 20);
	check(rc == 0, "Couldn't set Frank's age.");
	rc = Person_set_weight(frank, Person_get_weight(frank) + 20.3);
	Person_print(frank);
	// Destroy both!
	rc = Person_destroy(joe);
	check(rc == 0, "Couldn't destroy Joe.");
	rc = Person_destroy(frank);
	check(rc == 0, "Couldn't destroy Frank.");
	return 0;
error:
	return -1;
}

