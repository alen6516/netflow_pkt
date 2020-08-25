
all: a.out

# declare variables
SHELL = //bin/bash
CC = gcc
INSTDIR = /tmp
CFLAGS = -g -Wall -ansi
LIBS += -framework CoreFoundation

a.out: logger.o main.o
	$(CC) -o $@ $^

logger.o: logger.c
	$(CC) -c logger.c

main.o: main.c
	$(CC) -c main.c

clean:
	rm *.o *.out

install: mytest
	@echo "here execute shell script";
	@#if [ -d $(INSTDIR) ] \
	#then	\
	#    cp hello.out $(INSTDIR) \
	#    chmod a+x $(INSTDIR)/hello.out \
	#    chmod og-w $(INSTDIR)/hello.out \
	#    echo "Installed in $(INSTDIR)" \
	#else \
	#    echo "Sorry, $(INSTDIR) does not exist" \
	#fi
