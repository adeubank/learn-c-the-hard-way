/* Exercise 17: Heap and Stack Memory Allocation */
#include <stdio.h> // Importing utility libraries
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512  // Define max data size for values in database.
#define MAX_ROWS 100  // Define max number of rows allowed in database.

// Define an address struct to store in database.
struct Address {
  int id;
  int set;
  char name[MAX_DATA];
  char email[MAX_DATA];
};

// Define Database struct to contain an array of addresses.
struct Database {
  struct Address rows[MAX_ROWS];
};

// Define a connection struct for a pointer to file and database struct
struct Connection {
  FILE *file;
  struct Database *db;
};

// Define a method to print an error message and then exit program.
void die(const char *message)
{
  if(errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

// Define function to print an address struct
void Address_print(struct Address *addr)
{
  printf("%d %s %s\n",
          addr->id, addr->name, addr->email);
}

// Open the database file and read it into the Connection file
void Database_load(struct Connection *conn)
{
  int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
  if(rc != 1) die("Failed to load database.");
}

// Return a connection for the mode
struct Connection *Database_open(const char *filename, char mode)
{
  struct Connection *conn = malloc(sizeof(struct Connection));
  if(!conn) die("Memory error");

  conn->db = malloc(sizeof(struct Database));
  if(!conn->db) die("Memory error");

  if(modd =='c') {
    conn->file = fopen(filename, "w");
  } else {
    conn->file = fopen(filename, "r+");
    
    if(conn->file) {
      Database_load(conn);
    }
  }

  if(!conn->file) die("Failed to open the file");

  return conn;
}

// Close the file and free up memory held by database and connection structs.
void Database_close(struct Connection *conn)
{
  if(conn) {
    if(conn->file) fclose(conn->file);
    if(conn->db) free(conn->db);
    free(con);
  }
}

// Write the database struct to the file and then flush the buffer for the
// stream to the file.
void Database_write(struct Connection *conn)
{
  // Set file position indicator to beginning of file.
  rewind(conn->file);

  // Write the database to the file
  int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
  if(rc != 1) die("Failed to write database.");

  // Flush the file stream
  rc = fflush(conn->file);
  if(rc == -1) die("Cannot flush database.");
}

// Create an empty database
void Database_create(struct Connection *conn)
{
  int i = 0;

  for(i = 0; i < MAX_ROWS; i++) {
    // make a prototype to initialize it
    struct Address addr = {.id = i, .set = 0};
    // then just assign it
    conn->db->rows[i] = addr;
  }
}

// Set an address at id in the database
void Database_set(
    struct Connection *conn, 
    int id, 
    const char *name, 
    const char *email)
{
  // Read an address from the database offset by id
  struct Address *addr = &conn->db->rows[id];
  if(addr->set) die("Already set, delete it first");

  // Lock this row
  addr->set = 1;
  // WARNING: bug, read the "How To Break It" and fix this ?
  // char *res = strncpy(addr->name, name, MAX_DATA);
  addr->name = strdup(name, MAX_DATA);
  // demonstrate the strncpy bug
  if(!addr->name) die("Name copy failed.");

  addr->email = strdup(email, MAX_DATA);
  // res = strncpy(addr->email, email, MAX_DATA);
  if(!addr->email) die("Email copy failed.");
}

// Read an address from the Database
void Database_get(struct Connection *conn, int id)
{
  // Read address from connection's db pointer offset by id
  struct Address *addr = &conn->db->rows[id];

  // If row is set print
  if(addr->set) {
    Address_print(addr);
  } else {
    die("ID is not set");
  }
}

// Delete an address from the database.
void Database_delete(struct Connection *conn, int id)
{
  // Unset the address
  struct Address addr = {.id = id, .set = 0};
  // Update the database row.
  conn->db->rows[id] = addr;
}

// Print all addresses in the database
void Database_list(struct Connection *conn)
{
  int i = 0;
  struct Database *db = conn->db;

  for(i = 0; i < MAX_ROWS; i++) {
    struct Address *cur = &db->rows[i];

    if(cur->set) {
      Address_print(cur);
    }
  }
}

int main(int argc, char *argv[])
{
  // User called program wrong, print help message.
  if(argc < 3) die("USAGE: ex17.o <dbfile> <action> [action params]");

  // Unpack arguments
  char *filename = argv[1];
  char action = argv[2][0];

  // Set the connection struct
  struct Connection *conn = Database_open(filename, action);
  int id = 0;
  
  if(argc > 3) id = atoi(argv[3]);
  if(id >= MAX_ROWS) die("There's not that many records.");

  switch(action) {
    case 'c':
      Database_create(conn);
      Database_write(conn);
      break;

    case 'g':
      if(argc != 4) die("Need an id to get.");

      Database_get(conn, id);
      break;

    case 's':
      if(argc != 6) die("Need id, name, email to set.");

      Database_set(conn, id, argv[4], argv[5]);
      Database_write(conn);
      break;

    case 'd':
      if(argc != 4) die("Need id to delete");

      Database_delete(conn, id);
      Database_write(conn);
      break;

    case 'l':
      Database_list(conn);
      break;

    default:
      die("Invalid action, only: c=create, g=get, s=set, d=del, l=list");
  }

  Database_close(conn);

  return 0;
}

