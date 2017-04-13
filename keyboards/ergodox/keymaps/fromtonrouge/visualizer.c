#include "simple_visualizer.h"

// This function should be implemented by the keymap visualizer
// Don't change anything else than state->target_lcd_color and state->layer_text as that's the only thing
// that the simple_visualizer assumes that you are updating
// Also make sure that the buffer passed to state->layer_text remains valid until the previous animation is
// stopped. This can be done by either double buffering it or by using constant strings
static void get_visualizer_layer_and_color(visualizer_state_t* state)
{
#define RED 0
#define GREEN 84
#define BLUE 168

    uint8_t saturation = 127;
    uint8_t hue = GREEN;
    uint8_t intensity = 255;

    if (state->status.layer & 0x10)
    {
        // Layer 4
#ifdef PROGRAMMER_COLEMAK_ENABLE
#ifdef AZERTY_OS_ENABLE
        state->layer_text = "Function";
        saturation = 255;
        hue = BLUE;
#endif
#endif
    }
    else if (state->status.layer & 0x8)
    {
        // Layer 3
#ifdef PROGRAMMER_COLEMAK_ENABLE
#ifdef AZERTY_OS_ENABLE
        state->layer_text = "Accents";
        saturation = 255;
        hue = GREEN;
#else
        state->layer_text = "Function";
        saturation = 255;
        hue = BLUE;
#endif
#endif
    }
    else if (state->status.layer & 0x4)
    {
        // Layer 2
#ifdef PROGRAMMER_COLEMAK_ENABLE
        state->layer_text = "Shift Colemak";
        saturation = 255;
        hue = GREEN;
#else
        state->layer_text = "Function";
        saturation = 255;
        hue = BLUE;
#endif
    }
    else if (state->status.layer & 0x2)
    {
        // Layer 1
        state->layer_text = "Jackdaw";
        hue = RED;
    }
    else
    {
#ifdef PROGRAMMER_COLEMAK_ENABLE
        state->layer_text = "Colemak";
#else
        state->layer_text = "Default";
#endif
        saturation = 63;
        hue = GREEN;
    }

    if (state->status.leds & (1u << USB_LED_CAPS_LOCK))
    {
        saturation = 255;
    }

    state->target_lcd_color = LCD_COLOR(hue, saturation, intensity);
}
