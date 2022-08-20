.PHONY: all, clean

TARGET=main

HDRS+=
CSRCS+=main.c

OBJSDIR=build
OBJS:= $(patsubst %.c, $(OBJSDIR)/%.o, $(CSRCS))

CFLAGS+= -I./include -DDEBUG -Wall -g
LDFLAGS+= -L./lib -lm

CC:= gcc

all: ${TARGET}
${TARGET}: $(OBJS)
	@echo "  LINK	 $@"
	@mkdir -p $(shell dirname $@)
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJSDIR)/%.o: %.c $(HDRS)
	@echo "  CC	 $@"
	@mkdir -p $(shell dirname $@)
	@$(CC) -c $< -o $@ $(CFLAGS)

clean:
	@echo "  CLEAN	 ${OBJSDIR}"
	@rm -rf ${OBJSDIR}
	@echo "  CLEAN	 ${TARGET}"
	@rm -rf ${TARGET}
