#include "comm.h"

void comm_init(struct comm_t *comm, int role) {
  comm->role = role;
  if (role == ROLE_PARENT) {
    comm->file_desc_copi = open(COPI, O_RDONLY);
    comm->file_desc_poci = open(POCI, O_WRONLY);
  } else /* if(role == ROLE_CHILD) */ {
    comm->file_desc_copi = open(COPI, O_WRONLY);
    comm->file_desc_poci = open(POCI, O_RDONLY);
  }
}

// Returns 0 if SUCESS
int write_child(struct comm_t *comm, char *data, int len) {
  return (write(comm->file_desc_poci, data, len) == -1);
}

int read_child(struct comm_t *comm, char *data, int len) {
  return read(comm->file_desc_copi, data, len);
}

int write_parent(struct comm_t *comm, char *data, int len) {
  return (write(comm->file_desc_copi, data, len) == -1);
}

int read_parent(struct comm_t *comm, char *data, int len) {
  return read(comm->file_desc_poci, data, len);
}

void comm_close(struct comm_t *comm) {
    close(comm->file_desc_copi);
    close(comm->file_desc_poci);
}
