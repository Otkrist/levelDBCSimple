#include <stdio.h>
#include "assert.h"
#include "QLevelDB.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "string.h"
#include <time.h>
#include <stdlib.h>

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


void setup()
{
  assert(!del_dict(TEST_DB_NAME,TEST_DIR_NAME));
  assert(!mk_dict(TEST_DB_NAME,TEST_DIR_NAME));
}
void cleanup()
{
  assert(!del_dict(TEST_DB_NAME,TEST_DIR_NAME));
}
void out(const char * msg)
{
  printf("%-70s... %s\n",msg,ANSI_COLOR_GREEN "[PASSED]" ANSI_COLOR_RESET);
}
void fail(const char * msg)
{
  printf("%-70s... %s\n",msg,ANSI_COLOR_RED "[FAILED]" ANSI_COLOR_RESET);
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

int randomInt()
{
  return rand() % 10000000;
}

/** This function created multiple key value pairs and reads them and tests that they are okay **/
void testMultipleReadWrites()
{
  setup();

  /* initialize random seed: */
  srand (time(NULL));
  
  int i=0;
  const int count_iter =100000;
  const char ** keys = (const char **)(malloc(count_iter*(sizeof(char *))));
  const char ** keysX = (const char **)(malloc(count_iter*(sizeof(char *)))); // Only for testing
  char keyBuffer[100];
  char valueBuffer[100];
  char readBuffer[100];
  clock_t begin, end;

  begin = clock();
  for(i=0;i<count_iter;i++)
  {
    sprintf(keyBuffer,"Key%d",randomInt());
    sprintf(valueBuffer,"Value%d",randomInt());
    char * key = (char *)malloc(strlen(keyBuffer)+1);
    strcpy(key,keyBuffer);
    keys[i] = key;
  
    add_to_dict(TEST_DB_NAME,TEST_DIR_NAME,keyBuffer,valueBuffer);
  }
  end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  begin = clock();
  for(i=0;i<count_iter;i++)
  {
    sprintf(keyBuffer,"Key%d",randomInt());
    sprintf(valueBuffer,"Value%d",randomInt());
    char * key = (char *)malloc(strlen(keyBuffer)+1);
    strcpy(key,keyBuffer);
    keysX[i] = key;
  }
  end = clock();
  double time_spent_outside = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("For %d writes Total Time Taken:" ANSI_COLOR_RED "[%.3fs]" ANSI_COLOR_RESET ". removing additional [%fs] overhead.\n",count_iter,time_spent-time_spent_outside,time_spent_outside);
  printf("Time Taken per Write:" ANSI_COLOR_RED "[%fs]" ANSI_COLOR_RESET "\n",(time_spent-time_spent_outside)/count_iter);

  begin = clock();
  for(i=0;i<count_iter;i++)
  {
    get_from_dict_2(TEST_DB_NAME,TEST_DIR_NAME,keys[i],readBuffer,50);
  } 
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  begin = clock();
  for(i=0;i<count_iter;i++)
  {
    keys[i] = "ds";
  }
  end = clock();
  time_spent_outside = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("For %d reads Total Time Taken:" ANSI_COLOR_RED "[%.3fs]" ANSI_COLOR_RESET ". removing additional [%fs] overhead..\n",count_iter,time_spent-time_spent_outside,time_spent_outside);
  printf("Time Taken per Read:" ANSI_COLOR_RED "[%fs]" ANSI_COLOR_RESET "\n",(time_spent-time_spent_outside)/count_iter);
}

int main()
{
  testMultipleReadWrites();
  return 0;
}
