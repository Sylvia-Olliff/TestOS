#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	for (int i = 1; i < 35; i++) {
		printf("Line Number: %i\n", i);
	}
	printf("This is a test of floating numbers converted to string: %f\n", 567.917);
	printf("test of negative numbers: %i\n", -234);
}
