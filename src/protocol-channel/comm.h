#ifndef COMM_H
#define COMM_H
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


enum comm_roles {
    ROLE_PARENT,
    ROLE_CHILD
};

static char * const COPI = "child_out_parent_in";
static char * const POCI = "parent_out_child_in";


struct comm_t {
    int file_desc_copi;
    int file_desc_poci;
    int role;
};

void comm_init(struct comm_t* comm, int role);

int write_child(struct comm_t *comm, char *data, int len);
int read_child(struct comm_t *comm, char *buffer, int size);


int write_parent(struct comm_t *comm, char *data, int len);
int read_parent(struct comm_t *comm, char *buffer, int size);

void comm_close(struct comm_t *comm) ;

#endif
