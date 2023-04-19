run : src/mvn.c
	if [ ! -d "~/Dropbox" ]; then \
		echo "Dir exists"; \
	fi
	gcc -Wall -std=c99 -pedantic -O2 -Wextra -Wno-unused-result -Wpedantic -O0 src/mvn.c -o bin/mvn && echo "Execution starting..." && ./bin/mvn
