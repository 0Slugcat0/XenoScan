#pragma once

#define ASSERT(x)\
do { \
if (!(x)) {	\
	asm ( "int 3" ); \
} \
} while (0, 0);
