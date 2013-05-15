#include <iostream>
#include "FastIntMap.h"
#include <sys/stat.h>
#include <unistd.h>
#include "string.h"
#include <assert.h>
#include <math.h>

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
  return rand() % 10000000 + 1;
}

/**
 * Create a dictionary full of random key value pairs.
 * Create a map for comparison, check the error rate. Print a result.
**/
void testMultipleReadWrites(const int max_iter,const int tableSize)
{
  int keys[max_iter]; // Make arrays to cheaply iterate over map //
  double sum = 0;
  FastIntMap * intMap = new FastIntMap(tableSize);
  clock_t begin, end;

  begin = clock();
  for(int i=0;i<max_iter;i++)
  {
    int key = randHelper();
    int value = randHelper();
    intMap->put(key,value);
    keys[i] = key;
    sum = sum + value + key;
  }
  end = clock();
  double time_spent = (double)(end - begin);

  double sum2 = 0;
  begin = clock();
  for(int i=0;i<max_iter;i++)
  {
    int key = randHelper();
    int value = randHelper();
    key = keys[i];
    sum2 = sum2 + value + key;
  }
  end = clock();
  double time_spent_outside = (double)(end - begin);

  printf("For %d writes Total Time Taken:" ANSI_COLOR_RED "[%.3fs]" ANSI_COLOR_RESET ". removing additional [%fs] overhead.\n",max_iter,time_spent-time_spent_outside,time_spent_outside);
  printf("Time Taken per Write:" ANSI_COLOR_RED "[%f cycles]" ANSI_COLOR_RESET "\n", (time_spent-time_spent_outside)/max_iter);
  printf("Dummy Sums %f %f \n",sum,sum2);

  sum = 0;
  begin = clock();
  for(int i=0;i<max_iter;i++)
  {
    int key = keys[i];
    int value = intMap->get(key);
    sum = sum + key + value;
  }
  end = clock();
  time_spent = (double)(end - begin);
  
  sum2 = 0;
  begin = clock();
  for(int i=0;i<max_iter;i++)
  {
    int key = keys[i];
    sum2 = sum2 + key;
  }
  end = clock();
  time_spent_outside = (double)(end - begin);
  
  printf("For %d reads Total Time Taken:" ANSI_COLOR_RED "[%.3fs]" ANSI_COLOR_RESET ". removing additional [%fs] overhead.\n",max_iter,time_spent-time_spent_outside,time_spent_outside);
  printf("Time Taken per Read:" ANSI_COLOR_RED "[%f cycles]" ANSI_COLOR_RESET "\n", (time_spent-time_spent_outside)/max_iter);
  printf("Dummy Sums %f %f \n",sum,sum2);
  delete intMap;
}


int main()
{
  for(int i=1;i<6;i++)
  {
    testMultipleReadWrites(pow(10,i),pow(2,20));
  }
  return 0;
}
