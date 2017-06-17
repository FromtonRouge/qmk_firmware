#include "stroke.h"
#include "undo.h"
#include "lookup_tables/keycodes.h"
#include "lookup_tables/shelton_tables.h"
#include "lookup_tables/user_tables.h"
#include "tmk_core/common/keycode.h"
#include "tmk_core/common/action_util.h"
#include "tmk_core/common/action.h"
#include "quantum/keymap_extras/keymap_french.h"

stroke_element_t g_stroke_buffer[MAX_STROKE_BUFFER] = {{0}};
uint8_t g_stroke_buffer_count = 0;
uint32_t g_bits_keys_pressed_part1 = 0;
uint32_t g_bits_keys_pressed_part2 = 0;
uint16_t g_separator_mode = CKC_SEPMODE_SPC;
uint16_t g_case_mode = CKC_CASE_NORMAL;
uint16_t g_case_mode_on_next_stroke = 0;
uint8_t g_family_bits[NB_FAMILY] = {0};
uint32_t* g_family_to_keys_pressed[NB_FAMILY] = 
{
    &g_bits_keys_pressed_part1, // Special controls
    &g_bits_keys_pressed_part2, // Left controls
    &g_bits_keys_pressed_part2, // Left pinky
    &g_bits_keys_pressed_part1, // Left hand
    &g_bits_keys_pressed_part1, // Thumbs
    &g_bits_keys_pressed_part1, // Right hand
    &g_bits_keys_pressed_part1, // Right pinky
    &g_bits_keys_pressed_part2, // Right controls
    &g_bits_keys_pressed_part1
};


bool can_stroke(void) { return (g_bits_keys_pressed_part1 == 0) && (g_bits_keys_pressed_part2 == 0); }

bool is_letter(uint8_t code)
{
#ifdef AZERTY_OS_ENABLE
    return (code != FR_COMM) && (((code >= KC_A) && (code <= KC_Z)) || (code == FR_M));
#else
    return (code >= KC_A) && (code <= KC_Z);
#endif
}

void send_mods_and_code(uint8_t mods, uint8_t code)
{
    const uint8_t original_mods = get_mods();
    set_mods(mods);
    register_code(code);
    set_mods(original_mods);
}

void stroke_init_buffer(void)
{
    g_stroke_buffer_count = 0;
    for (uint8_t i = 0; i < MAX_STROKE_BUFFER; ++i)
    {
        g_stroke_buffer[i].kind = KIND_UNKNOWN;
        g_stroke_buffer[i].keycode = 0;
    }
}

void stroke_add_element(kind_table_t kind, uint16_t keycode)
{
    g_stroke_buffer[g_stroke_buffer_count].kind = kind;
    g_stroke_buffer[g_stroke_buffer_count].keycode = keycode;
    g_stroke_buffer_count++;
}

void stroke(void)
{
    // Init stroke buffer
    stroke_init_buffer();
    
    // Send characters for each key family
    const uint8_t original_mods = get_mods();
    del_mods(MOD_LSFT|MOD_RSFT);

    g_new_undo_command.change_index = 0;
    for (int i = 0; i < MAX_CHANGES; ++i)
    {
        g_new_undo_command.changes[i].kind = NO_CHANGE;
        g_new_undo_command.changes[i].count = 0;
    }

    // Get *, + and case controls info
    const uint8_t special_controls_bits = g_family_bits[FAMILY_SPECIAL_CONTROLS];
    const bool has_star = special_controls_bits & (1 << (SC_STAR & 0xF));
    const bool has_plus = special_controls_bits & (1 << (SC_PLUS & 0xF));
    const bool has_separator = special_controls_bits & (1 << (SC_SEP & 0xF));
    const uint8_t thumbs_bits = g_family_bits[FAMILY_THUMBS];
    const uint8_t left_pinky_bits = g_family_bits[FAMILY_LEFT_PINKY];
	const bool punctuation_mode = (!thumbs_bits && has_star) && (g_family_bits[FAMILY_LEFT_HAND] || g_family_bits[FAMILY_RIGHT_HAND]);
	if (punctuation_mode)
	{
		// Get bits from right controls and put them in the left controls
		g_family_bits[FAMILY_LEFT_CONTROLS] |= g_family_bits[FAMILY_RIGHT_CONTROLS];
		g_family_bits[FAMILY_LEFT_CONTROLS] &= 0x7; // Keep the first 3 bits only
		g_family_bits[FAMILY_RIGHT_CONTROLS] = 0;
	}

	// Apply new case mode
	if (g_case_mode_on_next_stroke)
	{
		g_case_mode = g_case_mode_on_next_stroke;
		g_case_mode_on_next_stroke = 0;
	}

    // Reset L3 if LP_I is pressed
    if (left_pinky_bits & STENO_KEY_BIT(LP_I))
    {
		uint8_t* left_controls_bits = &g_family_bits[FAMILY_LEFT_CONTROLS];
		const uint8_t bit_L3 = STENO_KEY_BIT(L3);
		if (*left_controls_bits & bit_L3)
		{
			*left_controls_bits &= ~bit_L3;
		}
    }

    // Build stroke (but we don't send it yet)
    uint16_t left_pinky_keycode = 0;
    bool undo_allowed = true;
    for (int family_id = 0; family_id < NB_FAMILY; ++family_id)
    {
        uint8_t family_bits = g_family_bits[family_id];
        if (family_bits == 0)
        {
            continue;
        }

        // Get the lookup table
        undo_allowed = family_id == FAMILY_SPECIAL_CONTROLS;
        void* any_table = g_all_tables[family_id];
        uint8_t kind = g_family_to_kind_table[family_id];
        if (family_id == FAMILY_THUMBS)
        {
            if (has_star)
            {
                any_table = (void*)g_thumbs_bigrams_table;
            }
        }
        else if (family_id == FAMILY_LEFT_HAND)
        {
            if (punctuation_mode)
            {
                any_table = (void*)g_left_punctuation_table;
                kind = KIND_PUNCTUATION;
            }

            // Check left pinky value
            if (left_pinky_keycode == _O || left_pinky_keycode == _U)
			{
				// L_A become L_S, so get the bit state of L_A and put it on L_S
				family_bits &= ~STENO_KEY_BIT(L_S); // Clear L_S
				family_bits |= (family_bits & STENO_KEY_BIT(L_A)) << 1; // Transfert L_A to L_S
				family_bits |= STENO_KEY_BIT(L_A); // Force L_A (it will be converted to _U, _O or _I after)
			}
        }
        else if (family_id == FAMILY_RIGHT_HAND)
        {
            if (punctuation_mode)
            {
                any_table = (void*)g_right_punctuation_table;
                kind = KIND_PUNCTUATION;
            }
        }

        if (any_table)
        {
            switch (kind)
            {
            case KIND_ONE_KEYCODE:
                {
                    one_keycode_table_t* one_keycode_table = (one_keycode_table_t*)any_table;
                    const uint16_t word = pgm_read_word(&(one_keycode_table[family_bits][0]));
                    if (word)
                    {
                        if (family_id == FAMILY_LEFT_CONTROLS)
                        {
							uint16_t previous_case_mode = g_case_mode;
                            switch (word)
                            {
                            case CKC_RESET_SEP_AND_CASE:
                                {
                                    g_separator_mode = CKC_SEPMODE_SPC;
                                    g_case_mode = CKC_CASE_NORMAL;
                                    break;
                                }
                            case CKC_CAMEL:
                                {
                                    g_separator_mode = CKC_SEPMODE_NOSPC;
                                    g_case_mode = CKC_CASE_INNER_LOCKED;
                                    break;
                                }
                            default:
                                {
                                    if ((word >= CKC_CASE_NORMAL) && (word <= CKC_CASE_UPPER_LOCKED))
                                    {
                                        g_case_mode = word;
                                    }
                                    else if ((word >= CKC_SEPMODE_SPC) && (word <= CKC_SEPMODE_NOSPC))
                                    {
                                        g_separator_mode = word;
                                    }
                                    break;
                                }
                            }

							if (punctuation_mode)
							{
								g_case_mode_on_next_stroke = g_case_mode;
								g_case_mode = previous_case_mode;
							}
                        }
                        else if (family_id == FAMILY_LEFT_PINKY)
                        {
                            left_pinky_keycode = word;
                            if (word == _I)
                            {
                                g_family_bits[FAMILY_LEFT_HAND] |= STENO_KEY_BIT(L_A);
                            }
                            else if (word == _O || word == _U)
                            {
								// Add useless L_S so g_family_bits[FAMILY_LEFT_HAND] is not empty
                                g_family_bits[FAMILY_LEFT_HAND] |= STENO_KEY_BIT(L_S);
                            }
                        }
                    }
                    break;
                }
            case KIND_LETTERS:
                {
                    letters_table_t* letters_table = (letters_table_t*)any_table;
                    for (int code_pos = 0; code_pos < MAX_LETTERS; ++code_pos)
                    {
                        uint8_t byte = pgm_read_byte(&(letters_table[family_bits][code_pos]));
                        if (byte)
                        {
                            // Left pinky vowels
                            if (family_id == FAMILY_LEFT_HAND)
                            {
                                if (byte == _A)
                                {
                                    if (left_pinky_keycode != 0)
                                    {
										if (left_pinky_keycode < CKC_STENO)
										{
											byte = (uint8_t)left_pinky_keycode;
										}
                                    }
                                }
                            }

                            stroke_add_element(kind, byte);

                            // Jackdaw rule: If a 'Q' is detected on the left hand followed by a thumb vowel => add a 'U'
                            if (byte == _Q && thumbs_bits)
                            {
                                stroke_add_element(kind, _U);
                            }

                            // Jackdaw rule: Double the first letter for the right hand only if + is in the stroke
                            if (has_plus && (family_id == FAMILY_RIGHT_HAND) && !code_pos)
                            {
                                stroke_add_element(kind, byte);
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    break;
                }
            case KIND_SYMBOLS:
                {
                    symbols_table_t* symbols_table = (symbols_table_t*)any_table;
                    for (int code_pos = 0; code_pos < MAX_SYMBOLS; ++code_pos)
                    {
                        const uint16_t word = pgm_read_word(&(symbols_table[family_bits][code_pos]));
                        if (word)
                        {
                            stroke_add_element(kind, word);
                        }
                        else
                        {
                            break;
                        }
                    }
                    break;
                }
            case KIND_PUNCTUATION:
                {
                    punctuation_table_t* punctuation_table = (punctuation_table_t*)any_table;
                    for (int code_pos = 0; code_pos < MAX_PUNCTUATION; ++code_pos)
                    {
                        const uint16_t word = pgm_read_word(&(punctuation_table[family_bits][code_pos]));
                        if (word)
                        {
                            stroke_add_element(kind, word);
                        }
                        else
                        {
                            break;
                        }
                    }
                    break;
                }
            default:
                {
                    break;
                }
            }
        }
    }

    bool force_normal_case = false;
    // Check if we can send the separator (space, underscore...) before the stroke
    if (g_stroke_buffer_count || (!g_stroke_buffer_count && has_separator && !has_star))
    {
        uint16_t keycode_separator = 0;
        switch (g_separator_mode)
        {
        case CKC_SEPMODE_SPC:
            {
                keycode_separator = KC_SPC;
                break;
            }
        case CKC_SEPMODE_NOSPC:
            {
                // Force normal case for this stroke if the SC_SEP is pressed
                force_normal_case = has_separator;
                break;
            }
        default:
            {
                break;
            }
        }

        bool can_send_separator = true;
        if (g_stroke_buffer_count)
        {
            // Check the first keycode in the stroke
            stroke_element_t* stroke_element = &g_stroke_buffer[0];
            switch (stroke_element->keycode)
            {
            case _COMM:
            case _DOT:
            case _SCLN:
            case KC_ESC:
            case KC_ENT:
            case KC_END:
            case KC_HOME:
                {
                    can_send_separator = false;
                    break;
                }
            default:
                {
                    break;
                }
            }
        }

        if (can_send_separator && keycode_separator && (!has_separator || (!g_stroke_buffer_count && has_separator && !has_star)))
        {
            send_mods_and_code(keycode_separator >> 8, keycode_separator);
            unregister_code(keycode_separator);
            undo_command_add_change(&g_new_undo_command, CHARACTER);
        }
    }

    if (!force_normal_case && (g_case_mode > CKC_CASE_NORMAL))
    {
        add_mods(MOD_LSFT);
    }

    // Send stroke buffer
    uint8_t inserted_characters = 0;
    for (uint8_t i = 0; i < g_stroke_buffer_count; ++i)
    {
        stroke_element_t* stroke_element = &g_stroke_buffer[i];
        switch (stroke_element->kind)
        {
        case KIND_LETTERS:
            {
                register_code(stroke_element->keycode);
                unregister_code(stroke_element->keycode);
                undo_command_add_change(&g_new_undo_command, CHARACTER);
                inserted_characters++;
                break;
            }
        case KIND_SYMBOLS:
            {
                const uint8_t code = (uint8_t)stroke_element->keycode;
                if (is_letter(code))
                {
                    // By doing this the shift mod can be applied on letter code
                    register_code(code);
                }
                else
                {
                    send_mods_and_code(stroke_element->keycode >> 8, code);
                }
                unregister_code(code);
                if (undo_command_add_change_from_code(&g_new_undo_command, code) == CHARACTER)
                {
                    inserted_characters++;
                }
                break;
            }
        case KIND_PUNCTUATION:
            {
                // TODO: Use another table for specific key sequences
                uint16_t specific_sequence[5] = {0};
                uint16_t word = stroke_element->keycode;
                switch (word)
                {
                case CKC_DELWORD:
                    {
                        specific_sequence[0] = LCTL(LSFT(KC_LEFT));
                        specific_sequence[1] = KC_BSPC;
                        break;
                    }
                case CKC_DLEFT:
                    {
                        specific_sequence[0] = KC_LEFT;
                        specific_sequence[1] = KC_LEFT;
                        break;
                    }
                case CKC_ENTABOVE:
                    {
                        specific_sequence[0] = KC_UP;
                        specific_sequence[1] = KC_END;
                        specific_sequence[2] = KC_ENT;
                        break;
                    }
                default:
                    {
                        const uint8_t code = (uint8_t)word;
                        send_mods_and_code(word >> 8, code);
                        unregister_code(code);
                        if (undo_command_add_change_from_code(&g_new_undo_command, code) == CHARACTER)
                        {
                            inserted_characters++;
                        }
                        break;
                    }
                }

                // Specific key sequence if any
                for (int i = 0; i < 5; ++i)
                {
                    const uint16_t word = specific_sequence[i];
                    if (word == 0)
                    {
                        break;
                    }
                    const uint8_t code = (uint8_t)word;
                    send_mods_and_code(word >> 8, code);
                    unregister_code(code);
                    if (undo_command_add_change_from_code(&g_new_undo_command, code) == CHARACTER)
                    {
                        inserted_characters++;
                    }
                }
                break;
            }
        default:
            {
                break;
            }
        }

        // Camel case
        if (inserted_characters == 1)
        {
            switch (g_case_mode)
            {
            case CKC_CASE_INNER_ONCE:
                {
                    g_case_mode = CKC_CASE_NORMAL;
                    // no break
                }
            case CKC_CASE_INNER_LOCKED:
                {
                    del_mods(MOD_LSFT);
                    break;
                }
            default:
                {
                    break;
                }
            }
        }
    }

    if (inserted_characters && (g_case_mode == CKC_CASE_UPPER_ONCE))
    {
        g_case_mode = CKC_CASE_NORMAL;
        del_mods(MOD_LSFT);
    }

    if (can_undo(&g_new_undo_command))
    {
        // Undo history
        if (g_undo_stack_index == MAX_UNDO)
        {
            g_undo_stack_index = 0;
        }

        g_undo_stack[g_undo_stack_index++] = g_new_undo_command;
    }
    else if (has_star)
    {
        // Compute the previous index
        int8_t previous_index = g_undo_stack_index - 1;
        if (previous_index < 0)
        {
            previous_index = MAX_UNDO - 1;
        }

        // Check if we have data to undo at previous_index
        undo_command_t* previous_undo_command = &g_undo_stack[previous_index];
        if (can_undo(previous_undo_command))
        {
            if (has_separator)
            {
                // SC_SEP becomes a Backspace
                register_code(KC_BSPC);
                unregister_code(KC_BSPC);
                
                // Patch chars to delete for the next undo
                for (int i = previous_undo_command->change_index; i >= 0; --i)
                {
                    if (previous_undo_command->changes[i].kind == CHARACTER)
                    {
                        previous_undo_command->changes[i].count--;
                        break;
                    }
                }

                // If there is no more data to remove at previous_index we can go to the previous stroke undo data 
                const uint8_t inserted_characters = undo_command_get_changes_count(&g_undo_stack[previous_index], CHARACTER);
                if (inserted_characters == 0)
                {
                    g_undo_stack_index = previous_index;
                }
            }
            else if (undo_allowed)
            {
                // Parse changes in reverse order and undo them
                for (int i = previous_undo_command->change_index; i >= 0; --i)
                {
                    change_t* change = &previous_undo_command->changes[i];
                    for (int j = 0; j < change->count; ++j)
                    {
                        switch (change->kind)
                        {
                        case CHARACTER:
                            {
                                register_code(KC_BSPC);
                                unregister_code(KC_BSPC);
                                break;
                            }
                        case MOVE_LEFT:
                            {
                                register_code(KC_RIGHT);
                                unregister_code(KC_RIGHT);
                                break;
                            }
                        case ENTER:
                            {
                                // We use SHIFT to select text. We have to do this for text editors that autoindent text.
                                // Vim users needs to add : 'behave mswin' in their .vimrc file
                                register_code(KC_LSFT);
                                register_code(KC_UP);
                                unregister_code(KC_UP);
                                register_code(KC_END);
                                unregister_code(KC_END);
                                unregister_code(KC_LSFT);

                                register_code(KC_DEL);
                                unregister_code(KC_DEL);
                                break;
                            }
                        case MOVE_UP:
                            {
                                register_code(KC_DOWN);
                                unregister_code(KC_DOWN);
                                break;
                            }
                        default:
                            {
                                break;
                            }
                        }
                    }
                }

                g_undo_stack_index = previous_index;
            }
        }
        else if (has_separator)
        {
            // No data to remove in the undo stack, but we allow the SC_SEP to become a Backspace
            register_code(KC_BSPC);
            unregister_code(KC_BSPC);
        }
    }

    // Restore original mods
    set_mods(original_mods);

    // Clear bits
    for (int i = 0; i < NB_FAMILY; ++i)
    {
        g_family_bits[i] = 0;
    }
}

