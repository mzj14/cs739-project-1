#ifdef __cplusplus
extern "C" {
#endif
	int kv739_init(char ** server_list);
	void increment_counter();
	int kv739_put(char * key, char * value, char * old_value);
	int kv739_get(char * key, char * value);
	int kv739_shutdown();
#ifdef __cplusplus
}
#endif
