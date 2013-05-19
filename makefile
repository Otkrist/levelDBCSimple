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

COBJS=bin/QLevelDB.o bin/libleveldbcxx.a
CXXOBJS=bin/LevelDict.o bin/DictFunctions.o bin/DBManager.o bin/FastIntMap.o
TEST_DIR=test
SRC_DIR=src
TARGET_DIR=bin
TEST_TARGET_DIR=$(TEST_DIR)/$(TARGET_DIR)

CXXSO=bin/libleveldbcxx.so.1
CXXLIB=bin/libleveldbcxx.a
CLIB=bin/libleveldbc.a

# The test object compilation logic. We have made it super general and any test file with right extension gets compile to correct directory with correct flags
TEST_CSRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_CXXSRCS = $(wildcard $(TEST_DIR)/*.cc)
TEST_BINS = $(TEST_CXXSRCS:$(TEST_DIR)/%.cc=$(TEST_TARGET_DIR)/%.cc.exe) $(TEST_CSRCS:$(TEST_DIR)/%.c=$(TEST_TARGET_DIR)/%.c.exe)

all: $(CXXLIB) $(CLIB) $(TEST_BINS)

# Compiles a .cc file in tests #
$(TEST_TARGET_DIR)/%.cc.exe: $(TEST_DIR)/%.cc $(CXXLIB)
	$(CXX) -o $@ $< $(CXXFLAGS) $(CXXLIB) $(CXXLDINCLUDES) $(CXXINCLUDES)

# Compiles a .c file in tests #
$(TEST_TARGET_DIR)/%.c.exe: $(TEST_DIR)/%.c $(CXXLIB) $(CLIB)
	$(CC) -o $@  $< $(CFLAGS) $(CLIB) $(CXXLIB) $(CXXLDINCLUDES) $(CINCLUDES) -lstdc++

$(CXXSO): $(CXXOBJS) 
	$(CXX) $(CXXOBJS) $(CXXLDINCLUDES) -shared -fPIC -Wl,-soname -Wl,$(CXXSO) -o $(CXXSO)

$(CXXLIB): $(CXXOBJS) 
	ar -rs -o $@ $(CXXOBJS)

$(CLIB): $(CXXOBJS) $(COBJS)
	#$(CC) $(CXXLIB) $(COBJS) $(CLDINCLUDES) -shared -fPIC -o $(CLIB)
	ar -rs -o $@ $(CXXOBJS) $(COBJS)

# Compiles any .cc file to .o file in src #
$(TARGET_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(CXXINCLUDES) -c $< -o $@ 

# Compiles any .c file to .o file in src #
$(TARGET_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(CINCLUDES) -c $< -o $@ 

clean:
	rm -rf bin/*.* $(TEST_TARGET_DIR)/*.*
