#!/bin/bash
rm atm.o
rm bank.o
rm bank
rm atm
g++ -Wall -pedantic-errors -Werror -std=c++11 -c atm.cpp -o atm.o
g++ -Wall -pedantic-errors -Werror -std=c++11 -c bank.cpp -o bank.o
g++ atm.o -o atm -lcommoncpp -lccrtp -lpthread -lucommon
g++ bank.o -o bank -lcommoncpp -lccrtp -lpthread -lucommon
