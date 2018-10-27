#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'i') {
			format++;
			int n = va_arg(parameters, int);
			int r;
			char* str;
			memset(str, 0, sizeof str);
			size_t count = 0;

			// while (n>0) {
			// 	r=n%10;
			// 	sum=sum*10+r;
			// 	n=n/10;
			// }
			// n=sum;

			while (n > 0) {
				r = n % 10;
				switch(r) {
					case 1:
						str[count] = '1';
						break;
					case 2:
						str[count] = '2';
						break;
					case 3:
						str[count] = '3';
						break;
					case 4:
						str[count] = '4';
						break;
					case 5:
						str[count] = '5';
						break;
					case 6:
						str[count] = '6';
						break;
					case 7:
						str[count] = '7';
						break;
					case 8:
						str[count] = '8';
						break;
					case 9:
						str[count] = '9';
						break;
					case 0:
						str[count] = '0';
						break;
				}
				n = n / 10;
				count++;
			}
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW/
				return -1;
			}
			if (len > 1) {
				for (size_t index = 0; index < (len / 2); index++) {
					char current = str[index];
					char swapped = str[len - 1 - index];
					str[index] = swapped;
					str[len - 1 - index] = current;
				}
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
