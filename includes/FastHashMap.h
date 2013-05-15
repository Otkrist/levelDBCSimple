/**
  Interface for a fast hashmap
**/

#include<map>
using namespace std;
template <class K,class V> class FastHashMap
{
  public:
  virtual void put(const K k,const V v);
  virtual const V get(const K k);
};
