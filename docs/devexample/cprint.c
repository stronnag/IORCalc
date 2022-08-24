#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "iorext.h"

int main(int argc, char **argv) {
  if(argc > 1) {
    ior_rec_t* u = allocate_ior_rec();
    calc_rec_t* c = allocate_calc_rec();
    int fd = open(argv[1],O_RDONLY);
    if(fd > 0) {
      u_char hdr[6];
      size_t n = read(fd, hdr, sizeof(hdr));
      if(n == 6 && strcmp((const char*)hdr, "ior88") == 0) {
        n = read(fd, u, sizeof(ior_rec_t));
        if (n > 0) {
          char *s;
          s = calc_ior(u, c);
          printf("%s\n", s);
          char *ofn = strdup(argv[1]);
          if((s = rindex(ofn,'.')) != NULL) {
            strcpy(s+1,"txt");
          } else {
            free(ofn);
            ofn = NULL;
          }
          pcert(u,c, ofn, 1);
        }
      }
    }
    free(u);
    free(c);
  }
  return 0;
}
