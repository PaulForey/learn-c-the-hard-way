#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dbg.h"

#define DATA_SIZE 100000

int normal_copy(char* from, char* to, int count)
{
	int i = 0;
	for(i = 0; i < count; i++) {
		to[i] = from[i];
	}
	return i;
}

int duffs_device(char* from, char* to, int count)
{
	int n = (count + 7) / 8;
	switch(count % 8) {
		case 0: do { *to++ = *from++;
					case 7: *to++ = *from++;
					case 6: *to++ = *from++;
					case 5: *to++ = *from++;
					case 4: *to++ = *from++;
					case 3: *to++ = *from++;
					case 2: *to++ = *from++;
					case 1: *to++ = *from++;
				} while(--n > 0);
	}
	return count;
}

int zeds_device(char* from, char* to, int count)
{
	int n = (count + 7) / 8;
	switch(count % 8) {
		case 0:
		again: *to++ = *from++;
		case 7: *to++ = *from++;
		case 6: *to++ = *from++;
		case 5: *to++ = *from++;
		case 4: *to++ = *from++;
		case 3: *to++ = *from++;
		case 2: *to++ = *from++;
		case 1: *to++ = *from++;
				if(--n > 0) goto again;
	}
	return count;
}

int valid_copy(char* data, int count, char expects)
{
	int i = 0;
	for(i = 0; i < count; i++) {
		if(data[i] != expects) {
			log_err("[%d] %c != %c", i, data[i], expects);
			return 0;
		}
	}
	return 1;
}

int main(int argc, char* argv[])
{
	char from[DATA_SIZE] = {'a'};
	char to[DATA_SIZE] = {'c'};
	int rc = 0;
	time_t time1;
	time_t time2;

	// Give the from some stuff:
	memset(from, 'x', DATA_SIZE);
	// Give the to some stuff and make sure it's right:
	memset(to, 'y', DATA_SIZE);
	check(valid_copy(to, DATA_SIZE, 'y'), "Not initialized right.");

	// Use normal copy:
	time1 = time(NULL);
	rc = normal_copy(from, to, DATA_SIZE);
	check(rc == DATA_SIZE, "Normal copy failed: %d", rc);
	check(valid_copy(to, DATA_SIZE, 'x'), "Normal copy failed.");

	// Reset it and then use Duff's Device:
	memset(to, 'y', DATA_SIZE);
	rc = duffs_device(from, to, DATA_SIZE);
	check(rc == DATA_SIZE, "Duff's copy failed: %d", rc);
	check(valid_copy(to, DATA_SIZE, 'x'), "Duff's copy failed.");

	// And then Zed's device:
	memset(to, 'y', DATA_SIZE);
	rc = zeds_device(from, to, DATA_SIZE);
	check(rc == DATA_SIZE, "Zed's copy failed: %d", rc);
	check(valid_copy(to, DATA_SIZE, 'x'), "Zed's copy failed.");
	time2 = time(NULL);
	log_info("Time 1: %ld, Time 2: %ld, Time Diff: %f",
			time1, time2, difftime(time1, time2));
	return 0;
error:
	return 1;
}


