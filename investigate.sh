if [ ! -d "obj" ]
then
    mkdir obj
fi
gcc -c -Wall -Wextra -pedantic -Iinclude src/sherlock.c -o obj/sherlock.o
gcc -c -Wall -Wextra -pedantic -Iinclude test.c -o obj/test.o

gcc obj/sherlock.o obj/test.o -o test

