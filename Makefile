# Makefile template for a shared library in C
# https://www.topbug.net/blog/2019/10/28/makefile-template-for-a-shared-library-in-c-with-explanations/

# CC = gcc  # C compiler
CC = arm-linux-gnueabi-gcc # ARM C compiler

CFLAGS = -fPIC -Wall -Wextra -O2 -g  # C flags
LDFLAGS = -shared   # linking flags
RM = rm -f   # rm command
TARGET_LIB = fps.o  # target lib
TARGET_CLIB = fps.lib

SRCS = fps_cmd.c fps_res.c fps_data.c  # source files
OBJS = $(SRCS:.c=.o)

.PHONY: all
all: ${TARGET_CLIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

$(TARGET_CLIB) : $(TARGET_LIB)
	ar rcs $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)