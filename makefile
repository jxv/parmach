all:
	gcc src/par_mach.c -c -o src/par_mach.o -I./include -Wall -Werror -pedantic -std=c11 -g -O3 -lc
	ar rvs libpar_mach.a src/*.o
clean:
	rm libpar_mach.a src/*.o
install:
	cp include/*.h /usr/include
	cp libpar_mach.a /usr/lib/

gcw0:
	mipsel-gcw0-linux-uclibc-cc src/par_mach.c -c -o src/par_mach.o -I./include -Wall -Werror -pedantic -std=c11 -g -O2 -lc -I./include
	mipsel-gcw0-linux-uclibc-ar rvs libpar_mach.a src/*.o
install_gcw0:
	cp include/*.h /opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/include
	cp libpar_mach.a /opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/lib
