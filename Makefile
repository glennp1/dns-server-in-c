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
EXE = phase1 # Change this to dns_svr after done testing phase 1
OBJ =
LOG = dns_svr.log

# Running "make" with no argument will make the first target in the file
all: $(BIN_PHASE1) $(BIN_PHASE2)

$(BIN_PHASE2): main.c $(OBJ)
	$(CC) -o $(EXE) main.c $(OBJ) $(CFLAGS)

# Wildcard rule to make any  .o  file,
# given a .c and .h file with the same leading filename component
%.o: %.c %.h
	$(CC) -c $< $(CFLAGS) -g

# Removes the executable files, object files and log file
clean:
	rm -f $(BIN_PHASE1) $(BIN_PHASE2) $(OBJ) $(LOG)

# "clean" and "all" don't actually create files called "clean" and "all"
# and are therefore "Phony Targets"
.PHONY: clean all