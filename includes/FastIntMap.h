/**
  Interface for a fast hashmap for integers.
  
  @author Otkrist Gupta
**/
#define TABLE_COLUMN_COUNT 2
#include "FastHashMap.h"
#include<map>
using namespace std;
class FastIntMap : public FastHashMap<int,int>
{
  protected:
  unsigned int a,b,p;

  int _tableSize;
  int ** _table;
  map<int,int> * collisionMap;

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
  ~FastIntMap();

  public:
  virtual void init();
  virtual void put(const int key,const int value);
  virtual const int get(const int key);
  
};
