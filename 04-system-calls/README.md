# 04 · System calls and user mode

Privileged vs. unprivileged execution, MSP/PSP, the `svc` trap, the syscall
ABI, argument checking, `crt0`, and a small user-space libc on top of
`read()`/`write()`.

## What happens

1. `kmain()` calls `task_start_user(_start)`. `enter_user_mode`
   (`kernel/user_mode.S`) points PSP at the program's stack, sets
   `CONTROL.SPSEL` and `CONTROL.nPRIV`, and jumps to `_start`.
2. A wrapper such as `getpid()` executes `svc #5` with arguments in r0–r2.
3. The CPU stacks r0–r3, r12, lr, pc and xpsr on the process stack and enters
   `SVCall_Handler`. `svc_dispatch()` reads the number from the `svc`
   instruction before the saved pc, runs the handler and writes the result to
   the saved r0.
4. The program resumes after its `svc`, unprivileged, with the result in r0.

Unknown numbers return `-ENOSYS`, bad file descriptors `-EBADF`, and buffers
outside RAM/flash `-EFAULT`.

```
 user mode (PSP)                         kernel (handler mode, MSP)
 ─────────────────                       ─────────────────────────────
 printf("%d", x)
   └─ vformat → buffer
   └─ write(1, buf, n)
        └─ svc #55  ───── exception ───▶ SVCall_Handler
                                          └─ svc_dispatch(frame)
                                               └─ syscall(55, 1, buf, n)
                                                    └─ sys_write: access_ok? → kputc…
        r0 = n      ◀──── return ──────   frame->r0 = n
```

## Read the code

| File | Look for |
|---|---|
| [`kernel/user_mode.S`](kernel/user_mode.S) | the CONTROL write, PSP setup |
| [`user/syscall_arch.h`](user/syscall_arch.h) | a C call becoming `svc #n` |
| [`kernel/svc.c`](kernel/svc.c) | finding the frame, decoding the number, writing back r0 |
| [`kernel/syscall.c`](kernel/syscall.c) | the system call table |
| [`kernel/sys_io.c`](kernel/sys_io.c), [`kernel/syscall.h`](kernel/syscall.h) | `access_ok()`, the console as fds 0/1/2 |
| [`user/crt0.c`](user/crt0.c), [`user/stdio.c`](user/stdio.c) | `_start` → `main()` → `exit()`, user-space printf |
| [`app.c`](app.c) | the shell that exercises each call |

## Run it

```bash
make LESSON=04-system-calls flash
```

```
== Lesson 04: system calls ==
[kernel] privileged, on the main stack (sp 0x2001ffe8)
[kernel] starting the user program: unprivileged, process stack

User program running: pid 1000, unprivileged, on its own stack.
...
$ uptime
4170 ms
$ badptr
write() returned -6; -EFAULT is -6: the kernel refused to read that address on our behalf
$ exit 7

[kernel] process 1000 exited with status 7
```

`peek` reads `SysTick->VAL` from user mode, which faults. With only one
process the kernel halts; lesson 05 kills just the faulting task.

## Try this

- Add a `sys_uptime_us()` call: a number in `syscall_def.h`, a case in
  `kernel/syscall.c`, a wrapper in `user/unistd.c`.
- Issue `svc` with a number the kernel doesn't know. What comes back?
- Remove the `access_ok()` check from `sys_write` and run `badptr` again.

## Known limitations

- No MPU: user code can't touch core registers but can read and write kernel
  memory. `access_ok()` only checks that a pointer is in RAM or flash.
- `read()` busy-waits in the kernel until Enter is pressed.
