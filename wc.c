#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int total_nlines = 0;
int total_nwords = 0;
int total_nbytes = 0;

typedef struct {
  int nlines;
  int nwords;
  int nbytes;
  char *path;
} WC;

static int digit_num(int nbytes) {
  int num = 0;

  while (nbytes > 0) {
    nbytes /= 10;
    num++;
  }

  return num;
}

static char *gen_fmt(int nbytes) {
  char *ret = calloc(sizeof(char), 15 + 1);
  int n = digit_num(nbytes);
  sprintf(ret, "%%%dd %%%dd %%%dd %%s\n", n, n, n);
  return ret;
}

static WC *new_wc(int nlines, int nwords, int nbytes, char *path) {
  WC *wc = malloc(sizeof(WC));

  wc->nlines = nlines;
  wc->nwords = nwords;
  wc->nbytes = nbytes;
  wc->path = path;

  return wc;
}

static WC *wc(FILE *f, char *path) {
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

  // TODO: refactor
  total_nlines += nlines;
  total_nwords += nwords;
  total_nbytes += nbytes;

  return new_wc(nlines, nwords, nbytes, path);
}

static void print_wc(WC *wc) {
  if (wc->path != NULL) {
    char *fmt = gen_fmt(wc->nbytes);
    printf(fmt, wc->nlines, wc->nwords, wc->nbytes, wc->path);
  } else {
    printf("%7d %7d %7d\n", wc->nlines, wc->nwords, wc->nbytes);
  }
}

int main(int argc, char **argv) {
  char **args = argv + 1;

  if (argc == 1) {
    WC *w = wc(stdin, NULL);
    print_wc(w);
    return 0;
  }

  for (char **p = args; *p != NULL; p++) {
    FILE *f = fopen(*p, "r");

    WC *w = wc(f, *p);
    print_wc(w);
    fclose(f);
  }

  if (argc >= 3) {
    printf("%4d %4d %4d %s\n", total_nlines, total_nwords, total_nbytes,
           "total");
  }

  return 0;
}
