
void mem_reset();
void* mem_alloc(int size);
void mem_free(void* address,int size);
void mem_reserve(void* address,int size);
void mem_dump(int start,int end);
int mem_num();
