#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "QLevelDB.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "string.h"

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

/** This function creates a Db, checks if the file exists physically, then deletes it and tests that files were removed **/
void testDBCreationDeletion()
{
  printf("Running Dict Creation/Deletion Tests\n");

  struct stat s;
  char * dbName = TEST_DB_NAME;
  char * pathName = TEST_DIR_NAME;
  char fullName[50]; // This is okay since this is a test
  char msg[500];
  strcpy(fullName,pathName);
  strcpy(fullName+strlen(pathName),dbName);

  // Test 1. Check if the dict is created and exists using apis //
  check(del_dict(TEST_DB_NAME,TEST_DIR_NAME),"Deleting Exisiting DB (if Any)");
  sprintf(msg,"Testing mk_dict for %s",fullName);
  check(mk_dict(TEST_DB_NAME,TEST_DIR_NAME),msg);
  check(is_dict(dbName,pathName),"Testing is_dict");
  
  // Test 2. Check if the directory exists physically //
  int err = stat(fullName, &s);
  if(err!=-1 && S_ISDIR(s.st_mode)) out("DB Directory Verified.");
  sprintf(msg,"ls %s",fullName);

  // Test 3. Remove and check if the directory exists physically //
  check(del_dict(TEST_DB_NAME,TEST_DIR_NAME),"Deleting DB. Testing del_dict."); 
  err = stat(fullName, &s);
  check((err!=-1 && S_ISDIR(s.st_mode)),"Verifying DB directory deletion.");
  
  check(!is_dict(dbName,pathName),"Testing is_dict for deleted dict");
}

/** This function tests that correct string is read after write and there is an error if key does not exist **/
void testReadWrites()
{
  setup();
  char * key = "star";
  char * value = "burst";
  char buffer[10];
  
  // try reading a random key
  sprintf(buffer,"XXX");
  check(!get_from_dict_2(TEST_DB_NAME,TEST_DIR_NAME,"laBlah",buffer,10),"Attempting Read of a random key. This should error.");
  check(!strcmp(buffer,value),"Read string should not have same value");

  // Create a value in dict, read it and compare
  check(add_to_dict(TEST_DB_NAME,TEST_DIR_NAME,key,value),"Attempting write of a single string.");
  check(get_from_dict_2(TEST_DB_NAME,TEST_DIR_NAME,key,buffer,10),"Attempting Read of stored string");
  check(strcmp(buffer,value),"Read string has same value");

  // Remove the value from dict and try reading again
  sprintf(buffer,"XXX");
  check(del_from_dict(TEST_DB_NAME,TEST_DIR_NAME,key),"Attempting deletion of key from dict");
  check(!get_from_dict_2(TEST_DB_NAME,TEST_DIR_NAME,key,buffer,10),"Attempting Read of deleted key. This should error.");
  check(!strcmp(buffer,value),"Read string should not have same value");
  
  cleanup();
}

/** This function created multiple key value pairs and reads them and tests that they are okay **/
void testMultipleReadWrites()
{
  setup();

  int i=0;
  char msg[200];
  char buffer[20];
  const char const * keys[] = {"alpha","beta","gamma","delta","theta","kappa","lambda"};
  const char const * values[] = {"apple","pear","banana","orange","melon","mango","guava"};

  for(i=0;i<7;i++)
  {
    const char * key = keys[i];
    const char * value = values[i];
    sprintf(msg,"Writing the key:[%s] and value:[%s] in dict",key,value);
    check(add_to_dict(TEST_DB_NAME,TEST_DIR_NAME,key,value),msg);
  }
  
  for(i=0;i<7;i++)
  {
    const char * key = keys[i];
    const char * value = values[i];
    sprintf(msg,"Reading the key:[%s] from dict",key);
    check(get_from_dict_2(TEST_DB_NAME,TEST_DIR_NAME,key,buffer,20),msg);
    sprintf(msg,"Correct value:[%s] should be fetched. Fetched Value:[%s]",value,buffer);
    check(strcmp(value,buffer),msg);
  }
}

int main()
{
  testDBCreationDeletion();
  testReadWrites();
  testMultipleReadWrites();
  return 0;
}
