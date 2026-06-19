/*
 * receiver.c
 *
 * Demonstrates:
 *   POSIX Message Queue Receiver
 *
 * Flow:
 *
 * 1. Create/Open a POSIX message queue
 * 2. Wait for a message using mq_receive()
 * 3. Read and print the message
 * 4. Cleanup resources
 *
 * Note:
 *   The receiver can be started before the sender.
 *
 *   If the queue is empty, mq_receive() blocks until
 *   another process sends a message using mq_send().
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // O_CREAT, O_RDONLY
#include <mqueue.h>     // POSIX Message Queue APIs

#define MQ_NAME "/my_message_queue"
#define BUFFER_SIZE 256

int main(void)
{
    mqd_t mq;
    char buffer[BUFFER_SIZE];

    /*
     * Message Queue Attributes
     *
     * mq_flags
     *      0 -> Blocking mode
     *
     * mq_maxmsg
     *      Maximum number of messages
     *      that can be stored in queue.
     *
     * mq_msgsize
     *      Maximum size of each message.
     *
     * mq_curmsgs
     *      Current number of messages.
     *      Kernel manages this field.
     */
    struct mq_attr attr =
    {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = BUFFER_SIZE,
        .mq_curmsgs = 0
    };

    /*
     * Create/Open Message Queue.
     *
     * Arguments:
     *
     * MQ_NAME
     *      Queue name.
     *      Must begin with '/'.
     *
     * O_CREAT
     *      Create queue if it does not exist.
     *
     * O_RDONLY
     *      Open queue for receiving messages.
     *
     * 0666
     *      Permissions:
     *          owner  -> read/write
     *          group  -> read/write
     *          others -> read/write
     *
     * &attr
     *      Queue attributes used only if queue
     *      is created.
     */
    mq = mq_open(MQ_NAME,
                 O_CREAT | O_RDONLY,
                 0666,
                 &attr);

    if (mq == (mqd_t)-1)
    {
        perror("mq_open");
        return EXIT_FAILURE;
    }

    printf("Receiver: Waiting for message...\n");

    /*
     * Receive a message.
     *
     * Arguments:
     *
     * mq
     *      Queue descriptor.
     *
     * buffer
     *      Destination buffer.
     *
     * BUFFER_SIZE
     *      Size of destination buffer.
     *
     * NULL
     *      Ignore message priority.
     *
     * Blocking Behavior:
     *
     * If queue is empty:
     *      Receiver blocks here.
     *
     * If sender sends a message:
     *      Kernel wakes receiver.
     */
    ssize_t bytes_received =
        mq_receive(mq,
                   buffer,
                   BUFFER_SIZE,
                   NULL);

    if (bytes_received == -1)
    {
        perror("mq_receive");
        mq_close(mq);
        return EXIT_FAILURE;
    }

    printf("Receiver: Message received.\n");
    printf("Received: %s\n", buffer);

    /*
     * Close queue descriptor.
     *
     * Queue itself still exists.
     */
    mq_close(mq);

    /*
     * Remove queue from kernel.
     *
     * Similar to unlink() for files.
     *
     * Queue is removed once all
     * processes close it.
     */
    mq_unlink(MQ_NAME);

    return EXIT_SUCCESS;
}
