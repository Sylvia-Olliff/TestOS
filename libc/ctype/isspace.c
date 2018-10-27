#include <ctype.h>

int isspace (int c) {
  const unsigned char test = (unsigned char) c;
  if (test == ' ' || test == '\f' || test == '\n' || test == '\r' || test == '\t' || test == '\v') return 1;
  else return 0;
}
