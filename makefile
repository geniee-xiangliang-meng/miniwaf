main:
	gcc -Wall -o bin/miniwaf miniwaf.c
install:
	cp -f bin/miniwaf /usr/local/bin/miniwaf
	cp -f conf/miniwaf.conf /usr/local/etc/miniwaf.conf
clean:
	rm -f bin/miniwaf
