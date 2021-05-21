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
BIN_PHASE1 = phase1
BIN_PHASE2 = dns_svr
LOG = dns_svr.log

OBJ = input_handler.o log_handler.o packet.o byte_converter.o timestamp.o \
server.o upstream_connection.o client_connection.o

# Handles "$ make" and "$ make all"
all: $(BIN_PHASE1) $(BIN_PHASE2)

# For phase 1
$(BIN_PHASE1): phase1.c $(OBJ)
	$(CC) -o $(BIN_PHASE1) phase1.c $(OBJ) $(CFLAGS)

# For phase 2
$(BIN_PHASE2): main.c $(OBJ)
	$(CC) -o $(BIN_PHASE2) main.c $(OBJ) $(CFLAGS)

# Wildcard rule to make any .o file,
# given a .c and .h file with the same leading filename component
%.o: %.c %.h
	$(CC) -c $< $(CFLAGS) -g

# Removes the executable file, object files and log file
clean:
	rm -f $(BIN_PHASE1) $(BIN_PHASE2) $(OBJ) $(LOG)

# "clean" and "all" don't actually create files called "clean" and "all"
# and are therefore "Phony Targets"
.PHONY: clean all