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

#define STD_OUT 1
#define STD_ERR 2

size_t get_str_sz(const char*);

#define SUCCESS_MESSAGE "\nCats can pee!\n"
#define SUCC_SIZE get_str_sz(SUCCESS_MESSAGE)

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
  char *buf = malloc(count);
  ssize_t bytes_read = read(fd, buf, count);
  // pass the string to writefile
  const char *buffer = buf;
  write_file(buffer, count);
  write_file(SUCCESS_MESSAGE, SUCC_SIZE);
}

// There are five system calls that generate file descriptors: create, open, fcntl, dup and pipe.
// lseek(2)
// O_RDONLY and not O_CREAT(auto not specified)

int main(int argc, char* argv[])
{
  if (argc < 2) {
    write_error("Usage: ./catpy <filename>\n");
    return 1;
  }
  // here we want to store file_name in a const char* and pass it to slurp_file
  const char *filename = argv[1];
  slurp_file(filename);
  return 0;
}
