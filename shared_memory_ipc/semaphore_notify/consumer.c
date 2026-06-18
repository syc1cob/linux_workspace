/*
 * consumer.c
 *
 * Consumer can start before producer.
 * It blocks on semaphore until producer
 * signals that data is available.
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
     * Create/Open shared memory.
     *
     * If consumer starts first,
     * it creates the shared memory.
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
     * Ensure shared memory size.
     */
    if (ftruncate(shm_fd, SHM_SIZE) == -1)
    {
        perror("ftruncate");
        return EXIT_FAILURE;
    }

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
     * Create/Open semaphore.
     *
     * Initial count = 0
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

    printf("Consumer: Waiting for notification...\n");

    /*
     * Blocks until producer calls sem_post()
     */
    sem_wait(sem);

    printf("Consumer: Notification received.\n");

    printf("Consumer received: %s\n", shm_ptr);

    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);

    sem_close(sem);

    /*
     * Cleanup kernel objects.
     */
    sem_unlink(SEM_NAME);
    shm_unlink(SHM_NAME);

    return EXIT_SUCCESS;
}
