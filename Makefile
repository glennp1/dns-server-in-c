# COMP30023 Computer Systems
# 2021 Semester 1
#
# Project 2 Makefile
# Implementation by Glenn Phillips (820624)
#
# Sourced from Sample Makefile
# COMP30023 Computer Systems, 2021 Semester 1, Project 2, helper-files

# Variables
CC = gcc
CFLAGS = -I -Wall
EXE = phase1 # todo Change this to dns_svr after done testing phase 1
OBJ = byte_converter.o timestamp.o
LOG = dns_svr.log

# Handles "$ make" and "$ make all"
all: main.c $(OBJ)
	$(CC) -o $(EXE) main.c $(OBJ) $(CFLAGS)

# Wildcard rule to make any .o file,
# given a .c and .h file with the same leading filename component
%.o: %.c %.h
	$(CC) -c $< $(CFLAGS) -g

# Removes the executable file, object files and log file
clean:
	rm -f $(EXE) $(OBJ) $(LOG)

# "clean" and "all" don't actually create files called "clean" and "all"
# and are therefore "Phony Targets"
.PHONY: clean all