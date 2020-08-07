#include <stdbool.h>
#include <stdio.h>

// printf("%2d %2d %2d %s\n",  3,  3,  10, ".gitignore");
// printf("%3d %3d %3d %s\n", 17, 32, 180, "Makefile");
// printf("%3d %3d %3d %s\n", 20, 35, 190, "total");

int total_nlines = 0;
int total_nwords = 0;
int total_nbytes = 0;

static void wc(FILE *f, char *path) {
  int nlines = 0;
  int nwords = 0;
  int nbytes = 0;
  bool in_word = false;

  int c;
  while ((c = fgetc(f)) != EOF) {
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

  total_nlines += nlines;
  total_nwords += nwords;
  total_nbytes += nbytes;

  if (path != NULL) {
    printf("%4d %4d %4d %s\n", nlines, nwords, nbytes, path);
  } else {
    printf("%7d %7d %7d\n", nlines, nwords, nbytes);
  }
}

int main(int argc, char **argv) {
  char **args = argv + 1;

  if (argc == 1) {
    wc(stdin, NULL);
    return 0;
  }

  for (char **p = args; *p != NULL; p++) {
    FILE *f = fopen(*p, "r");

    wc(f, *p);
    fclose(f);
  }

  if (argc >= 3) {
    printf("%4d %4d %4d %s\n", total_nlines, total_nwords, total_nbytes,
           "total");
  }

  return 0;
}
