PREFIX ?= i686-w64-mingw32-

CC := $(PREFIX)gcc
STRIP := $(PREFIX)strip

dongletools.exe: dongletools.c
	$(CC) -std=gnu99 -O2 -o $@ $< RCGrandDogW32.dll
	$(STRIP) -s $@
