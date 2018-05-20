

all:
	gcc -g3 -DDEBUG -Wall -Wextra -Wno-unused-parameter -std=c99 DriverMem.c DriverMem_TestApp.c -o DriverMem_TestApp

unit_test:
	gcc -g3 -DDEBUG -Wno-unused-parameter -fprofile-arcs -ftest-coverage -std=c99 DriverMem.c DriverMem_UnitTest.c -o DriverMem_UnitTest
	
clean:
	rm -rf  ./*.o
	rm -rf  ./*.exe
	rm -rf  ./*.stackdump
	rm -rf  ./*.gcda
	rm -rf  ./*.gcno

    
