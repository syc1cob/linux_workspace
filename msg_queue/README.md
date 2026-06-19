# POSIX Message Queue IPC

This project demonstrates Inter-Process Communication (IPC) using POSIX Message Queues in Linux.

The repository contains:

- Basic Message Queue Communication
- Multiplexed Message Queue Server using `select()`

---

## Repository Structure

```text
msg_queue/
├── README.md
├── sender.c
├── receiver.c
└── multiplexing/
    ├── sender1.c
    ├── sender2.c
    └── server.c
```

---

## What is a Message Queue?

A message queue is a kernel-managed IPC mechanism that allows processes to exchange messages.

Unlike Shared Memory, the kernel manages:

- Message storage
- Message boundaries
- Synchronization
- Blocking and wake-up behavior

This makes message queues simpler to use than shared memory because additional synchronization primitives such as semaphores are not required.

---

# Example 1: Basic Message Queue Communication

## Architecture

```text
+----------+
| Sender   |
+----------+
      |
      | mq_send()
      v

+------------------+
| Message Queue    |
|    (Kernel)      |
+------------------+

      ^
      |
      | mq_receive()
      |
+----------+
| Receiver |
+----------+
```

---

## Execution Flow

### Step 1

Receiver starts and waits for a message.

```text
Receiver: Waiting for message...
```

Receiver blocks inside:

```c
mq_receive()
```

### Step 2

Sender sends a message.

```c
mq_send()
```

Example:

```text
Hello from POSIX Message Queue
```

### Step 3

Kernel wakes the receiver.

Receiver reads the message and continues execution.

---

## Build

```bash
gcc sender.c -o sender 

gcc receiver.c -o receiver 
```

---

## Run

### Terminal 1

```bash
./receiver
```

Output:

```text
Receiver: Waiting for message...
```

### Terminal 2

```bash
./sender
```

Output:

```text
Message sent: Hello from POSIX Message Queue
```

### Receiver Output

```text
Receiver: Message received.
Received: Hello from POSIX Message Queue
```

---

# Example 2: Multiplexed Message Queue Server

This example demonstrates how a single process can monitor multiple message queues simultaneously using `select()`.

The concept is similar to a multiplexed Unix Domain Socket server.

---

## Architecture

```text
Sender1
   |
   v
+--------+
| MQ1    |
+--------+
     \
      \
       \
        +----------------------+
        | Multiplexed Receiver   |
        |      select()        |
        +----------------------+
       /
      /
     /

+--------+
| MQ2    |
+--------+
   ^
   |
Sender2
```

---

## Execution Flow

### Step 1

Server creates/opens:

```text
MQ1
MQ2
```

and monitors both queues using:

```c
select()
```

### Step 2

Sender1 sends a message to MQ1.

```text
MQ1 becomes readable
```

The kernel wakes the receiver.

Receiver reads:

```c
mq_receive(mq1, ...)
```

### Step 3

Sender2 sends a message to MQ2.

```text
MQ2 becomes readable
```

The kernel wakes the Receiver.

Receiver reads:

```c
mq_receive(mq2, ...)
```

---

## Why Multiplexing?

Without multiplexing:

```text
Thread 1 -> MQ1
Thread 2 -> MQ2
Thread 3 -> MQ3
```

With multiplexing:

```text
Single Receiver
      |
      +--> MQ1
      +--> MQ2
      +--> MQ3
```

A single thread can efficiently monitor multiple IPC endpoints.

---

## Important Linux Note

POSIX does not require a message queue descriptor (`mqd_t`) to be implemented as a file descriptor.

However, on Linux:

```text
mqd_t -> File Descriptor
```

Therefore Linux applications can use:

- select()
- poll()
- epoll()

with POSIX message queues.

This multiplexing example relies on this Linux-specific behavior.

---

# Message Queue vs Shared Memory

| Feature | Message Queue | Shared Memory |
|----------|---------------|---------------|
| Data Copy | Yes | No |
| Message Boundaries | Yes | No |
| Kernel Managed Buffer | Yes | No |
| Synchronization Built-in | Yes | No |
| Additional Semaphore Required | No | Usually Yes |
| Speed | Fast | Fastest |
| Complexity | Lower | Higher |

---

# Important APIs

### mq_open()

Creates or opens a message queue.

```c
mq_open()
```

### mq_send()

Places a message into the queue.

```c
mq_send()
```

### mq_receive()

Receives a message from the queue.

If the queue is empty, the calling process blocks.

```c
mq_receive()
```

### mq_close()

Closes the queue descriptor.

```c
mq_close()
```

### mq_unlink()

Removes the queue from the kernel.

```c
mq_unlink()
```

---

# Learning Outcomes

After completing these examples, you should understand:

- POSIX Message Queues
- Message-based IPC
- Blocking and wake-up behavior
- Queue lifecycle management
- Kernel-managed synchronization
- Multiplexing using `select()`
- Differences between Message Queues and Shared Memory

---

# Related Projects

```text
linux_workspace/
├── uds_clientserver/
├── shared_memory_ipc/
└── msg_queue/
```

