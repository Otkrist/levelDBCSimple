/**
  Interface for a fast hashmap
**/

#include<map>
using namespace std;

template <class K,class V> class FastHashMap
{
  public:
  virtual void put(const K k,const V v) = 0;
  virtual const V get(const K k) = 0;
  //map<K,V>::iterator begin();
  //map<K,V>::iterator end();
  //virtual map<K,V>::iterator iterator() = 0;
};
