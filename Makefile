bin/mvn : $(shell find src -type f)
	if [ ! -d ./bin ]; then \
		mkdir bin; \
	fi
	gcc -Wall -std=c99 -pedantic -O2 -Wextra -Wno-unused-result -Wpedantic -O0 src/main.c -o bin/mvn

run : bin/mvn
	./bin/mvn $(FILE)

debug : src/main.c
	if [ ! -d ./bin ]; then \
		mkdir bin; \
	fi
	gcc -g -Wall -std=c99 -pedantic -O2 -Wextra -Wno-unused-result -Wpedantic -O0 src/main.c -o bin/mvn
	gdb ./bin/mvn
