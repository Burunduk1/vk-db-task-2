flags = -std=c++17 -Wall -Wextra -O3 # -pg 

corecpp = randomNumbers.cpp

main: Makefile $(corecpp) main.cpp *.h 
	g++-8 $(flags) main.cpp $(corecpp) -o main #2>&1 | grep -iE "(warning)|(error)|(note)"
