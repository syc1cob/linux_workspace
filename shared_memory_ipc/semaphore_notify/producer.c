/*
 * producer.c
 *
 * Shared Memory + Semaphore Notification
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    /* Create or open shared memory */
    shm_fd = shm_open(SHM_NAME,
                      O_CREAT | O_RDWR,
                      0666);

    if (shm_fd == -1)
    {
        perror("shm_open");
        return EXIT_FAILURE;
    }

    /* Ensure required size */
    if (ftruncate(shm_fd, SHM_SIZE) == -1)
    {
        perror("ftruncate");
        return EXIT_FAILURE;
    }

    /* Map shared memory */
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

    /* Create/Open semaphore */
    sem = sem_open(SEM_NAME,
                   O_CREAT,
                   0666,
                   0);

    if (sem == SEM_FAILED)
    {
        perror("sem_open");
        return EXIT_FAILURE;
    }

    printf("Producer: Writing data...\n");

    strcpy(shm_ptr,
           "Hello from Producer using Shared Memory + Semaphore");

    /*
     * Notify consumer that data is ready.
     */
    sem_post(sem);

    printf("Producer: Notification sent.\n");

    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
    sem_close(sem);

    return EXIT_SUCCESS;
}
