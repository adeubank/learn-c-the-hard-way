/* Exercise 17: Heap and Stack Memory Allocation */
#include <stdio.h> // Importing utility libraries
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

// Define an address struct to store in database.
struct Address {
  int id;
  int set;
  char *name;
  char *email;
};

// Define Database struct to contain an array of addresses.
struct Database {
  long max_data;
  long max_rows;
  struct Address *rows;
};

// Define a connection struct for a pointer to file and database struct
struct Connection {
  FILE *file;
  struct Database *db;
};

static struct Connection conn = {NULL, NULL};

void Database_close();

// Define a method to print an error message and then exit program.
void die(const char *message)
{
  // Close and clean up
  Database_close();

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
void Database_load(void)
{
  struct Database *db = conn.db;
  struct Address *addr;

  if (fread(&db->max_data, sizeof(db->max_data), 1, conn.file) != 1)
    die("Failed to load database.");
  if (fread(&db->max_rows, sizeof(db->max_rows), 1, conn.file) != 1)
    die("Failed to load database.");

  db->rows = malloc(db->max_rows * sizeof(struct Address));
  if (!db->rows) die("Memory error.");

  for (addr = db->rows; addr < db->rows + db->max_rows; ++addr) {
    if (fread(&addr->id, sizeof(addr->id), 1, conn.file) != 1)
      die("Failed to load database.");
    if (fread(&addr->set, sizeof(addr->set), 1, conn.file) != 1)
      die("Failed to load database.");

    addr->name = malloc(db->max_data);
    if (!addr->name) die("Memory error.");
    if (fread(addr->name, db->max_data, 1, conn.file) != 1)
      die("Failed to load database.");

    addr->email = malloc(db->max_data);
    if (fread(addr->email, db->max_data, 1, conn.file) != 1)
      die("Failed to load database.");
  }
}

// Return a connection for the mode
void Database_open(const char *filename, char mode)
{
  conn.db = malloc(sizeof(struct Database));
  if(!conn.db) die("Memory error");

  if(mode =='c') {
    conn.file = fopen(filename, "w");
  } else {
    conn.file = fopen(filename, "r+");
    
    if(conn.file) {
      Database_load();
    }
  }

  if(!conn.file) die("Failed to open the file");
}

// Close the file and free up memory held by database and connection structs.
void Database_close(void)
{
  struct Database *db;

  if (conn.file) fclose(conn.file);
  if ((db = conn.db)) {
    if (db->rows) {
      struct Address *addr;
      for (addr = db->rows;
            addr < db->rows + db->max_rows;
            ++addr) {
        if (addr->name) free(addr->name);
        if (addr->email) free(addr->email);
      }
      free(db->rows);
    }
    free(db);
  }
}

// Write the database struct to the file and then flush the buffer for the
// stream to the file.
void Database_write()
{
  struct Database *db = conn.db;
  struct Address *addr;

  // Set file position indicator to beginning of file.
  rewind(conn.file);

  if (fwrite(&db->max_data, sizeof(db->max_data), 1, conn.file) != 1)
    die("Failed to write database");
  if (fwrite(&db->max_rows, sizeof(db->max_rows), 1, conn.file) != 1)
    die("Failed to write database");

  for(addr = db->rows; addr < db->rows + db->max_rows; ++addr)
    if (fwrite(&addr->id, sizeof(addr->id), 1, conn.file) != 1 ||
          fwrite(&addr->set, sizeof(addr->set), 1, conn.file) != 1 ||
          fwrite(&addr->name, db->max_data, 1, conn.file) != 1 ||
          fwrite(&addr->email, db->max_data, 1, conn.file) != 1)
      die("Failed to write database.");

  // Flush the file stream
  if (fflush(conn.file) == -1) die("Cannot flush database.");
}

// Create an empty database
void Database_create(long max_rows, long max_data)
{
  int i = 0;
  struct Database *db = conn.db;
  struct Address *addr;

  db->rows = malloc(db->max_rows * sizeof(struct Address));
  if (!db->rows) die("Memory error.");

  for(i = 0; i < db->max_rows; i++) {
    addr = &db->rows[i];
    addr->id = i;
    addr->set = 0;

    addr->name = malloc(db->max_data);
    if(!addr->name) die("Memory error.");
    addr->name[0] = '\0';


    addr->email = malloc(db->max_data);
    if(!addr->email) die("Memory error.");
    addr->email[0] = '\0';
  }
}

// Set an address at id in the database
void Database_set(int id, const char *name, const char *email)
{
  struct Database *db = conn.db;

  // Read an address from the database offset by id
  struct Address *addr = &db->rows[id];
  if(addr->set) die("Already set, delete it first.");

  // Lock this row
  addr->set = 1;
  char *res = strncpy(addr->name, name, db->max_data);
  if(!res) die("Name copy failed.");
  res[db->max_data-1] = '\0';

  res = strncpy(addr->email, email, db->max_data);
  if(!res) die("Email copy failed.");
  res[db->max_data-1] = '\0';
}

// Read an address from the Database
void Database_get(int id)
{
  // get the i element of rows, which is in db, which is in conn, 
  // then get the address of (&) it
  struct Address *addr = &conn.db->rows[id];

  // If row is set print
  if(addr->set) {
    Address_print(addr);
  } else {
    die("ID is not set");
  }
}

// Delete an address from the database.
void Database_delete(int id)
{
  // Unset the address
  struct Address *addr = &conn.db->rows[id];
  addr->set = 0;
  addr->name[0] = '\0';
  addr->email[0] = '\0';
}

// Print all addresses in the database
void Database_list()
{
  struct Database *db = conn.db;
  int i = 0;

  for(i = 0; i < db->max_rows; i++) {
    struct Address *cur = &db->rows[i];

    if(cur->set) {
      Address_print(cur);
    }
  }
}

int is_valid_ref(char *);
int is_valid_range(const char *);

int main(int argc, char *argv[])
{
  // User called program wrong, print help message.
  if(argc < 3) die("USAGE: ex17.o <dbfile> <action> [action params]");

  char *filename = argv[1];
  char action = argv[2][0];
  Database_open(filename, action);

  long id, id_start, id_end;
  
  /* Check validity of argumnets */
  if(argc > 3) {
    switch(action) {
      case 'c':
        if ((argc >= 4 && !is_valid_ref(argv[3])) ||
              (argc >= 5 && !is_valid_ref(argv[4])))
          die("Invalid database size parameters.");
        break;

      case 'g': /* g and d have the same requirements */
      case 'd':
        if (strchr(argv[4], '-')) {
          if (!is_valid_range(argv[3]))
            die("Invalid id range.");
          sscanf(argv[3], "%ld-%ld", &id_start, &id_end);
        } else {
          if (!is_valid_ref(argv[3]))
            die("Invliad id");
          id_start = id_end = strol(argv[3], NULL, 10);
        }
        id_start = MAX(id_start, 0);
        id_end = MIN(id_end, conn.db->max_rows);

        break;
      case 's':
        if (!is_valid_ref(argv[3]))
          die("Invalid id.");
        id = MIN(strtol(argv[3], NULL, 10), conn.db->max_rows);
        break;
      default: /* for actions that don't take ids or ranges */
        break;
    }
  }

  long max_data, max_rows;

  switch(action) {
    case 'c':
      max_data = (argc >= 4) ? strtol(argv[3], NULL, 10) : 512;
      max_rows = (argc >= 5) ? strtol(argv[4], NULL, 10) : 100;
      Database_create(max_data, max_rows);
      Database_write();
      break;

    case 'g':
      if (argc < 4) die("Need an id or id range to get.");

      for(id = id_start; id <= id_end; ++id)
        Database_get(id);
      break;

    case 's':
      if (argc < 6) die("need id, name, email, to set.");

      Database_set(id, argv[4], argv[5]);
      Database_write();
      break;

    case 'd':
      if (argc < 4) die("Need id or id range to delete.");

      for(id = id_start; id <= id_end; ++id)
        Database_delete(id);
      Database_write();
      break;

    case 'l':
      Database_list();
      break;

    case 'f':
      if (argc < 4) die("Need string to search for.");

      Database_find(argv[3]);
      break;

    default:
      die("Invalid action, only: c=create, g=get, s=set, d=del, l=list, f=find");
      break;
  }

  Database_close();

  return 0;
}

