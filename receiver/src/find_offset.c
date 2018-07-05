#include "find_offset.h"

int find_offset(const unsigned char c) {
	int i = 0;
	for (; i < 8; i++) {
		unsigned char mask = 0x01 << i;
		if ((c & mask) == 0) {
			break;
		}
	}
	return 8-i;
}
