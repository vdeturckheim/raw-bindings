// Simple header to exercise parser patterns

typedef struct HandleImpl* Handle;

int get_value(int* out_value);
int write_data(void* buf, size_t len);
int set_name(const char* name, size_t length);
void set_callback(void (*cb)(int), void* userdata);

Handle create_handle(void);
void destroy_handle(Handle h);

