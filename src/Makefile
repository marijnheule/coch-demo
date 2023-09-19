all: 6gon-final gen-6gon-cube 6hole-final gen-6hole-cube

6gon-final: 6gon-final.c
	gcc 6gon-final.c -std=c99 -O2 -o 6gon-final

6hole-final: 6hole-final.c
	gcc 6hole-final.c -std=c99 -O2 -o 6hole-final

gen-6gon-cube: gen-6gon-cube.c
	gcc gen-6gon-cube.c -std=c99 -O2 -o gen-6gon-cube

gen-6hole-cube: gen-6hole-cube.c
	gcc gen-6hole-cube.c -std=c99 -O2 -o gen-6hole-cube

clean:
	rm 6gon-final gen-6gon-cube 6hole-final gen-6hole-cube
