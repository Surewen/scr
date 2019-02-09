#!/bin/bash
g++ -I. atm.cpp atm_communication.cpp atm_service.cpp -o atm.exe
g++ -I. bank.cpp -o bank.exe