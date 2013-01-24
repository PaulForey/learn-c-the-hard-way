#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Person {
	char *name;
	int age;
	int height;
	int weight;
};

void Person_destroy(struct Person who) {
	//assert(who != NULL);
	//free(who.name);
	printf("\t%s is at memory location: %p\n", who.name, &who);
}

void Person_print(struct Person who) {
	printf("Name: %s\n",     who.name);
	printf("\tAge: %d\n",    who.age);
	printf("\tHeight: %d\n", who.height);
	printf("\tWeight: %d\n", who.weight);
	printf("\t%s is at memory location: %p\n", who.name, &who);
}

int main(int argc, char *argv[]) {
	// Make two people:
	struct Person joe = {"Joe Alex", 32, 64, 140};
	struct Person frank = {"Frank Blank", 20, 72, 180};
	// Print them out and where they are in memory:
	printf("Joe is at memory location: %p\n", &joe);
	Person_print(joe);
	printf("Frank is at memory location: %p\n", &frank);
	Person_print(frank);
	// Make everyone age 20 years and print them:
	joe.age += 20;
	joe.height -= 2;
	joe.weight += 40;
	Person_print(joe);
	frank.age += 20;
	frank.weight += 20;
	Person_print(frank);
	// Destroy both!
	Person_destroy(joe);
	Person_destroy(frank);
	return 0;
}

