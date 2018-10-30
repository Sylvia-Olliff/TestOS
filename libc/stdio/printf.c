#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

static int decShiftInt(int x, int y) {
	int result = 1;
	while (y > 0) {
		result *= x;
		y--;
	}
	return result;
}

static char digitToChar(int digit) {
	char result;
	switch(digit) {
		case 1:
			result = '1';
			break;
		case 2:
			result = '2';
			break;
		case 3:
			result = '3';
			break;
		case 4:
			result = '4';
			break;
		case 5:
			result = '5';
			break;
		case 6:
			result = '6';
			break;
		case 7:
			result = '7';
			break;
		case 8:
			result = '8';
			break;
		case 9:
			result = '9';
			break;
		case 0:
			result = '0';
			break;
	}
	return result;
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
			// FIXME: Negative numbers are getting an 'S' added to the end
			format++;
			int num = va_arg(parameters, int);
			size_t size = 0;
			size_t index = 0;
			int digit;
			int count = 1;
			int factor = 1;

			if (num < 0) {
				factor = -1;
				num *= factor;
			}

			while (count <= num) {
				++size;
				count *= 10;
			}
			
			char* str;
			memset(str, 0, sizeof str);

			while (size > 0) {
				int shiftValue = decShiftInt(10, (size - 1));
				if (num > 10)	digit = num / shiftValue;
				else digit = num;

				if (index == 0 && factor == -1) {
					str[index] = '-';
					index++;
				}

				str[index] = digitToChar(digit);

				num -= shiftValue * digit;
				size--;
				index++;
			}
			size_t len = strlen(str);

			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW/
				return -1;
			}

			if (!print(str, len))
				return -1;
			written += len;
			memset(str, 0, sizeof str);
		} else if (*format == 'f') {
			format++;
			float num = (float) va_arg(parameters, double); /* float promotes to double */
			float decimalRight = num - (int) num;
			int decimalLeft = (int) num;
			int factor = 1;

			if (num < 0) {
				factor = -1;
				num *= factor;
			}

			size_t sizeLeft = 0;
			size_t sizeRight = 0;
			size_t index = 0;

			int digit;
			float count = 1;

			char* str;
			memset(str, 0, sizeof str);

			while (count <= decimalLeft) {
				++sizeLeft;
				count *= 10;
			}
			count = 1;
			while (count >= decimalRight) {
				++sizeRight;
				count /= 10;
			}

			while (sizeLeft > 0) {
				int shiftValue = decShiftInt(10, (sizeLeft - 1));
				if (decimalLeft > 10)	digit = decimalLeft / shiftValue;
				else digit = decimalLeft;

				if (index == 0 && factor == -1) {
					str[index] = '-';
					index++;
				}

				str[index] = digitToChar(digit);

				decimalLeft -= shiftValue * digit;
				sizeLeft--;
				index++;
			}

			str[index] = '.';
			index++;

			while (sizeRight > 0) {
				float shiftValue = 0.1;
				digit = (int) (decimalRight / shiftValue);

				str[index] = digitToChar(digit);

				decimalRight -= digit;
				sizeRight--;
				index++;
			}

			size_t len = strlen(str);

			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW/
				return -1;
			}

			if (!print(str, len))
				return -1;
			written += len;
			memset(str, 0, sizeof str);
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
