#include "FastIntMap.h"
#include <assert.h>


// The one millionth prime number
#define DEFAULT_PRIME 15485863
/**
 * Helper function to return random integers between the range integers.
 * Checks before hand if this is possible. Aborts if not possible.
**/
unsigned int getRand(unsigned int lowerBound,unsigned int upperBound)
{
  assert(RAND_MAX>upperBound);
  return ((rand() % (upperBound - lowerBound)) + lowerBound);
}

/**
 * Get the prime number greater than tableSize. If not possible to do so - fail.
 * In future we will allow for auto generation of primes rather than hard coding them.
**/
unsigned int getPrimeModulous(int tableSize)
{
  assert(tableSize<DEFAULT_PRIME);
  return DEFAULT_PRIME;
}

void FastIntMap::init()
{
  // Initialize the table 
  collisionMap = new map<int,int>();
  _table = new int*[TABLE_COLUMN_COUNT];
  for(int i=0;i<TABLE_COLUMN_COUNT;i++)
  {
    _table[i] = new int[_tableSize];
    for(int k=0;k<_tableSize;k++)
    {
      _table[i][k] = 0;
    }
  }

  // Initialize a,b,p
  srand(time(NULL));
  p  =  getPrimeModulous(_tableSize);
  a = getRand(1,p);
  b = getRand(0,p);
}

FastIntMap::~FastIntMap()
{
  for(int i=0;i<TABLE_COLUMN_COUNT;i++)
  {
    delete _table[i];
  }
  delete _table;
  delete collisionMap;
}

unsigned int FastIntMap::hash(const int key)
{
  return ((a*key+b) % p) % _tableSize;
}
void FastIntMap::put(const int key,const int value)
{
  unsigned int tableIndex = hash(key);
  if(key && (!_table[1][tableIndex] || _table[1][tableIndex]==key))
  {
    _table[0][tableIndex] = value;
    _table[1][tableIndex] = key;
  }
  else
  {
    (*collisionMap)[key] = value;
  }
}

const int FastIntMap::get(const int key)
{
  if(key==0) return (*collisionMap)[key];

  unsigned int tableIndex = hash(key);
  if(_table[1][tableIndex])
  {
    if(_table[1][tableIndex]==key)
    {
      return _table[0][tableIndex];
    }
    else
    {
      return (*collisionMap)[key];
    }
  }
  return 0;
}
