/*
 * consumer.c
 *
 * Opens existing shared memory and reads data.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM_NAME "/my_shm"
#define SHM_SIZE 256

int main(void)
{
    int shm_fd;
    char *shm_ptr;

    /*
     * Open existing shared memory.
     *
     * O_RDONLY
     *      Read-only access.
     */
    shm_fd = shm_open(SHM_NAME,
                      O_RDONLY,
                      0666);

    if (shm_fd == -1)
    {
        perror("shm_open");
        return EXIT_FAILURE;
    }

    /*
     * Map shared memory into this process.
     *
     * PROT_READ because we only need to read.
     */
    shm_ptr = mmap(NULL,
                   SHM_SIZE,
                   PROT_READ,
                   MAP_SHARED,
                   shm_fd,
                   0);

    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        return EXIT_FAILURE;
    }

    /*
     * Read message written by producer.
     */
    printf("Consumer received: %s\n", shm_ptr);

    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);

    /*
     * Delete shared memory object.
     *
     * Similar to unlink() for files.
     *
     * After all processes close it,
     * kernel removes it permanently.
     */
    shm_unlink(SHM_NAME);

    return EXIT_SUCCESS;
}
