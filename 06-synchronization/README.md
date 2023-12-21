# 06 · Synchronization with semaphores

Race conditions, critical sections, counting semaphores, blocking vs.
busy-waiting, the bounded-buffer problem.

Builds on lessons 04 and 05 (see `lesson.mk`). Only the files below are new
or replaced.

## What happens

Lesson 05's workers lose updates when preempted between reading and writing
the shared counter. This lesson adds kernel semaphores to guard that critical
section.

- `kmain()` creates semaphores with `ksem_create(initial)` and passes their
  ids to the tasks, which use the `sem_wait()` and `sem_post()` system calls.
- `sem_wait` takes a unit if the count is positive. Otherwise `task_block()`
  moves the task to the semaphore's wait queue and switches away.
- `sem_post` hands the unit to the first waiter with `task_wake()` (FIFO), or
  increments the count if nobody is waiting.
- Both run in the SVC handler, which shares a priority with SysTick and
  PendSV, so nothing runs between checking the count and blocking. See the
  comment at the top of `kernel/sem.c`.

The menu runs three experiments under round robin:

1. **No lock:** lesson 05's race with a wider window, so updates are lost on
   every run.
2. **Mutex** (a semaphore created with 1): the increments add up to exactly
   the target.
3. **Producer/consumer:** two producers and two slow consumers share a 4-slot
   ring buffer guarded by `empty`, `full` and `mutex`.

## Read the code

| File | Look for |
|---|---|
| [`kernel/sem.c`](kernel/sem.c) | wait/post, blocking, hand-off to the first waiter |
| [`../05-scheduling/kernel/task.c`](../05-scheduling/kernel/task.c) | `task_block()`/`task_wake()`, the BLOCKED state |
| [`kernel/syscall.c`](kernel/syscall.c) | lesson 05's table plus `SYS_sem_wait`/`SYS_sem_post` |
| [`user/semaphore.c`](user/semaphore.c) | the user-side wrappers |
| [`app.c`](app.c) | the three experiments |

## Run it

```bash
make LESSON=06-synchronization flash
```

```
choice> 2
Workers exit with their increment counts; with the lock they add up to exactly 200000.

pid 1002: 49871 increments
...
sum of exit statuses: 200000
```

```
choice> 3
pid 1000 produced 1
        pid 1002 consumed 1
pid 1000 produced 2
...
sum of exit statuses: 110
```

## Try this

- Swap `sem_wait(s->empty)` and `sem_wait(s->mutex)` in the producer and
  explain the deadlock.
- Remove `mutex` from the buffer with one producer and one consumer, then
  with two of each. When is it needed?
- Post a semaphore from lesson 03's button interrupt to wake a task. What
  would `sem.c` need? (Hint: `irq_save()`.)

## Known limitations

- No priority inheritance, though the scheduler has no priorities anyway.
- Tasks can't create or destroy semaphores, and there are at most
  `MAX_SEMAPHORES`.
