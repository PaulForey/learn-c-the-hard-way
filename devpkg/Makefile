PREFIX?=.
CFLAGS=-g -Wall -I/usr/include/apr-1.0/
LDFLAGS=-lapr-1 -pthread -laprutil-1

all: devpkg
devpkg: bstrlib.o db.o shell.o commands.o
install: all
	install -d $(DESTDIR)/$(PREFIX)/bin/
	install devpkg $(SDESTDIR)/$(PREFIX)/bin/
clean:
	rm -f *.o
	rm -f devpkg
	rm -rf *.dYSM
