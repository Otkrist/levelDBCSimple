# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=gcc
CXX=g++
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS=-ggdb -Wall -fPIC -O2
CXXFLAGS=-ggdb -Wall -fPIC -O2
CINCLUDES=-I ./includes/
CXXINCLUDES=-I ../leveldb/include/ -I ./includes/
CLDINCLUDES=../leveldb/libleveldb.a -lpthread -O2
CXXLDINCLUDES=../leveldb/libleveldb.a -lpthread -O2

COBJS=bin/QLevelDB.c.o
CXXOBJS=bin/LevelDict.cc.o bin/DictFunctions.cc.o bin/DBManager.cc.o
TEST_DIR=test
SRC_DIR=src
TARGET_DIR=bin
TEST_TARGET_DIR=$(TEST_DIR)/$(TARGET_DIR)

CXXLIB=bin/leveldbcxx.a
CLIB=bin/leveldbc.a

# The test object compilation logic. We have made it super general and any test file with right extension gets compile to correct directory with correct flags
TEST_CSRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_CXXSRCS = $(wildcard $(TEST_DIR)/*.cc)
TEST_BINS = $(TEST_CXXSRCS:$(TEST_DIR)/%.cc=$(TEST_TARGET_DIR)/%.cc.exe) $(TEST_CSRCS:$(TEST_DIR)/%.c=$(TEST_TARGET_DIR)/%.c.exe)

all:	$(CXXLIB) $(CLIB) $(TEST_BINS)

# Compiles a .cc file in tests #
$(TEST_TARGET_DIR)/%.cc.exe: $(TEST_DIR)/%.cc $(CXXLIB)
	$(CXX) $(CXXFLAGS) $(CXXLIB) $(CXXLDINCLUDES) $(CXXINCLUDES) $< -o $@

# Compiles a .c file in tests #
$(TEST_TARGET_DIR)/%.c.exe: $(TEST_DIR)/%.c $(CXXLIB) $(CLIB)
	$(CC) $(CFLAGS) $(CLIB) $(CXXLIB) $(CXXLDINCLUDES) $(CINCLUDES) $< -o $@

$(CXXLIB): $(CXXOBJS) 
	$(CXX) $(CXXOBJS) $(CXXLDINCLUDES) -shared -fPIC -o $(CXXLIB)

$(CLIB): $(CXXLIB) $(COBJS)
	$(CC) $(CXXLIB) $(COBJS) $(CLDINCLUDES) -shared -fPIC -o $(CLIB)

# Compiles any .cc file to .o file in src #
$(TARGET_DIR)/%.cc.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(CXXINCLUDES) -c $< -o $@ 

# Compiles any .c file to .o file in src #
$(TARGET_DIR)/%.c.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(CINCLUDES) -c $< -o $@ 

clean:
	rm -rf bin/* $(TEST_TARGET_DIR)/*
