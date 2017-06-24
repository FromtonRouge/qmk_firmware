#ifndef TABLE_H
#define TABLE_H

#include <stdint.h>

// A lookup table can stores letters (8 bits) or symbols (16 bits)
typedef enum
{
    KIND_UNKNOWN,
    KIND_ONE_KEYCODE,
    KIND_LETTERS,
    KIND_SYMBOLS,
    KIND_PUNCTUATION
} kind_table_t;

#define MAX_LETTERS 6
#define NO_ENTRY {0}
#define _1(c1) {c1, 0, 0, 0, 0, 0}
#define _2(c1, c2) {c1, c2, 0, 0, 0, 0}
#define _3(c1, c2, c3) {c1, c2, c3, 0, 0, 0}
#define _4(c1, c2, c3, c4) {c1, c2, c3, c4, 0, 0}
#define _5(c1, c2, c3, c4, c5) {c1, c2, c3, c4, c5, 0}
#define _6(c1, c2, c3, c4, c5, c6) {c1, c2, c3, c4, c5, c6}

#define MAX_SYMBOLS 3
#define _S1(c1) {c1, 0, 0}
#define _S2(c1, c2) {c1, c2, 0}
#define _S3(c1, c2, c3) {c1, c2, c3}

#define MAX_PUNCTUATION 4
#define _P1(c1) {c1, 0, 0, 0}
#define _P2(c1, c2) {c1, c2, 0, 0}
#define _P3(c1, c2, c3) {c1, c2, c3, 0}
#define _P4(c1, c2, c3, c4) {c1, c2, c3, c4}

#define MAX_ONE_KEYCODE 1
#define _K1(c1) {c1}

typedef const uint16_t one_keycode_table_t[MAX_ONE_KEYCODE];
typedef const uint8_t letters_table_t[MAX_LETTERS];
typedef const uint16_t symbols_table_t[MAX_SYMBOLS];
typedef const uint16_t punctuation_table_t[MAX_PUNCTUATION];

#endif
