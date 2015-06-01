all:
	gcc src/char.c -c -o src/char.o -I./include -Wall -Werror -pedantic -std=c11 -g -O3 -lc
	gcc src/prim.c -c -o src/prim.o -I./include -Wall -Werror -pedantic -std=c11 -g -O3 -lc
	gcc src/combinator.c -c -o src/combinator.o -I./include -Wall -Werror -pedantic -std=c11 -g -O3 -lc
	gcc src/parmach.c -c -o src/parmach.o -I./include -Wall -Werror -pedantic -std=c11 -g -O3 -lc
	ar rvs libparmach.a src/*.o
clean:
	rm libparmach.a src/*.o
install:
	cp include/*.h /usr/include
	cp libparmach.a /usr/lib/

gcw0:
	mipsel-gcw0-linux-uclibc-cc src/char.c -c -o src/char.o -I./include -Wall -Werror -pedantic -std=c11 -g -O2 -lc -I./include
	mipsel-gcw0-linux-uclibc-cc src/prim.c -c -o src/char.o -I./include -Wall -Werror -pedantic -std=c11 -g -O2 -lc -I./include
	mipsel-gcw0-linux-uclibc-cc src/combinator.c -c -o src/combinator.o -I./include -Wall -Werror -pedantic -std=c11 -g -O2 -lc -I./include
	mipsel-gcw0-linux-uclibc-cc src/parmach.c -c -o src/parmach.o -I./include -Wall -Werror -pedantic -std=c11 -g -O2 -lc -I./include
	mipsel-gcw0-linux-uclibc-ar rvs libparmach.a src/*.o
install_gcw0:
	cp include/*.h /opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/include
	cp libparmach.a /opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/lib
