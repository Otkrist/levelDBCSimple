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

void printPoll(int location)
{
    if(location%1000000 == 0) 
    {
      printf(".");
      fflush(stdout);
    }
}

/** This function created multiple key value pairs and reads them and tests that they are okay **/
void testMultipleReadWrites()
{
  setup();

  unsigned int i=0;
  unsigned int count_iter=0;
  size_t len = 100;
  char keyBuffer[100];
  char * valueBuffer = (char *)malloc(len+1);
  char readBuffer[100];
  ssize_t read;
  clock_t begin, end;
  FILE * fp;
  
  printf("Initiating DB Writes. \n");
  fp = fopen("../sample_emails.txt", "r");
  begin = clock();
  while ((read = getline(&valueBuffer, &len, fp)) != -1) 
  {
    printPoll(count_iter);
    sprintf(keyBuffer,"%d",count_iter++);
    add_to_dict(TEST_DB_NAME,TEST_DIR_NAME,keyBuffer,valueBuffer);
  }
  end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  fclose(fp);

  printf("\nInitiating Control Sequence Writes.\n");
  count_iter = 0;
  fp = fopen("../sample_emails.txt", "r");
  begin = clock();
  while ((read = getline(&valueBuffer, &len, fp)) != -1) 
  {
    printPoll(count_iter);
    sprintf(keyBuffer,"%d",count_iter++);
  }
  end = clock();
  double time_spent_outside = (double)(end - begin) / CLOCKS_PER_SEC;
  fclose(fp);

  printf("\n For %d writes Total Time Taken:" ANSI_COLOR_RED "[%.3fs]" ANSI_COLOR_RESET ". removing additional [%fs] overhead.\n",count_iter,time_spent-time_spent_outside,time_spent_outside);
  printf("Time Taken per Write:" ANSI_COLOR_RED "[%fs]" ANSI_COLOR_RESET "\n",(time_spent-time_spent_outside)/count_iter);

  printf("Initiating DB Reads\n");
  begin = clock();
  for(i=0;i<count_iter;i++)
  {
    printPoll(i);
    sprintf(keyBuffer,"%d",i);
    get_from_dict_2(TEST_DB_NAME,TEST_DIR_NAME,keyBuffer,readBuffer,50);
  } 
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("Initiating DB Reads (Control)\n");
  begin = clock();
  for(i=0;i<count_iter;i++)
  {
    printPoll(i);
    sprintf(keyBuffer,"%d",i);
  }
  end = clock();
  time_spent_outside = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("\nFor %d reads Total Time Taken:" ANSI_COLOR_RED "[%.3fs]" ANSI_COLOR_RESET ". removing additional [%fs] overhead..\n",count_iter,time_spent-time_spent_outside,time_spent_outside);
  printf("Time Taken per Read:" ANSI_COLOR_RED "[%fs]" ANSI_COLOR_RESET "\n",(time_spent-time_spent_outside)/count_iter);
}

int main()
{
  testMultipleReadWrites();
  return 0;
}
