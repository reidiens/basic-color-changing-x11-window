PROG = basic-window
SRC = ${PROG}.c
OBJ = ${SRC:.c=.o}

CC = gcc
INCS = /usr/include/X11
LIBS = -lX11

LDFLAGS = ${LIBS}
CFLAGS = -Wall -Wextra ${INCS}

.PHONY: all clean

all: ${PROG}

${PROG}: ${OBJ}
  ${CC} ${OBJ} -o $@ ${LDFLAGS}

%.o: %.c
  ${CC} -c $< ${CFLAGS}

clean:
  rm ${OBJ}
