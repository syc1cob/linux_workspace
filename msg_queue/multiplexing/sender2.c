/*
 * sender.c
 *
 * Sends a message through a POSIX Message Queue.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>

#define MQ_NAME "/mq2"

int main(void)
{
    mqd_t mq;

    struct mq_attr attr =
    {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = 256,
        .mq_curmsgs = 0
    };

    /*
     * Create/Open message queue.
     */
    mq = mq_open(MQ_NAME,
                 O_CREAT | O_WRONLY,
                 0666,
                 &attr);

    if (mq == (mqd_t)-1)
    {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    const char *message =
        "Hello from POSIX Message Queue";

    /*
     * Send message.
     */
    if (mq_send(mq,
                message,
                strlen(message) + 1,
                0) == -1)
    {
        perror("mq_send");
        return EXIT_FAILURE;
    }

    printf("Message sent: %s\n", message);

    mq_close(mq);

    return EXIT_SUCCESS;
}
