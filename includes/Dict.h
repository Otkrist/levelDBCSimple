/**
  This class encapsulates the levelDb operations we are required to make
  The main reason to encapsulate is to add efficiency, not requiring us to open the db repeatedly.
  And allow for easy migration in future.

  @author Otkrist Gupta
**/
class Dict
{
  protected:
  
  const char * _pathname;
  const char * _name;
  const char * _fullname;
  
  public :

  // Create Dict //
  Dict() {} 

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
  
  ~Dict() {}
};
