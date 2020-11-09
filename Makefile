all:
	gcc -ansi -pedantic -Wall -Werror -fomit-frame-pointer -O1 fastcsv2json.c -o fastcsv2json

clean:
	rm -rf fastcsv2json

install:
	cp fastcsv2json /usr/bin		
	
	