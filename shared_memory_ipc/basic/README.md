# Shared Memory IPC - Level 1 (Basic Shared Memory)

This example demonstrates the simplest form of Inter-Process Communication (IPC) using POSIX Shared Memory.

A producer process writes data into a shared memory region, and a consumer process reads the same data from that region.

---

## Architecture

```text
+----------+
| Producer |
+----------+
      |
      | Write Data
      v

+----------------------+
|    Shared Memory     |
+----------------------+

      ^
      | Read Data
      |
+----------+
| Consumer |
+----------+
```

---

## How It Works

1. Producer creates a shared memory object.
2. Producer maps the shared memory into its address space.
3. Producer writes a message into the shared memory.
4. Consumer opens the same shared memory object.
5. Consumer maps the shared memory into its address space.
6. Consumer reads the message.

---

## Build

Compile producer:

```bash
gcc producer.c -o producer
```

Compile consumer:

```bash
gcc consumer.c -o consumer
```

---

## Run

Terminal 1:

```bash
./producer
```

Output:

```text
Producer wrote: Hello from Producer
```

Terminal 2:

```bash
./consumer
```

Output:

```text
Consumer received: Hello from Producer
```

---
Note:
Start producer first

