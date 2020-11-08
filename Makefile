all:
	gcc -ansi -pedantic -Wall -fomit-frame-pointer -Werror fastcsv2json.c -o fastcsv2json

clean:
	rm -rf fastcsv2json

install:
	cp fastcsv2json /usr/bin		
	
	