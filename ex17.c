#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "dbg.h"

//#define MAX_DATA 512
//#define MAX_ROWS 100

struct Address {
	int id;
	int set;
	char* name;
	char* email;
};

struct Database {
	int max_data;
	int max_rows;
	struct Address* rows;
};

struct Connection {
	FILE* file;
	struct Database* db;
};

// A quick function declaration
void Database_close(struct Connection *conn);

void Address_print(struct Address *addr)
{
	printf("%d %s %s\n",
			addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
	printf("size: %d\n", sizeof(*conn->db));
	int rc = fread(conn->db, sizeof(*conn->db), 1, conn->file);
error:
	if(conn->db->rows) free(conn->db->rows);
	if(conn->db) free(conn->db);
	return -1;
}

struct Connection *Database_open(const char *filename, char mode,
								int max_rows, int max_data)
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn) die(conn, "Memory error.");
	conn->db = malloc(sizeof(struct Database));
	if(!conn->db) die(conn, "Memory error.");

	if(mode == 'c') {
		conn->file = fopen(filename, "w");
	} else {
		conn->file = fopen(filename, "r+");
		if(conn->file) {
			Database_load(conn);
		}
	}
	if(!conn->file)
		die(conn, "Failed to open the file.");
	printf("Database opened.\n");
	return conn;
}

void Database_close(struct Connection *conn)
{
	if(conn) {
		if(conn->file)
			fclose(conn->file);
		if(conn->db)
			free(conn->db);
		free(conn);
	}
	printf("Database closed.\n");
}

void Database_write(struct Connection *conn)
{
	printf("Writing database...\n");
	rewind(conn->file);
	int rc = fwrite(&conn->db->max_rows, sizeof(int), 1, conn->file);
	if(rc != 1)
		die(conn, "Failed to write max rows.");
	if(rc == -1)
		die(conn, "Cannot flush max rows.");
	rc = fwrite(&conn->db->max_data, sizeof(int), 1, conn->file);
	if(rc != 1)
		die(conn, "Failed to write max data.");
	if(rc == -1)
		die(conn, "Cannot flush max data.");
	rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1)
		die(conn, "Failed to write database.");
	if(rc == -1)
		die(conn, "Cannot flush database.");
	printf("Database written.\n");
}

void Database_create(struct Connection *conn,
						int max_rows, int max_data)
{
	printf("Creating database...\n");
	int i = 0;
	for(i = 0; i < max_rows; i++) {
		// Make a prototype to initialize it:
		struct Address addr = {.id = i, .set = 0};
		// ...then set it:
		conn->db->rows[i] = addr;
	}
	printf("Database created.\n");
}

void Database_set(struct Connection *conn, int id,
				const char *name, const char *email)
{
	//printf("Setting DB entry...\n");
	struct Address *addr = &conn->db->rows[id];
	if(addr->set)
		die(conn, "DB Address already set, delete it first.");
	// WARNING: This a bug:
	char *res = strncpy(addr->name, name, conn->db->max_data);
	// Demonstrate the strncopy bug:
	if(!res)
		die(conn, "Name copy failed.");
	res = strncpy(addr->email, email, conn->db->max_data);
	if(!res)
		die(conn, "Email copy failed.");
	addr->set = 1;
	//printf("DB entry set\n");
}

void Database_get(struct Connection *conn, int id)
{
	struct Address *addr = &conn->db->rows[id];
	if(addr->set) {
		Address_print(addr);
	} else {
		die(conn, "DB Address is not set");
	}
	printf("DB entry got\n");
}

void Database_delete(struct Connection *conn, int id)
{
	struct Address addr = {.id = id, .set = 0};
	conn->db->rows[id] = addr;
	printf("DB entry deleted\n");
}

void Database_list(struct Connection *conn)
{
	printf("Printing Database...\n");
	int i = 0;
	struct Database *db = conn->db;
	for(i = 0; i < db->max_rows; i++) {
		struct Address *cur = &db->rows[i];
		if(cur->set) {
			//printf("Is set!");
			Address_print(cur);
		}
		//printf("Is not set!");
	}
	printf("...Database printed.\n");
}

void Database_find(struct Connection *conn, char *target)
{
	struct Database *db = conn->db;
	struct Address *addr;
	int i;
	printf("Finding %s...\n", target);
	for(i = 0; i < db->max_rows; i++) {
		addr = &db->rows[i];
		if(addr->set == 1) {
			if(strstr(addr->name, target) || strstr(addr->email, target))
				Address_print(addr);
		}
	}
	printf("All results found.\n");
}

int main(int argc, char* argv[])
{
	if(argc < 3)
		die(NULL, "USAGE: ex17 <dbfile> <action> [action params]");
	char* filename = argv[1];
	char action = argv[2][0];
	struct Connection* conn;
	if(action == 'c')
		conn = Database_open(filename, action,
					atoi(argv[3]), atoi(argv[4]));
	else
		conn = Database_open(filename, action, 0, 0);
	int id = 0;
	switch(action) {
		case 'c':
			if(argc != 5)
				die(conn, "Incorrect params.");
			Database_create(conn, atoi(argv[3]), atoi(argv[4]));
			Database_write(conn);
			break;
		case 'g':
			id = atoi(argv[3]);
			if(id > conn->db->max_rows)
				die(conn, "ID is greater than number of records.");
			if(argc != 4)
				die(conn, "Need ID to get.");
			Database_get(conn, id);
			break;
		case 's':
			id = atoi(argv[3]);
			if(id > conn->db->max_rows)
				die(conn, "ID is greater than number of records.");
			if(argc != 6)
				die(conn, "Need ID, name and email to set.");
			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;
		case 'd':
			id = atoi(argv[3]);
			if(id > conn->db->max_rows)
				die(conn, "ID is greater than number of records.");
			if(argc != 4)
				die(conn, "Need ID to delete.");
			Database_delete(conn, id);
			Database_write(conn);
			break;
		case 'l':
			Database_list(conn);
			break;
		case 'f':
			Database_find(conn, argv[3]);
			break;
		default:
			die(conn,
"Invalid action. c=create, g=get, s=set, d=delete, s=set, f=find.");
	}
	Database_close(conn);
	return 0;
}
