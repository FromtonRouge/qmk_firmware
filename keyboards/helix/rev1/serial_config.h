#ifndef SOFT_SERIAL_CONFIG_H
#define SOFT_SERIAL_CONFIG_H

/* Soft Serial defines */
#define SERIAL_PIN_DDR DDRD
#define SERIAL_PIN_PORT PORTD
#define SERIAL_PIN_INPUT PIND
#define SERIAL_PIN_MASK _BV(PD0)
#define SERIAL_PIN_INTERRUPT INT0_vect

#define SERIAL_SLAVE_BUFFER_LENGTH MATRIX_ROWS/2
#define SERIAL_MASTER_BUFFER_LENGTH 0

/// #error rev1 serial config

#endif /* SOFT_SERIAL_CONFIG_H */