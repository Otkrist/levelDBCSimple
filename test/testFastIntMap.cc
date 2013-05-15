#include <iostream>
#include "FastIntMap.h"
#include <sys/stat.h>
#include <unistd.h>
#include "string.h"
#include <assert.h>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atoi */

#define ABORT_ON_FAILURE 0
#define TEST_DB_NAME "testDB"
#define TEST_DIR_NAME "/tmp/"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

using namespace std;
void out(const char * msg)
{
  printf("%-100s... %s\n",msg,ANSI_COLOR_GREEN "[PASSED]" ANSI_COLOR_RESET);
}
void fail(const char * msg)
{
  printf("%-100s... %s\n",msg,ANSI_COLOR_RED "[FAILED]" ANSI_COLOR_RESET);
}
void check(int code,const char * msg)
{
  if(code==0)
    out(msg);
  else
  {
    fail(msg);
    if(ABORT_ON_FAILURE) assert(0);
  }
}

int randHelper()
{
  return rand() % 100000 + 1;
}

void testSingleReadWrite()
{
  FastIntMap * intMap = new FastIntMap();
  for(int i=0;i<10;i++)
  {
    int key = randHelper();
    int value = randHelper();
    char msg[200];

    intMap->put(key,value);
    int valueL = intMap->get(key);

    sprintf(msg,"KEY: [%d]. Correct value:[%d] should be fetched. Fetched Value:[%d]",key,value,valueL);
    check(valueL!=value,msg);
  }
  delete intMap;
}

/**
 * Create a dictionary full of random key value pairs.
 * Create a map for comparison, check the error rate. Print a result.
**/
void testMultipleReadWrites()
{
  int max_iter = 20000;
  char msg[200];
  int keys[max_iter]; // Make arrays to cheaply iterate over map //
  FastIntMap * intMap = new FastIntMap();
  map<int,int> * keyValMap = new map<int,int>();


  for(int i=0;i<max_iter;i++)
  {
    int key = randHelper();
    int value = randHelper();
    intMap->put(key,value);
    (*keyValMap)[key] = value;
    keys[i] = key;
  }

  int failedCount = 0;
  map<int,int>::iterator iter = keyValMap->begin();
  while(iter!=keyValMap->end())
  {
    int key = iter->first;
    int value = intMap->get(key);
    int valueL = iter->second;
    sprintf(msg,"KEY: [%d]. Correct value:[%d] should be fetched. Fetched Value:[%d]",key,value,valueL);
    if(value!=valueL) 
    {
      check(value!=valueL,msg);
      failedCount++;
    }
    iter++;
  }
  
  sprintf(msg,"Multiple Read Writes Failed for [%d] out of [%d] writes.",failedCount,(int)(keyValMap->size()));
  check(failedCount,msg);

  delete intMap;
  delete keyValMap;
}


int main()
{
  testSingleReadWrite();
  testMultipleReadWrites();
  return 0;
}
