/*
 * receiver.c
 *
 * Monitor two message queues using select()
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/select.h>

#define MQ1 "/mq1"
#define MQ2 "/mq2"

#define MSG_SIZE 256

int main(void)
{
    mqd_t mq1;
    mqd_t mq2;

    struct mq_attr attr =
    {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = MSG_SIZE,
        .mq_curmsgs = 0
    };

    char buffer[MSG_SIZE];

    mq1 = mq_open(MQ1,
                  O_CREAT | O_RDONLY,
                  0666,
                  &attr);

    mq2 = mq_open(MQ2,
                  O_CREAT | O_RDONLY,
                  0666,
                  &attr);

    if ((mq1 == (mqd_t)-1) || (mq2 == (mqd_t)-1))
    {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    printf("Receiver waiting....\n");

    while (1)
    {
        fd_set readfds;

        FD_ZERO(&readfds);

        FD_SET((int)mq1, &readfds);
        FD_SET((int)mq2, &readfds);

        int maxfd =
            ((int)mq1 > (int)mq2)
            ? (int)mq1
            : (int)mq2;

        int ret =
            select(maxfd + 1,
                   &readfds,
                   NULL,
                   NULL,
                   NULL);

        if (ret < 0)
        {
            perror("select");
            break;
        }

        if (FD_ISSET((int)mq1, &readfds))
        {
            mq_receive(mq1,
                       buffer,
                       MSG_SIZE,
                       NULL);

            printf("[MQ1] %s\n", buffer);
        }

        if (FD_ISSET((int)mq2, &readfds))
        {
            mq_receive(mq2,
                       buffer,
                       MSG_SIZE,
                       NULL);

            printf("[MQ2] %s\n", buffer);
        }
    }

    mq_close(mq1);
    mq_close(mq2);

    mq_unlink(MQ1);
    mq_unlink(MQ2);

    return 0;
}
