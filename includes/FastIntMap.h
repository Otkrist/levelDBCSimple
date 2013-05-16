/**
  Interface for a fast hashmap for integers.
  
  @author Otkrist Gupta
**/
#define TABLE_COLUMN_COUNT 2
#include "FastHashMap.h"
#include<map>
using namespace std;
template <typename INTTYPE> class FastIntMap : public FastHashMap<INTTYPE,INTTYPE>
{
  protected:
  unsigned int a,b,p;

  int _tableSize;
  INTTYPE ** _table;
  map<INTTYPE,INTTYPE> * collisionMap;

  unsigned int hash(int key);

  public:
  FastIntMap() 
  {
    _tableSize = 32768;
    init();
  }
  FastIntMap(int tableSize) 
  {
    _tableSize = tableSize;
    init();
  }
  ~FastIntMap()
  {
    for(int i=0;i<TABLE_COLUMN_COUNT;i++)
    {
      delete _table[i];
    }
    delete _table;
    delete collisionMap;
  }


  public:
  virtual void init();
  virtual void put(const INTTYPE key,const INTTYPE value);
  virtual const INTTYPE get(const INTTYPE key);
  
};
