// c apis for leveldb //

int mk_dict(const char * name, const char * pathname);
int del_dict(const char * name,const char * pathname);
int is_dict(const char * name, const char * pathname);
int add_to_dict(const char * name, const char * pathname, const char * key,const char * value);
int del_from_dict(const char * name, const char * pathname, const char * key);
int get_from_dict(const char * name, const char * pathname, const char * key,char ** value);
int get_from_dict_2(const char * name, const char * pathname, const char * key,char * buffer,int buffer_size);
void cleanUp();
