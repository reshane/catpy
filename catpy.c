/* catpy.c
 * 
 * Author: shane kenny
 * Date: 17-08-2022
 * 
 * Notes: My first time programming :)
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>

#define STD_OUT 1
#define STD_ERR 2

size_t get_str_sz(const char*);

#define SUCCESS_MESSAGE "\nCats can pee:)\n"
#define SUCC_SIZE get_str_sz(SUCCESS_MESSAGE)
#define _ASSERT_MESSAGE(test, message) assert(((void)message, test))

size_t get_str_sz(const char* buffer)
{
  size_t sz = 0;
  while (buffer[sz] != '\0') ++sz;
  return sz;
}

void write_error(const char* error_message)
{
  size_t count = get_str_sz(error_message);
  write(STD_ERR, error_message, count);
}

void write_file(const char* buffer, size_t count)
{
  ssize_t result = write(STD_OUT, buffer, count);
  if (result == -1) {
    write_error("Error: Cat cannot py because cat has no thumbs!"); 
  }
}

void slurp_file(const char* file_name)
{
  // int open(const char *pathname, int flags);
  int fd = open(file_name, O_RDONLY);
  if (fd == -1) {
    write_error("Error: Cat cannot py because file cannot be opened!\n");
  }
  // find the size of the file at fd
  struct stat statbuf;
  int result = fstat(fd, &statbuf);
  off_t count = statbuf.st_size;
  if (result == -1) {
    write_error("Cat cannot py because it does not know when to stop!");
  }
  // syscall read that number of bytes into a malloced cstr
  // count is the number of bytes that we want to eventually process
#define BUF_CAP 256
  
  char buf[BUF_CAP];
  int unread_bytes = count;
  while (unread_bytes) {
    if (unread_bytes >= BUF_CAP) {
      read(fd, buf, BUF_CAP);
      write_file(buf, BUF_CAP);
      unread_bytes -= BUF_CAP;
    } else {
      read(fd, buf, unread_bytes);
      write_file(buf, unread_bytes);
      unread_bytes = 0;
    }
  }
}

// There are five system calls that generate file descriptors: create, open, fcntl, dup and pipe.
// lseek(2)
// O_RDONLY and not O_CREAT(auto not specified)

int main(int argc, char* argv[])
{
  if (argc < 2 || argc%2 != 0) {
    write_error("Usage: ./catpy <filename>\n");
    return 1;
  }
  if (argc == 2) {
    const char *filename = argv[1];
    slurp_file(filename);
    write_file(SUCCESS_MESSAGE, SUCC_SIZE);
    return 0;
  }

  // we can read them all in and anything that isn't paired with a -* is the filename
  int filename_found = 0, message_len = SUCC_SIZE;
  char *message, *pot_filename;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      // this is a flag and we can switch on the character
      const char c = argv[i][1];
      switch (c) {
        case '\0' :
          _ASSERT_MESSAGE(0, "Usage");
          return 1;
          break;
        case 'm':
          message = argv[++i];
          int j=-1;
          while (message[++j]!='\0');
          message_len = j;
          break;
        case 's':
          message = SUCCESS_MESSAGE;
          break;
      } 
      
    } else {
      pot_filename = argv[i];
      if (access(pot_filename, F_OK) != 0) {
        // the file does not exist
        write_error("Cat cannot py because the cat cannot find a file to py.");
        return 1;
      }
    }
  }
  const char *filename = pot_filename;
  if (access(filename, F_OK) != 0) {
    // the file does not exist
    write_error("Cat cannot py because the cat cannot find a file to py.\n");
    return 1;
  }
  slurp_file(filename);
  write_file("\n", 1);
  write_file(message, message_len);
  write_file("\n", 1);
  return 0;
}
