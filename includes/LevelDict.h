#include "leveldb/db.h"
#include "Dict.h"

/**
  This class encapsulates the levelDb operations we are required to make
  The main reason to encapsulate is to add efficiency, not requiring us to open the db repeatedly.
  And allow for easy migration in future.

  @author Otkrist Gupta
**/

class LevelDict: public Dict
{
  protected:
  leveldb::DB* db;

  public:
  LevelDict(const char * name,const char * pathname);
  ~LevelDict(); 
  
  // Get Identifiers//
  const char * getName();
  const char * getFullName();

  // Creates a fullName (concatenated pathname and fullname).
  const char * getFullName(const char * name,const char * pathname);

  // Check If the object is initialized //
  bool isInitialized();

  // Create a new dictionary on hard disk //
  int init();
  int exists(); // Test Existence //
  int remove(); // Delete //

  // Save a key value in dictionary //
  int put(const char * key,const char * value);

  // Remove a key from dictionary //
  int removeKey(const char * key);

  // Copy the value from dictionary //
  int get(const char * key, char ** value);
  int get(const char * key, char * buffer, int buffer_size);

  // Save and Get the string values by integer id //
  int put(const int key,const char * value);
  int get(const int key, char * buffer, int buffer_size);
  
  // Save and get int values from string key //
  int put(const char * key, const int value);
  int get(const char * key, int& value);
};
