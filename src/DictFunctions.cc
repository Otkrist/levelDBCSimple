#include "constants.h"
#include "DictFunctions.h"
#include "DBManager.h"

/**
  * Functional implementation of class Dict in connect.cpp
**/

int mk_dict_cc(const char * name, const char * pathname)
{
  LevelDict * d = DBManager::getInstance()->getOrCreate(name,pathname);
  return d->init();
}

int del_dict_cc(const char * name,const char * pathname)
{
  LevelDict * d = DBManager::getInstance()->getOrCreate(name,pathname);
  return d->remove();
}

int is_dict_cc(const char * name, const char * pathname)
{
  LevelDict * d = DBManager::getInstance()->getOrCreate(name,pathname);
  return d->exists();
}

int add_to_dict_cc(const char * name, const char * pathname, const char * key,const char * value)
{
  LevelDict * d = DBManager::getInstance()->getOrCreate(name,pathname);
  return d->put(key,value);
}

int del_from_dict_cc(const char * name, const char * pathname, const char * key)
{
  LevelDict * d = DBManager::getInstance()->getOrCreate(name,pathname);
  return d->removeKey(key);
} 

int get_from_dict_cc(const char * name, const char * pathname, const char * key,char ** value)
{
  LevelDict * d = DBManager::getInstance()->getOrCreate(name,pathname);
  return d->get(key,value);
}

int get_from_dict_2_cc(const char * name, const char * pathname, const char * key,char * buffer,int buffer_size)
{
  LevelDict * d = DBManager::getInstance()->getOrCreate(name,pathname);
  return d->get(key,buffer,buffer_size);
}

int cleanUp_cc()
{
  return DBManager::getInstance()->cleanUp();
}
