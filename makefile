all:
	gcc src/parmach.c -c -o src/parmach.o -I./include -Wall -Werror -pedantic -std=c11 -g -O3 -lc
	ar rvs libparmach.a src/*.o
clean:
	rm libparmach.a src/*.o
install:
	cp include/*.h /usr/include
	cp libparmach.a /usr/lib/

gcw0:
	mipsel-gcw0-linux-uclibc-cc src/parmach.c -c -o src/parmach.o -I./include -Wall -Werror -pedantic -std=c11 -g -O2 -lc -I./include
	mipsel-gcw0-linux-uclibc-ar rvs libparmach.a src/*.o
install_gcw0:
	cp include/*.h /opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/include
	cp libparmach.a /opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/lib
