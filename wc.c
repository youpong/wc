#include "util.h"
#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

char *progname;

int total_nlines = 0;
int total_nwords = 0;
int total_nbytes = 0;

typedef struct {
  int nlines;
  int nwords;
  int nbytes;
  char *path;
} WC;

static int log10i(int x) {
  int log = 0;

  for (; x >= 10; log++) {
    x /= 10;
  }

  return log;
}

static bool is_dir(char *path) {
  bool ret;

  DIR *dir = opendir(path);
  ret = dir != NULL ? true : false;
  closedir(dir);

  return ret;
}

static FILE *open_file(char *path) {
  if (is_dir(path)) {
    fprintf(stderr, "%s: %s: is directory\n", progname, path);
    return NULL;
  }

  FILE *f = fopen(path, "r");
  if (f == NULL) {
    fprintf(stderr, "%s: %s: %s\n", progname, path, strerror(errno));
    return NULL;
  }

  return f;
}

static char *gen_fmt(int nbytes) {
  char *ret = calloc(sizeof(char), 15 + 1);

  int n = log10i(nbytes) + 1;
  sprintf(ret, "%%%dd %%%dd %%%dd %%s\n", n, n, n);

  return ret;
}

static WC *new_wc(int nlines, int nwords, int nbytes, char *path) {
  WC *wc = malloc(sizeof(WC));

  wc->nlines = nlines;
  wc->nwords = nwords;
  wc->nbytes = nbytes;
  wc->path = path;

  total_nlines += nlines;
  total_nwords += nwords;
  total_nbytes += nbytes;

  return wc;
}

static WC *wc(FILE *f, char *path) {
  /** white-space characters in locale "C" and "POSIX" */
  char *white_space = " \f\n\r\v\t";
  int nlines = 0;
  int nwords = 0;
  int nbytes = 0;
  bool in_word = false;

  int c;
  while ((c = fgetc(f)) != EOF) {
    nbytes++;

    if (c == '\n')
      nlines++;

    if (strchr(white_space, c) != NULL) {
      in_word = false;
      continue;
    }

    if (in_word == false) {
      in_word = true;
      nwords++;
    }
  }

  return new_wc(nlines, nwords, nbytes, path);
}

static void print_wc(WC *wc) {
  if (wc->path != NULL) {
    char *fmt = gen_fmt(total_nbytes);
    printf(fmt, wc->nlines, wc->nwords, wc->nbytes, wc->path);
  } else {
    printf("%7d %7d %7d\n", wc->nlines, wc->nwords, wc->nbytes);
  }
}

int main(int argc, char **argv) {
  char **args = argv + 1;
  int ret = EXIT_SUCCESS;

  progname = argv[0];

  if (argc == 1) {
    print_wc(wc(stdin, NULL));
    return 0;
  }

  Vector *vec = new_vector();

  for (char **p = args; *p != NULL; p++) {
    FILE *f = open_file(*p);
    if (f == NULL) {
      ret = EXIT_FAILURE;
      continue;
    }
    vec_push(vec, wc(f, *p));

    fclose(f);
  }

  for (int i = 0; i < vec->len; i++) {
    print_wc(vec->data[i]);
  }

  if (argc >= 3) {
    char *fmt = gen_fmt(total_nbytes);
    printf(fmt, total_nlines, total_nwords, total_nbytes, "total");
  }

  return ret;
}
