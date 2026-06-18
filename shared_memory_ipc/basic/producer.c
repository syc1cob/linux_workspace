/*
 * producer.c
 *
 * Creates a POSIX shared memory object and writes a message into it.
 *
 * Flow:
 *
 * 1. Create/Open shared memory object
 * 2. Set its size
 * 3. Map it into this process address space
 * 4. Write data
 * 5. Cleanup local resources
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>      // O_CREAT, O_RDWR
#include <sys/mman.h>   // shm_open(), mmap()
#include <unistd.h>     // ftruncate(), close()

/*
 * Shared memory objects behave similarly to files.
 *
 * The name:
 *   - Must start with '/'
 *   - Is visible under /dev/shm on Linux
 */
#define SHM_NAME "/my_shm"

/*
 * Total size of shared memory region.
 */
#define SHM_SIZE 256

int main(void)
{
    int shm_fd;
    char *shm_ptr;

    /*
     * Create or open a shared memory object.
     *
     * Arguments:
     *   SHM_NAME
     *      Name of shared memory object.
     *
     *   O_CREAT
     *      Create if it does not already exist.
     *
     *   O_RDWR
     *      Open for read/write access.
     *
     *   0666
     *      Permissions:
     *          owner  -> read/write
     *          group  -> read/write
     *          others -> read/write
     */
    shm_fd = shm_open(SHM_NAME,
                      O_CREAT | O_RDWR,
                      0666);

    if (shm_fd == -1)
    {
        perror("shm_open");
        return EXIT_FAILURE;
    }

    /*
     * Shared memory initially has size 0.
     *
     * Expand it to SHM_SIZE bytes.
     *
     * Arguments:
     *   shm_fd
     *      Shared memory file descriptor.
     *
     *   SHM_SIZE
     *      Desired size.
     */
    if (ftruncate(shm_fd, SHM_SIZE) == -1)
    {
        perror("ftruncate");
        return EXIT_FAILURE;
    }

    /*
     * Map shared memory into virtual address space.
     *
     * mmap() returns a pointer to the shared memory.
     *
     * Arguments:
     *
     * NULL
     *      Kernel chooses mapping address.
     *
     * SHM_SIZE
     *      Number of bytes to map.
     *
     * PROT_READ | PROT_WRITE
     *      Memory can be read and written.
     *
     * MAP_SHARED
     *      Updates become visible to other processes
     *      mapping the same object.
     *
     * shm_fd
     *      Shared memory descriptor.
     *
     * 0
     *      Offset from start of object.
     */
    shm_ptr = mmap(NULL,
                   SHM_SIZE,
                   PROT_READ | PROT_WRITE,
                   MAP_SHARED,
                   shm_fd,
                   0);

    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        return EXIT_FAILURE;
    }

    /*
     * Write data into shared memory.
     *
     * Since shm_ptr points to the mapped region,
     * writing here writes directly into shared memory.
     */
    strcpy(shm_ptr, "Hello from Producer");

    printf("Producer wrote: %s\n", shm_ptr);

    /*
     * Remove mapping from current process.
     *
     * Shared memory object itself still exists.
     */
    munmap(shm_ptr, SHM_SIZE);

    /*
     * Close file descriptor.
     */
    close(shm_fd);

    return EXIT_SUCCESS;
}
