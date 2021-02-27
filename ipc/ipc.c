#include "ipc.h"
#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

void comm_init(comm_t *comm, int role) {
  comm->role = role;
  if (role == ROLE_PARENT) {
    comm->file_desc_copi = open(COPI, O_RDONLY);
    comm->file_desc_poci = open(POCI, O_WRONLY);
  } else /* if(role == ROLE_CHILD) */ {
    comm->file_desc_copi = open(COPI, O_WRONLY);
    comm->file_desc_poci = open(POCI, O_RDONLY);
  }
}

int comm_read(comm_t *comm, uint8_t *data, int len) {
  const int pipe =
      comm->role == ROLE_PARENT ? comm->file_desc_copi : comm->file_desc_poci;
  return read(pipe, data, len);
}

int comm_write(comm_t *comm, uint8_t *data, int len) {
  const int pipe =
      comm->role == ROLE_PARENT ? comm->file_desc_poci : comm->file_desc_copi;
  return write(pipe, data, len) == -1;
}

void comm_close(comm_t *comm) {
  close(comm->file_desc_copi);
  close(comm->file_desc_poci);
}
