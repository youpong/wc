#include <stdbool.h>
#include <stdio.h>

// printf("%2d %2d %2d %s\n",  3,  3,  10, ".gitignore");
// printf("%3d %3d %3d %s\n", 17, 32, 180, "Makefile");
// printf("%3d %3d %3d %s\n", 20, 35, 190, "total");

int main(int argc, char **argv) {
  int nlines = 0;
  int nwords = 0;
  int nbytes = 0;
  bool in_word = false;

  int c;
  while ((c = fgetc(stdin)) != EOF) {
    if (c == '\n')
      nlines++;

    switch (c) {
    case ' ':
    case '\t':
    case '\v':
    case '\n':
      in_word = false;
      break;
    default:
      if (in_word == false) {
        in_word = true;
        nwords++;
      }
    }

    nbytes++;
  }

  printf("%7d %7d %7d\n", nlines, nwords, nbytes);

  return 0;
}
