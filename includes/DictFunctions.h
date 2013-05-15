// cpp apis for leveldb //

#ifdef __cplusplus
extern "C" {
#endif

int mk_dict_cc(const char * name, const char * pathname);
int del_dict_cc(const char * name,const char * pathname);
int is_dict_cc(const char * name, const char * pathname);
int add_to_dict_cc(const char * name, const char * pathname, const char * key,const char * value);
int del_from_dict_cc(const char * name, const char * pathname, const char * key);
int get_from_dict_cc(const char * name, const char * pathname, const char * key,char ** value);
int get_from_dict_2_cc(const char * name, const char * pathname, const char * key,char * buffer, int buffer_size);
int add_int_to_dict_cc(const char * name, const char * pathname, const char * key,const int value);
int get_int_from_dict_cc(const char * name, const char * pathname, const char * key,int * value);
int add_char_from_dict_cc(const char * name, const char * pathname, const int key,char * value);
int get_char_from_dict_cc(const char * name, const char * pathname, const int key,char * buffer,int buffer_size);

int cleanUp_cc();

#ifdef __cplusplus
}
#endif
