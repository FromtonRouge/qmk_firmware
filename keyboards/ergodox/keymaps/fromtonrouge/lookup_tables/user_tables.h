#ifndef USER_TABLES_H
#define USER_TABLES_H

#include "encode.h"

// User symbols lookup tables for the steno layer
const uint16_t PROGMEM g_left_user_symbols_table[64][MAX_SYMBOLS] =
{
    NO_ENTRY,                               // [0]
    _S1(_EXLM),                             // [1]      0
    _S1(KC_ESC),                            // [2]      1
    _S2(KC_ESC, _COLN),                     // [3]      0|1
    _S1(_ASTR),                             // [4]      2
    NO_ENTRY,                               // [5]      0|2
    NO_ENTRY,                               // [6]      1|2
    NO_ENTRY,                               // [7]
    _S1(_UNDS),                             // [8]      3
    NO_ENTRY,                               // [9]
    NO_ENTRY,                               // [10]     1|3
    NO_ENTRY,                               // [11]
    NO_ENTRY,                               // [12]     2|3
    NO_ENTRY,                               // [13]     0|2|3
    NO_ENTRY,                               // [14]     1|2|3
    NO_ENTRY,                               // [15]
    NO_ENTRY,                               // [16]     4
    NO_ENTRY,                               // [17]
    NO_ENTRY,                               // [18]     1|4
    NO_ENTRY,                               // [19]
    NO_ENTRY,                               // [20]
    NO_ENTRY,                               // [21]
    NO_ENTRY,                               // [22]
    NO_ENTRY,                               // [23]
    NO_ENTRY,                               // [24]     3|4
    NO_ENTRY,                               // [25]
    NO_ENTRY,                               // [26]     1|3|4
    NO_ENTRY,                               // [27]
    NO_ENTRY,                               // [28]     2|3|4
    NO_ENTRY,                               // [29]
    NO_ENTRY,                               // [30]
    NO_ENTRY,                               // [31]
    NO_ENTRY,                               // [32]     5
    NO_ENTRY,                               // [33]     0|5
    NO_ENTRY,                               // [34]
    NO_ENTRY,                               // [35]     0|1|5
    NO_ENTRY,                               // [36]
    NO_ENTRY,                               // [37]
    NO_ENTRY,                               // [38]
    NO_ENTRY,                               // [39]
    NO_ENTRY,                               // [40]
    NO_ENTRY,                               // [41]
    NO_ENTRY,                               // [42]
    NO_ENTRY,                               // [43]
    NO_ENTRY,                               // [44]
    NO_ENTRY,                               // [45]
    NO_ENTRY,                               // [46]
    NO_ENTRY,                               // [47]
    NO_ENTRY,                               // [48]     4|5
    NO_ENTRY,                               // [49]     0|4|5
    NO_ENTRY,                               // [50]
    NO_ENTRY,                               // [51]
    NO_ENTRY,                               // [52]
    NO_ENTRY,                               // [53]
    NO_ENTRY,                               // [54]
    NO_ENTRY,                               // [55]
    NO_ENTRY,                               // [56]
    NO_ENTRY,                               // [57]     0|3|4|5
    NO_ENTRY,                               // [58]
    NO_ENTRY,                               // [59]
    NO_ENTRY,                               // [60]
    NO_ENTRY,                               // [61]
    NO_ENTRY,                               // [62]
    NO_ENTRY                                // [63]
};

const uint16_t PROGMEM g_right_user_symbols_table[64][MAX_SYMBOLS] =
{
    NO_ENTRY,                               // [0]
    _S1(_D),                                // [1]      0
    _S2(_APOS, _S),                         // [2]      1
    _S1(_D),                                // [3]      0|1
    NO_ENTRY,                               // [4]      2
    NO_ENTRY,                               // [5]      0|2
    _S3(_N, _APOS, _T),                     // [6]      1|2
    NO_ENTRY,                               // [7]      0|1|2
    _S2(_E, _R),                            // [8]      3
    NO_ENTRY,                               // [9]      0|3
    _S3(_APOS, _L, _L),                     // [10]     1|3
    NO_ENTRY,                               // [11]     0|1|3
    _S3(_I, _N, _G),                        // [12]     2|3
    NO_ENTRY,                               // [13]     0|2|3
    NO_ENTRY,                               // [14]     1|2|3
    NO_ENTRY,                               // [15]
    _S2(_E, _R),                            // [16]     4
    NO_ENTRY,                               // [17]     0|4
    _S3(_APOS, _V, _E),                     // [18]     1|4
    NO_ENTRY,                               // [19]     0|1|4
    NO_ENTRY,                               // [20]     2|4
    NO_ENTRY,                               // [21]     0|2|4
    NO_ENTRY,                               // [22]
    NO_ENTRY,                               // [23]
    _S3(_I, _N, _G),                        // [24]     3|4
    NO_ENTRY,                               // [25]     0|3|4
    NO_ENTRY,                               // [26]     1|3|4
    NO_ENTRY,                               // [27]
    NO_ENTRY,                               // [28]     2|3|4
    NO_ENTRY,                               // [29]
    NO_ENTRY,                               // [30]
    NO_ENTRY,                               // [31]
    _S2(_E, _R),                            // [32]     5
    NO_ENTRY,                               // [33]     0|5
    _S2(_APOS, _D),                         // [34]     1|5
    NO_ENTRY,                               // [35]     0|1|5
    NO_ENTRY,                               // [36]
    NO_ENTRY,                               // [37]
    NO_ENTRY,                               // [38]
    NO_ENTRY,                               // [39]
    NO_ENTRY,                               // [40]     3|5
    NO_ENTRY,                               // [41]
    NO_ENTRY,                               // [42]
    NO_ENTRY,                               // [43]
    NO_ENTRY,                               // [44]     2|3|5
    NO_ENTRY,                               // [45]
    NO_ENTRY,                               // [46]
    NO_ENTRY,                               // [47]
    _S3(_I, _N, _G),                        // [48]     4|5
    NO_ENTRY,                               // [49]
    _S2(_APOS, _M),                         // [50]     1|4|5
    NO_ENTRY,                               // [51]
    NO_ENTRY,                               // [52]
    NO_ENTRY,                               // [53]
    NO_ENTRY,                               // [54]
    NO_ENTRY,                               // [55]
    NO_ENTRY,                               // [56]     3|4|5
    NO_ENTRY,                               // [57]
    NO_ENTRY,                               // [58]
    NO_ENTRY,                               // [59]
    NO_ENTRY,                               // [60]
    NO_ENTRY,                               // [61]
    NO_ENTRY,                               // [62]
    NO_ENTRY                                // [63]
};

const uint16_t PROGMEM g_left_punctuations_table[64][MAX_PUNCTUATIONS] =
{
    NO_ENTRY,
    _P1(_APOS),
    _P1(_COMM),
    _P1(_SCLN),
    _P1(_LCBR),
    _P1(_DQUOT),
    _P1(_GRTR),
    _P2(_GRTR, _GRTR),
    _P1(_LPRN),
    _P1(_LESS),
    _P3(_MINS, _GRTR, _NOSPC),
    _P2(_LESS, _LESS),
    _P1(_LBRC),
    NO_ENTRY,
    _P1(_QUES),
    _P1(_HASH),
    _P1(_PLUS),
    _P1(_GRV),
    _P1(_TILD),
    _P1(_DOT),
    _P2(_PLUS, _PLUS),
    NO_ENTRY,
    _P1(_AMP),
    _P2(_AMP, _AMP),
    _P1(_BSLS),
    NO_ENTRY,
    _P2(_BSLS, _BSLS),
    _P1(_AT),
    NO_ENTRY,
    NO_ENTRY,
    NO_ENTRY,
    NO_ENTRY,
    _P1(_MINS),
    NO_ENTRY,
    _P1(_EQL),
    _P1(_DLR),
    _P1(_SLSH),
    _P2(_SLSH, _SLSH),
    _P1(_CIRC),
    _P3(_SLSH, _SLSH, _SLSH),
    _P2(_MINS, _MINS),
    NO_ENTRY,
    _P1(_UNDS),
    _P2(_DLR, _LPRN),
    _P2(_SLSH, _ASTR),
    NO_ENTRY,
    _P3(_SLSH, _ASTR, _ASTR),
    _P4(_SLSH, _SLSH, _SLSH, _LESS),
    _P1(_ASTR),
    NO_ENTRY,
    _P1(_EXLM),
    _P2(_DOT, _ASTR),
    _P2(_ASTR, _SLSH),
    NO_ENTRY,
    NO_ENTRY,
    _P2(_DLR, _LCBR),
    _P2(_ASTR, _LPRN),
    NO_ENTRY,
    _P1(_PIPE),
    _P3(_ASTR, _DOT, _ASTR),
    _P1(_PERC),
    NO_ENTRY,
    _P2(_PIPE, _PIPE),
    NO_ENTRY
};

const uint16_t PROGMEM g_right_punctuations_table[64][MAX_PUNCTUATIONS] =
{
    NO_ENTRY,
    _P1(_APOS),
    _P1(_DOT),
    _P3(_APOS, KC_LEFT, _NOSPC),
    _P1(_RCBR),
    _P1(_DQUOT),
    _P3(_RCBR, KC_LEFT, _NOSPC),
    _P3(_DQUOT, KC_LEFT, _NOSPC),
    _P1(_RPRN),
    _P1(_GRTR),
    _P3(_RPRN, KC_LEFT, _NOSPC),
    _P3(_GRTR, KC_LEFT, _NOSPC),
    _P1(_RBRC),
    NO_ENTRY,
    _P3(_RBRC, KC_LEFT, _NOSPC),
    _P2(KC_SPC, KC_LEFT),
    _P1(_DOT),
    _P2(_APOS, _DOT),
    _P1(_COLN),
    _P5(_APOS, _DOT, KC_LEFT, KC_LEFT, _NOSPC),
    _P2(_RCBR, _DOT),
    _P2(_DQUOT, _DOT),
    _P5(_RCBR, _DOT, KC_LEFT, KC_LEFT, _NOSPC),
    _P5(_DQUOT, _DOT, KC_LEFT, KC_LEFT, _NOSPC),
    _P2(_RPRN, _DOT),
    _P2(_ASTR, _SLSH),
    _P5(_RPRN, _DOT, KC_LEFT, KC_LEFT, _NOSPC),
    _P5(_ASTR, _SLSH, KC_LEFT, KC_LEFT, _NOSPC),
    _P2(_RBRC, _DOT),
    NO_ENTRY,
    _P5(_RBRC, _DOT, KC_LEFT, KC_LEFT, _NOSPC),
    NO_ENTRY,
    _P1(_COMM),
    _P2(_APOS, _COMM),
    _P1(_EQL),
    _P5(_APOS, _COMM, KC_LEFT, KC_LEFT, _NOSPC),
    _P2(_RCBR, _COMM),
    _P2(_DQUOT, _COMM),
    _P5(_RCBR, _COMM, KC_LEFT, KC_LEFT, _NOSPC),
    _P5(_DQUOT, _COMM, KC_LEFT, KC_LEFT, _NOSPC),
    _P2(_RPRN, _COMM),
    _P2(_GRTR, _COMM),
    _P5(_RPRN, _COMM, KC_LEFT, KC_LEFT, _NOSPC),
    _P5(_GRTR, _COMM, KC_LEFT, KC_LEFT, _NOSPC),
    _P2(_RBRC, _COMM),
    NO_ENTRY,
    _P5(_RBRC, _COMM, KC_LEFT, KC_LEFT, _NOSPC),
    NO_ENTRY,
    _P1(_SCLN),
    _P2(_APOS, _SCLN),
    _P2(_COLN, _COLN),
    _P5(_APOS, _SCLN, KC_LEFT, KC_LEFT, _NOSPC),
    _P2(_RCBR, _SCLN),
    _P2(_DQUOT, _SCLN),
    _P5(_RCBR, _SCLN, KC_LEFT, KC_LEFT, _NOSPC),
    _P5(_DQUOT, _SCLN, KC_LEFT, KC_LEFT, _NOSPC),
    _P2(_RPRN, _SCLN),
    NO_ENTRY,
    _P5(_RPRN, _SCLN, KC_LEFT, KC_LEFT, _NOSPC),
    NO_ENTRY,
    _P2(_RBRC, _SCLN),
    NO_ENTRY,
    _P5(_RBRC, _SCLN, KC_LEFT, KC_LEFT, _NOSPC),
    NO_ENTRY
};

#endif
