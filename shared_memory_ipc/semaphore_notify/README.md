# Shared Memory IPC - Level 2 (Shared Memory + Semaphore)

This example demonstrates Inter-Process Communication (IPC) using:

- POSIX Shared Memory
- POSIX Named Semaphores

The producer writes data into a shared memory region and notifies the consumer using a semaphore.

The consumer waits for the notification before reading the shared memory.

---

## Why Shared Memory?

Shared memory is one of the fastest IPC mechanisms available in Linux.

Unlike sockets, pipes, or message queues, data does not need to be copied between processes.

Both processes access the same physical memory region mapped into their virtual address spaces.

---

## Why Semaphore?

Shared memory only provides a common storage area.

It does **not** provide:

- Synchronization
- Notification
- Mutual exclusion

Without synchronization, the consumer may attempt to read data before the producer writes it.

A semaphore is used to notify the consumer when data is available.

---

## Architecture

```text
                sem_post()

 +----------+  ------------->  +-----------+
 | Producer |                  | Semaphore |
 +----------+                  +-----------+
      |
      |
      | Write Data
      |
      v

 +------------------------------+
 |      Shared Memory           |
 |                              |
 | "Hello from Producer"        |
 +------------------------------+

      ^
      |
      |
      | Read Data
      |
 +----------+
 | Consumer |
 +----------+

        sem_wait()
```

---

## IPC Objects Used

### Shared Memory

```c
#define SHM_NAME "/my_shm_sem"
```

Used for exchanging data between processes.

Created using:

```c
shm_open()
```

Mapped into process memory using:

```c
mmap()
```

---

### Named Semaphore

```c
#define SEM_NAME "/my_sem"
```

Used for synchronization.

Created/Open using:

```c
sem_open()
```

Producer signals:

```c
sem_post()
```

Consumer waits:

```c
sem_wait()
```

---

## Execution Flow

### Step 1

Consumer starts and waits on semaphore.

```text
Semaphore Count = 0
```

Consumer executes:

```c
sem_wait()
```

Consumer becomes blocked.

---

### Step 2

Producer starts.

Producer:

1. Creates shared memory
2. Maps shared memory
3. Writes message

```text
Hello from Producer using Shared Memory + Semaphore
```

---

### Step 3

Producer executes:

```c
sem_post()
```

Kernel increments semaphore count.

```text
0 -> 1
```

Consumer is awakened.

---

### Step 4

Consumer reads the shared memory.

```text
Consumer received:
Hello from Producer using Shared Memory + Semaphore
```

---

## Build

Compile producer:

```bash
gcc producer.c -o producer -pthread
```

Compile consumer:

```bash
gcc consumer.c -o consumer -pthread
```

---

## Run

Open Terminal 1:

```bash
./consumer
```

Output:

```text
Consumer: Waiting for producer...
```

---

Open Terminal 2:

```bash
./producer
```

Output:

```text
Producer: Message written.
Producer: Notification sent.
```

---

Consumer output:

```text
Consumer: Notification received.
Consumer received: Hello from Producer using Shared Memory + Semaphore
```

---

## Important APIs

### shm_open()

Creates or opens a shared memory object.

```c
shm_open(name, flags, mode);
```

Example:

```c
shm_open("/my_shm_sem", O_CREAT | O_RDWR, 0666);
```

---

### ftruncate()

Sets the size of the shared memory object.

```c
ftruncate(fd, size);
```

---

### mmap()

Maps shared memory into the process virtual address space.

```c
mmap(addr,
     length,
     protection,
     flags,
     fd,
     offset);
```

Example:

```c
mmap(NULL,
     SHM_SIZE,
     PROT_READ | PROT_WRITE,
     MAP_SHARED,
     shm_fd,
     0);
```

---

### sem_open()

Creates or opens a named semaphore.

```c
sem_open(name,
         oflag,
         mode,
         initial_value);
```

Example:

```c
sem_open("/my_sem",
         O_CREAT,
         0666,
         0);
```

---

### sem_wait()

Decrements the semaphore count.

If count is zero, the process blocks.

```c
sem_wait(sem);
```

---

### sem_post()

Increments the semaphore count.

If a process is waiting, it is awakened.

```c
sem_post(sem);
```

