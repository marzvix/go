# Makefile for go
PROJ = go
BENCH = ~/bench

TKDIR=$(BENCH)/toolkit

CC = gcc
CFLAGS = -g3 -O0 -Wall -Wextra -I $(TKDIR) -L $(TKDIR)

ARGS =
INSTALDIR = /usr/local/bin
MANDIR = /usr/share/man/man1

all: $(PROJ) man

man: $(PROJ).1.gz

$(PROJ).1.gz: $(PROJ).t2t
	txt2tags -o$(basename $@) $<
	gzip -q $(basename $@)

mancheck: $(PROJ).1.gz
	man ./$<

$(PROJ): $(PROJ).o $(TKDIR)/toolkit.lib

$(PROJ).o: $(PROJ).c

# $(TKDIR)/filetools.o  $(TKDIR)/txtools.o
# $(TKDIR)/toolkit.lib: $(TKDIR)/toolkit.o 
# 	ar rcs $@ $^

install: $(PROJ) $(PROJ).1.gz uninstall
	cp $(PROJ).1.gz $(MANDIR)/
	cp $(PROJ) $(INSTALDIR)/

.PHONY: uninstall
uninstall:
	$(RM) $(MANDIR)/$(PROJ).1.gz
	$(RM) $(INSTALDIR)/$(PROJ)

run:	$(PROJ)
	@echo cleaning...
	rm -rf ~/.config_/
	@echo
	@echo running...
	./$(PROJ) $(ARGS)
	@echo
	@echo dumping...
	cat ~/.config_/$(PROJ)/$(PROJ).conf

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) *.exe
	$(RM) *.asm
	$(RM) *.map
	$(RM) *.stackdump
	$(RM) *.core
