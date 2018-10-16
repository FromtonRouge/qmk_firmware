ifndef QUANTUM_DIR
	include ../../../../Makefile
endif

SRC += lookup_tables/tables.c \
	   lookup_tables/shelton_tables.c \
	   lookup_tables/user_tables.c \
	   stroke.c \
	   undo.c
