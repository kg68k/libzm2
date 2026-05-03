# Makefile for zm2call

ifeq ($(findstring human68k,$(MAKE_VERSION)),human68k)
ifeq ($(notdir $(CURDIR)),src)
$(error do not execute make in src directory)
endif

SRC_DIR = .
INC_DIR = $(shell cat ../srcdir.txt)/../include

CC = gcc
ifeq ($(CC),gcc2)
CWARNFLAGS = -Wall -W -Werror
else
CWARNFLAGS = -Wall -W
endif

else # elf2x68k

SRC_DIR = $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
INC_DIR = $(SRC_DIR)/../include

CC = m68k-xelf-gcc
CWARNFLAGS = -Wall -Wextra -Werror

endif

CFLAGS = -funsigned-char -O2 $(CWARNFLAGS) -I$(INC_DIR) -ldos

HEADERS = libzm2.h libzm2const.h libzm2midi.h libzm2util.h libzm2work.h libzm2internal.h
HEADS = $(addprefix $(INC_DIR)/,$(HEADERS))
SRCS = $(SRC_DIR)/zm2call.c
OBJS = $(SRCS:.c=.o)

TARGET = zm2call.x

.PHONY: all archive clean

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADS)
	$(CC) $(CFLAGS) -o$@ $<

zm2call.s: $(SRCS) $(HEADS)
	$(CC) $(CFLAGS) $< -S


clean:
	rm -f $(TARGET)

# EOF
