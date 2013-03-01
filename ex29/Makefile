#For detecting the os:
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	TESTSCRIPT=./tests/runtests.sh
	SO_EXT=.so
endif
ifeq ($(UNAME), MINGW32_NT-6.1)
	SHELL=powershell
	TESTSCRIPT=powershell -executionpolicy remotesigned ./tests/runtests.ps1
	SO_EXT=.dll
endif


CFLAGS=-g -O2 -Wall -Wextra -Isrc -DNDEBEG $(OPTFLAGS)
LIBS=-ldl $(OPTLIBS)
PREFIX?=/usr/local

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wildcard tests/*_test.c)
#TESTS=$(patsubst %.c,%,$(TEST_SRC))
TESTS=tests/ex29_linux_test

TARGET=build/libex29.a
SO_TARGET=$(patsubst %.a,%$(SO_EXT),$(TARGET))

# The Target Build
all: $(TARGET) $(SO_TARGET) tests

dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

#$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

build:
	@mkdir -p build
	@mkdir -p bin


# The Unit Tests
.PHONY: tests
tests: CFLAGS += $(TARGET) -ldl
tests: $(TESTS)
	@touch ./tests/tests.log
	@$(TESTSCRIPT)

valgrind:
	VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" $(MAKE)


# The Cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
#find . -name "*.gc*" -exec rm {} \;
#rm -rf 'find . -name "*.dSYM" -print'

# The Install
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# The Checker
BADFUNCS='[^_.>a-zA-Z0-9] (str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions:
	@grep $(BADFUNCS) $(SOURCES) || true
