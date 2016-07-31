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
    _S3(_I, _N, _G),                        // [8]      3
    NO_ENTRY,                               // [9]      0|3
    _S3(_APOS, _L, _L),                     // [10]     1|3
    NO_ENTRY,                               // [11]     0|1|3
    _S2(_E, _R),                            // [12]     2|3
    NO_ENTRY,                               // [13]     0|2|3
    NO_ENTRY,                               // [14]     1|2|3
    NO_ENTRY,                               // [15]
    _S3(_I, _N, _G),                        // [16]     4
    NO_ENTRY,                               // [17]     0|4
    _S3(_APOS, _V, _E),                     // [18]     1|4
    NO_ENTRY,                               // [19]     0|1|4
    NO_ENTRY,                               // [20]     2|4
    NO_ENTRY,                               // [21]     0|2|4
    NO_ENTRY,                               // [22]
    NO_ENTRY,                               // [23]
    _S2(_E, _R),                            // [24]     3|4
    NO_ENTRY,                               // [25]     0|3|4
    NO_ENTRY,                               // [26]     1|3|4
    NO_ENTRY,                               // [27]
    NO_ENTRY,                               // [28]     2|3|4
    NO_ENTRY,                               // [29]
    NO_ENTRY,                               // [30]
    NO_ENTRY,                               // [31]
    _S3(_I, _N, _G),                        // [32]     5
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
    _S2(_E, _R),                            // [48]     4|5
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

const uint16_t PROGMEM g_left_punctuations_table[64][MAX_SYMBOLS] =
{
    NO_ENTRY,                               // [0]
    _S1(_LPRN),                             // [1]
    _S1(_UNDS),                             // [2]
    NO_ENTRY,                               // [3]
    _S1(_RPRN),                             // [4]
    _S2(_LPRN, _RPRN),                      // [5]
    NO_ENTRY,                               // [6]
    NO_ENTRY,                               // [7]
    _S1(_MINS),                             // [8]
    NO_ENTRY,                               // [9]
    NO_ENTRY,                               // [10]
    NO_ENTRY,                               // [11]
    NO_ENTRY,                               // [12]
    NO_ENTRY,                               // [13]
    NO_ENTRY,                               // [14]
    NO_ENTRY,                               // [15]
    _S1(_DOT),                              // [16]
    NO_ENTRY,                               // [17]
    NO_ENTRY,                               // [18]
    NO_ENTRY,                               // [19]
    _S2(_RPRN, _DOT),                       // [20]
    _S3(_LPRN, _RPRN, _DOT),                // [21]
    NO_ENTRY,                               // [22]
    NO_ENTRY,                               // [23]
    NO_ENTRY,                               // [24]
    NO_ENTRY,                               // [25]
    NO_ENTRY,                               // [26]
    NO_ENTRY,                               // [27]
    NO_ENTRY,                               // [28]
    NO_ENTRY,                               // [29]
    NO_ENTRY,                               // [30]
    NO_ENTRY,                               // [31]
    _S1(_COMM),                             // [32]
    NO_ENTRY,                               // [33]
    NO_ENTRY,                               // [34]
    NO_ENTRY,                               // [35]
    _S2(_RPRN, _COMM),                      // [36]
    _S3(_LPRN, _RPRN, _COMM),               // [37]
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
    _S1(_SCLN),                             // [48]
    NO_ENTRY,                               // [49]
    NO_ENTRY,                               // [50]
    NO_ENTRY,                               // [51]
    _S2(_RPRN, _SCLN),                      // [52]
    _S3(_LPRN, _RPRN, _SCLN),               // [53]
    NO_ENTRY,                               // [54]
    NO_ENTRY,                               // [55]
    NO_ENTRY,                               // [56]
    NO_ENTRY,                               // [57]
    NO_ENTRY,                               // [58]
    NO_ENTRY,                               // [59]
    NO_ENTRY,                               // [60]
    NO_ENTRY,                               // [61]
    NO_ENTRY,                               // [62]
    NO_ENTRY                                // [63]
};

const uint16_t PROGMEM g_right_punctuations_table[64][MAX_SYMBOLS] =
{
    NO_ENTRY,                               // [0]
    _S1(_DOT),                              // [1]
    _S1(_DOT),                              // [2]
    _S1(_COLN),                             // [3]
    _S1(_LPRN),                             // [4]
    NO_ENTRY,                               // [5]
    _S1(_LESS),                             // [6]
    NO_ENTRY,                               // [7]
    _S1(_APOS),                             // [8]
    NO_ENTRY,                               // [9]
    NO_ENTRY,                               // [10]
    NO_ENTRY,                               // [11]
    _S1(_DQUOT),                            // [12]
    NO_ENTRY,                               // [13]
    NO_ENTRY,                               // [14]
    NO_ENTRY,                               // [15]
    _S1(_RPRN),                             // [16]
    NO_ENTRY,                               // [17]
    NO_ENTRY,                               // [18]
    NO_ENTRY,                               // [19]
    _S3(_LPRN, _RPRN, KC_LEFT),             // [20]
    NO_ENTRY,                               // [21]
    NO_ENTRY,                               // [22]
    NO_ENTRY,                               // [23]
    NO_ENTRY,                               // [24]
    NO_ENTRY,                               // [25]
    NO_ENTRY,                               // [26]
    NO_ENTRY,                               // [27]
    NO_ENTRY,                               // [28]
    NO_ENTRY,                               // [29]
    NO_ENTRY,                               // [30]
    NO_ENTRY,                               // [31]
    _S1(_QUES),                             // [32]
    NO_ENTRY,                               // [33]
    NO_ENTRY,                               // [34]
    NO_ENTRY,                               // [35]
    _S1(_GRTR),                             // [36]
    NO_ENTRY,                               // [37]
    _S3(_LESS, _GRTR, KC_LEFT),             // [38]
    NO_ENTRY,                               // [39]
    _S3(_APOS, _APOS, KC_LEFT),             // [40]
    NO_ENTRY,                               // [41]
    NO_ENTRY,                               // [42]
    NO_ENTRY,                               // [43]
    _S3(_DQUOT, _DQUOT, KC_LEFT),           // [44]
    NO_ENTRY,                               // [45]
    NO_ENTRY,                               // [46]
    NO_ENTRY,                               // [47]
    _S1(_EXLM),                             // [48]
    NO_ENTRY,                               // [49]
    NO_ENTRY,                               // [50]
    NO_ENTRY,                               // [51]
    NO_ENTRY,                               // [52]
    NO_ENTRY,                               // [53]
    NO_ENTRY,                               // [54]
    NO_ENTRY,                               // [55]
    NO_ENTRY,                               // [56]
    NO_ENTRY,                               // [57]
    NO_ENTRY,                               // [58]
    NO_ENTRY,                               // [59]
    NO_ENTRY,                               // [60]
    NO_ENTRY,                               // [61]
    NO_ENTRY,                               // [62]
    NO_ENTRY                                // [63]
};

#endif
