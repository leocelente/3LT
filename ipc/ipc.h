#ifndef COMM_H
#define COMM_H
#include <fcntl.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define COMM_BUFF_SZ 32

enum comm_roles {
    ROLE_PARENT,
    ROLE_CHILD
};

static const char * const COPI = "./build/child_out_parent_in";
static const char * const POCI = "./build/parent_out_child_in";

typedef struct  {
    int file_desc_copi;
    int file_desc_poci;
    int role;
} comm_t;

void comm_init(comm_t* comm, int role);

int comm_write(comm_t *comm, uint8_t *data, int len);
int comm_read(comm_t *comm, uint8_t *buffer, int size);

void comm_close(comm_t *comm) ;

#endif
