#compiler & flags
CC = gcc
CFLAGS = -Wall -Iinclude -g -lm -fPIC -lpthread
LDFLAGS = -shared -lm

#directories

SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
LIB_DIR = lib
QTC_DIR = QTC
PGM_DIR = PGM
#so target
LIB_NAME = libPGMcompressor.so
LIB_TARGET = $(LIB_DIR)/$(LIB_NAME)

# standard main executable
MAIN_SRC = $(SRC_DIR)/main.c
MAIN_TARGET = $(BIN_DIR)/PGM2QTC

#libmain executable

LIBMAIN_SRC = $(SRC_DIR)/libmain.c
LIBMAIN_TARGET = $(BIN_DIR)/PGM2QTCalt

# Collect all .c files in src directory

SRCS = $(wildcard $(SRC_DIR)/*.c)

#objs

OBJS = $(filter-out $(BIN_DIR)/libmain.o, $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o))

#exclude main.o for the shared lib

LIB_OBJS = $(filter-out $(BIN_DIR)/main.o $(BIN_DIR)/libmain.o, $(OBJS))

#default rule 
all: $(QTC_DIR) $(PGM_DIR) $(MAIN_TARGET) $(LIB_TARGET) $(LIBMAIN_TARGET) clean-objects

#Linking of standard main

$(MAIN_TARGET): $(OBJS) | $(BIN_DIR)
	$(CC)  -o $@ $(OBJS) $(CFLAGS)

# Create shared library

$(LIB_TARGET): $(LIB_OBJS) | $(LIB_DIR)
	$(CC) $(LDFLAGS) -o $@ $(LIB_OBJS)

#linking the demo app
$(LIBMAIN_TARGET): $(LIBMAIN_SRC) | $(BIN_DIR)
	$(CC)  -o $@ $< -L$(LIB_DIR) -lPGMcompressor -Wl,-rpath,$(LIB_DIR) $(CFLAGS)


# Compile object files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC)  -c $< -o $@ $(CFLAGS)

#create the bin dir if it doesn't exist

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(QTC_DIR):
	mkdir -p $(QTC_DIR)

$(PGM_DIR):
	mkdir  -p $(PGM_DIR)

clean-objects: 
	rm -rf $(BIN_DIR)/*.o

# Clean up build files
clean:
	rm -rf $(BIN_DIR)/*.o $(MAIN_TARGET) $(LIB_TARGET) $(LIBMAIN_TARGET)

# Rebuild everything
rebuild: clean all
.PHONY: all clean rebuild