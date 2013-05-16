#include "FastIntMap.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>

template class FastIntMap<int>;
template class FastIntMap<long long>;
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

template <typename INTTYPE> void FastIntMap<INTTYPE>::init()
{
  // Initialize the table 
  collisionMap = new map<INTTYPE,INTTYPE>();
  _table = new INTTYPE *[TABLE_COLUMN_COUNT];
  for(int i=0;i<TABLE_COLUMN_COUNT;i++)
  {
    _table[i] = new INTTYPE[_tableSize];
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

template <typename INTTYPE> unsigned int FastIntMap<INTTYPE>::hash(const int key)
{
  return ((a*key+b) % p) % _tableSize;
}
template <typename INTTYPE> void FastIntMap<INTTYPE>::put(const INTTYPE key,const INTTYPE value)
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

template <typename INTTYPE> const INTTYPE FastIntMap<INTTYPE>::get(const INTTYPE key)
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
  throw 0;
}
