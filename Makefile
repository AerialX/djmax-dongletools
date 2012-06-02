dongletools.exe: dongletools.c
	i486-mingw32-gcc -std=gnu99 -O2 -o dongletools.exe dongletools.c RCGrandDogW32.dll
