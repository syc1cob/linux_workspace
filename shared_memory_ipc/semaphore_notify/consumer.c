/*
 * consumer.c
 *
 * Demonstrates:
 *   Waiting for producer notification
 *   before reading shared memory.
 *
 * Flow:
 *
 * Consumer
 *    |
 *    | sem_wait()
 *    |
 *    | BLOCKED
 *    |
 * Producer writes data
 * Producer calls sem_post()
 *    |
 * Consumer wakes up
 *    |
 * Reads shared memory
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

#define SHM_NAME "/my_shm_sem"
#define SEM_NAME "/my_sem"

#define SHM_SIZE 256

int main(void)
{
    int shm_fd;
    char *shm_ptr;
    sem_t *sem;

    /*
     * Open existing shared memory.
     *
     * Producer must create it first.
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
     * Map shared memory for reading.
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
     * Open existing semaphore.
     *
     * Flag = 0
     *
     * Means:
     *     Do NOT create.
     *     Open already existing semaphore.
     */
    sem = sem_open(SEM_NAME, 0);

    if (sem == SEM_FAILED)
    {
        perror("sem_open");
        return EXIT_FAILURE;
    }

    printf("Consumer: Waiting for producer...\n");

    /*
     * Wait until semaphore count > 0
     *
     * If count == 0:
     *     Kernel blocks this process.
     *
     * If count > 0:
     *     Count decreases by one and
     *     execution continues.
     */
    sem_wait(sem);

    printf("Consumer: Notification received.\n");

    /*
     * Safe to read data now.
     */
    printf("Consumer received: %s\n", shm_ptr);

    /*
     * Cleanup resources.
     */
    munmap(shm_ptr, SHM_SIZE);

    close(shm_fd);

    sem_close(sem);

    /*
     * Remove named kernel objects.
     *
     * Similar to unlink() for files.
     */
    sem_unlink(SEM_NAME);

    shm_unlink(SHM_NAME);

    return EXIT_SUCCESS;
}
