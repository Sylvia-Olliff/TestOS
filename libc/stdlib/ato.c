
#include <ctype.h>

double atof (const char *nptr) {
  int decFound = 0;
  double result = 0.0;
  int neg = 1;

  int restart = 0;

  while(*nptr && isspace(*nptr))
    ++nptr;

  do
  {
    switch (*nptr) {
      case '-':
        neg = -1;
        for (; *nptr != '\0'; ++nptr) {
          if (*nptr == '.') {
            decFound = 1;
            continue;
          }

          int num = *nptr - '0';
          if (num >= 0 && num <= 9) {
            if (decFound) neg /= 10.0f;
            result = result * 10.0f + (float) num;
          } else {
            // set Error condition for invalid number
          }
        }
        restart = 0;
        break;
      case 'e':

        break;

      // Ignore leading zeros
      case '0':
        ++nptr;
        restart = 1;
        break;
    }
  } while (restart == 0);

  return result * neg;
}

int atoi(const char *nptr) {
  int temp = 1;
  return temp;
}

long int atol(const char *nptr) {
  long temp = 1;
  return temp;
}

long long int atoll(const char *nptr) {
  long long temp = 1;
  return temp;
}