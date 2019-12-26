# Makefile for go

CC = gcc
CFLAGS = -g3 -O0 -Wall -Wextra

ARGS =

INSTALDIR = /usr/local/bin

all: go

run:
	./go $(ARGS)

install:go
	cp ./go $(INSTALDIR)/

uninstall: $(INSTALDIR)/go
	rm -i $(INSTALDIR)/go

go: go.o

go.o: go.c

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) *.exe
	$(RM) *.asm
	$(RM) *.map
	$(RM) *.stackdump
	$(RM) *.core
