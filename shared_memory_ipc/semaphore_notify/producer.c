/*
 * producer.c
 *
 * Demonstrates:
 *   1. POSIX Shared Memory
 *   2. POSIX Named Semaphore
 *
 * Flow:
 *
 * Producer
 *    |
 *    | Write data into shared memory
 *    |
 *    | sem_post()
 *    v
 * Semaphore Count = 1
 *
 * Consumer waiting in sem_wait()
 * wakes up and reads data.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>      // O_CREAT, O_RDWR
#include <sys/mman.h>   // shm_open(), mmap()
#include <unistd.h>     // ftruncate(), close()
#include <semaphore.h>  // sem_open(), sem_post()

#define SHM_NAME "/my_shm_sem"
#define SEM_NAME "/my_sem"

#define SHM_SIZE 256

int main(void)
{
    int shm_fd;
    char *shm_ptr;
    sem_t *sem;

    /*
     * Create/Open shared memory.
     *
     * O_CREAT
     *      Create if not existing.
     *
     * O_RDWR
     *      Read/Write access.
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
     * Resize shared memory object.
     *
     * Newly created shared memory has size 0.
     */
    if (ftruncate(shm_fd, SHM_SIZE) == -1)
    {
        perror("ftruncate");
        return EXIT_FAILURE;
    }

    /*
     * Map shared memory into this process.
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
     * Create/Open named semaphore.
     *
     * Arguments:
     *
     * SEM_NAME
     *      Semaphore name.
     *
     * O_CREAT
     *      Create if not existing.
     *
     * 0666
     *      Permission bits.
     *
     * 0
     *      Initial semaphore count.
     *
     * Count starts at 0 because
     * no data is available yet.
     */
    sem = sem_open(SEM_NAME,
                   O_CREAT,
                   0666,
                   0);

    if (sem == SEM_FAILED)
    {
        perror("sem_open");
        return EXIT_FAILURE;
    }

    /*
     * Write message into shared memory.
     */
    strcpy(shm_ptr, "Hello from Producer using Shared Memory + Semaphore");

    printf("Producer: Message written.\n");

    /*
     * Increment semaphore count.
     *
     * sem_post():
     *
     * Count = Count + 1
     *
     * If some process is blocked in
     * sem_wait(), kernel wakes it up.
     */
    sem_post(sem);

    printf("Producer: Notification sent.\n");

    /*
     * Cleanup local resources.
     */
    munmap(shm_ptr, SHM_SIZE);

    close(shm_fd);

    sem_close(sem);

    return EXIT_SUCCESS;
}
