#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_CAP 65536

int main(void)
{
  int fd;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  char *filename = "long_test.file";
  fd = creat(filename, mode);
  char buf[BUF_CAP];
  for (int i = 0; i < BUF_CAP; ++i) {
    buf[i] = (char)(65 + (i%26));
  }
  write(fd, buf, BUF_CAP);
  close(fd);
  return 0;
}
