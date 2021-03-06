#For detecting the os:
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	TESTPAT=*_linux_test
	TESTSCRIPT=./tests/runtests.sh
	TESTFLAGS=-ldl
	SO_EXT=.so
endif
ifeq ($(UNAME), MINGW32_NT-6.1)
	TESTPAT=*_win_test
	TESTSCRIPT=powershell -executionpolicy remotesigned -NoProfile ./tests/runtests.ps1
	TESTFLAGS=
	SO_EXT=.dll
endif


CFLAGS=-g -O2 -Wall -Wextra -Isrc $(OPTFLAGS)
LIBS=-ldl $(OPTLIBS)
PREFIX?=/usr/local

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wildcard tests/$(TESTPAT).c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

TARGET=build/libex29.a
SO_TARGET=$(patsubst %.a,%$(SO_EXT),$(TARGET))

# The Target Build
all: $(TARGET) $(SO_TARGET) tests

dev: CFLAGS += -DDEBUG
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
tests: CFLAGS += $(TARGET) $(TESTFLAGS)
tests: $(TESTS)
	@touch ./tests/tests.log
	@TESTPAT=$(TESTPAT) $(TESTSCRIPT)

ifeq ($(UNAME), Linux)
memtest:
	VALGRIND="valgrind --track-origins=yes" $(MAKE)
endif
ifeq ($(UNAME), MINGW32_NT-6.1)
memtest:
	#DRMEMORY="C:\Program Files (x86)\Dr. Memory\bin\drmemory.exe -batch " $(MAKE)
	DRMEMORY="drmemory.exe -batch " $(MAKE)
endif

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
