# 05 · Multitasking and scheduling

TCBs, task states, per-task stacks, context switching, timer preemption,
round robin vs. FCFS, the idle task, scheduling metrics, fault isolation.

Builds on lesson 04 (see `lesson.mk`).

## What happens

1. `task_create()` fills in a TCB and builds a fake exception frame on the
   task's stack, so starting a task works like resuming one. The task joins
   the ready queue.
2. `sched_start()` pends PendSV, which loads the first task unprivileged.
   `kmain()` never resumes.
3. SysTick charges each 1 ms tick to the running task. Under round robin it
   pends PendSV when the 10 ms slice is used up; under FCFS it never preempts.
4. PendSV (`kernel/context_switch.S`) saves r4–r11 and EXC_RETURN (plus
   s16–s31 if the task used the FPU), calls `schedule()`, and restores the
   next task. The hardware handles the rest.
5. `schedule()` moves a still-running task to the back of the queue and takes
   the front one. If the queue is empty the idle task runs `wfi`.
6. When the last task exits, the kernel prints response, waiting and
   turnaround times.

```
            task_create            schedule()            exit()
   (new) ─────────────▶ READY ─────────────────▶ RUNNING ──────▶ TERMINATED
                          ▲   ◀───────────────────  │
                          │  time slice over / yield │ sem_wait (lesson 06)
                          │                          ▼
                          └─────── task_wake ──── BLOCKED
```

## Read the code

| File | Look for |
|---|---|
| [`kernel/task.h`](kernel/task.h) | the TCB, task states, policies |
| [`kernel/task.c`](kernel/task.c) | `init_stack()`, `schedule()`, `systick_hook()`, `print_stats()` |
| [`kernel/context_switch.S`](kernel/context_switch.S) | save/restore, the lazy FPU check, the first switch |
| [`user/crt0.c`](user/crt0.c) | `_task_start(fn, arg)`, where every task begins |
| [`kernel/syscall.c`](kernel/syscall.c) | lesson 04's table plus `SYS_yield` |
| [`app.c`](app.c), [`kmain.c`](kmain.c) | the worker tasks and the policy menu |

## Run it

```bash
make LESSON=05-scheduling flash
```

Five workers increment a shared counter to 10,000,000. Each exits with the
number of increments it made.

- **FCFS:** the first worker does all the work and the rest show huge
  response and waiting times (the convoy effect).
- **Round robin:** the work is shared, but a worker preempted between reading
  and writing the counter overwrites the others' updates, so the exit
  statuses add up to more than 10,000,000. Lesson 06 fixes this.
- **Round robin with a rogue task:** `rogue` touches a privileged register.
  The kernel kills only that task.

```
pid 1000 starts at 3518 ms
pid 1001 starts at 3528 ms
...
All tasks finished. Policy: round robin

  pid  arrival  response  waiting    cpu  turnaround  exit status
 1000     3517         1      ...
...
sum of exit statuses: 13042117
```

## Try this

- Set `TIME_SLICE_MS` to 1 and to 100. How do response time and lost updates
  change?
- Make a worker call `sched_yield()` every 1000 increments and run it under
  FCFS (cooperative multitasking).
- Add a priority to `struct task` and pick the highest-priority ready task in
  `schedule()`. What happens to low-priority tasks?

## Known limitations

- CPU time is counted in whole ticks.
- System calls run with the scheduler held off, so a task waiting in `read()`
  stalls everyone. The tasks here don't read input for that reason.
- Tasks are created only at start-up. There's no `fork()`/`spawn()` and
  terminated slots aren't reused.
