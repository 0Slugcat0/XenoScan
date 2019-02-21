#pragma once

#define ASSERT(x)\
do { \
if (!(x)) {	\
	__asm__("int3"); \
} \
} while (0, 0);
