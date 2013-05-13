#include "QLevelDB.h"
#include "DictFunctions.h"

/**
  * Functional implementation of class Dict in connect.cpp
**/

int mk_dict(const char * name, const char * pathname)
{
  return mk_dict_cc(name,pathname);
}

int del_dict(const char * name,const char * pathname)
{
  return del_dict_cc(name,pathname);
}

int is_dict(const char * name, const char * pathname)
{
  return is_dict_cc(name,pathname);
}

int add_to_dict(const char * name, const char * pathname, const char * key,const char * value)
{
  return add_to_dict_cc(name,pathname,key,value); 
}

int del_from_dict(const char * name, const char * pathname, const char * key)
{
  return del_from_dict_cc(name,pathname,key);
} 

int get_from_dict(const char * name, const char * pathname, const char * key,char ** value)
{
  return get_from_dict_cc(name,pathname,key,value);
}

int get_from_dict_2(const char * name, const char * pathname, const char * key,char * buffer, int buffer_size)
{
  return get_from_dict_2_cc(name,pathname,key,buffer,buffer_size);
}

void cleanUp()
{
  cleanUp_cc();
}

