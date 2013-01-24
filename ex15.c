#include <stdio.h>

int main (int argc, char *argv[])
{
	int ages[] = {23, 43, 12, 89, 2};
	char *names[] = {
		"Alan", "Frank",
		"Mary", "John", "Lisa"
	};
	// Get the size of ages:
	int count = sizeof(ages)/sizeof(int);
	int i;
	// Method 1 -- Indexing:
	for (i = 0; i < count; i++) {
		printf("%s has %d years alive.\n",
				names[i], ages[i]);
	}
	printf("---\n");
	// Set up pointers to start of array:
	int *cur_age = ages;
	char **cur_name = names;
	// Method 2 -- Pointers:
	for (i = 0; i < count; i++) {
		printf("%s is %d years old.\n",
				*(cur_name + i), *(cur_age + i));
	}
	printf("---\n");
	// Method 3 -- Pointers as arrays:
	for (i = 0; i < count; i++) {
		printf("%s is %d years old again.\n",
				cur_name[i], cur_age[i]);
	}
	printf("---\n");
	// Method 4 -- Stupid complex way:
	for (cur_name = names, cur_age = ages;
			(cur_age - ages) < count;
			cur_name++, cur_age++)
	{
		printf("%s lived %d years so far.\n",
				*cur_name+9, *(cur_age+2));
	}
	return 0;
}
