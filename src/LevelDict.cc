#include "constants.h"
#include "LevelDict.h"
#include <stdlib.h>

/**
  This class encapsulates the levelDb operations we are required to make
  The main reason to encapsulate is to add efficiency, not requiring us to open the db repeatedly.
  And allow for easy migration in future.

  @author Otkrist Gupta
**/


LevelDict::LevelDict(const char * name,const char * pathname)
{
  _name = name;
  _pathname = pathname;
  _fullname = getFullName(name,pathname);
  db = NULL;
}

/**
 * Creates a fullName (concatenated pathname and fullname).
 * 
**/
const char * LevelDict::getFullName(const char * name,const char * pathname)
{
  char * fullname = new char[strlen(name)+strlen(pathname)];
  strcpy(fullname,pathname);
  strcpy(fullname+strlen(pathname),name);
  return fullname;
}

const char * LevelDict::getName() { return _name; }
const char * LevelDict::getFullName() { return _fullname; }
  
bool LevelDict::isInitialized() { return db!=NULL; }

/**
* Initializes. Creates a new dict.
**/
int LevelDict::init()
{
  if(!isInitialized())
  {
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, _fullname, &db);
    if(!status.ok()) return CREATE_FAILURE;
  }
  return CREATE_OK;
}
int LevelDict::exists()
{
  if(!isInitialized())
  {
    leveldb::Options options;
    options.create_if_missing = false;
    options.error_if_exists = false;
    leveldb::Status status = leveldb::DB::Open(options, _fullname, &db);
    if(!status.ok()) return DICT_EXISTS_FAILURE;
  } 
  return DICT_EXISTS_OK; 
}

int LevelDict::remove()
{
  if(isInitialized())
  {
    delete db;
    db = NULL;
  }

  if(!isInitialized())
  {
    leveldb::Options options;
    leveldb::Status status = leveldb::DestroyDB(_fullname,options);
    if(status.ok()) return DICT_REMOVE_OK;    
  }
  return DICT_REMOVE_FAILURE;
}


int LevelDict::put(const char * key,const char * value)
{
  if(CREATE_OK == init())
  {
    leveldb::Status status = db->Put(leveldb::WriteOptions(), key, value); 
    return status.ok() ? INSERT_OK : INSERT_FAILURE;
  }
  return INSERT_FAILURE;
}

int LevelDict::removeKey(const char * key)
{
  if(CREATE_OK == init())
  {
    leveldb::Status status = db->Delete(leveldb::WriteOptions(), key);
    return status.ok() ? DELETE_OK : DELETE_FAILURE;
  }
  return DELETE_FAILURE;
}

int LevelDict::get(const char * key, char ** value)
{
  if(CREATE_OK == init())
  {
    std::string str;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &str);
    char *cstr = new char[str.length() + 1];
    if(status.ok()) 
    {
      strcpy(cstr, str.c_str());
      *value = cstr;
      return FETCH_OK;
    }
  }
  return FETCH_FAILURE;
}

int LevelDict::get(const char * key, char * buffer, int buffer_size)
{
  if(CREATE_OK == init())
  {
    std::string str;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &str);
    if(status.ok())
    {
      strcpy(buffer, str.c_str());
      return FETCH_OK;
    }
  }
  return FETCH_FAILURE;
}

int LevelDict::put(const char * key,const int valueInt)
{
  if(CREATE_OK == init())
  {
    // This should be 66 for 8 byte platforms. But keeping 64 for optimal performance //
    char buffer[64];
    snprintf(buffer, 64, "%d", valueInt);
    leveldb::Status status = db->Put(leveldb::WriteOptions(), key, buffer);
    return status.ok() ? INSERT_OK : INSERT_FAILURE;
  }
  return INSERT_FAILURE;
}
int LevelDict::get(const char * key, int * value)
{
  if(CREATE_OK == init())
  {
    std::string str;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &str);
    if(status.ok())
    {
      *value = atoi(str.c_str());
      return FETCH_OK;
    }
  }
  return FETCH_FAILURE;
}

int LevelDict::put(const int key,const char * value)
{
  if(CREATE_OK == init())
  {
    char buffer[64];
    snprintf(buffer, 64, "%d", key);
    leveldb::Status status = db->Put(leveldb::WriteOptions(), buffer, value);
    return status.ok() ? INSERT_OK : INSERT_FAILURE;
  }
  return INSERT_FAILURE;
}
int LevelDict::get(const int key, char * value, int buffer_size)
{
  if(CREATE_OK == init())
  {
    char buffer[64];
    snprintf(buffer, 64, "%d", key);
    std::string str;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), buffer, &str);
    if(status.ok())
    {
      strcpy(buffer, str.c_str());
      return FETCH_OK;
    }
  }
  return FETCH_FAILURE;
}

 
LevelDict::~LevelDict() 
{ 
  if(db!=NULL) delete db;
  if(_fullname!=NULL) delete _fullname;
}
