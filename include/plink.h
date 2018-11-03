


typedef struct{
    unsigned int sn;
    unsigned int original_sender;
    unsigned int sender;
}mid_t;


typedef struct{
    mid_t mid;
}plink_msg_t;

typedef struct{
    unsigned int no_entries;
    unsigned int no_process;
    unsigned int*** entries;
}uid_table_t;


int init_table_uid(uid_table_t* table,unsigned int no_entries, unsigned int no_process);

int table_read_entry(uid_table_t* table,mid_t mid);

int table_write_entry(uid_table_t* table,mid_t mid,unsigned int value);

int free_table_uid(uid_table_t* table);