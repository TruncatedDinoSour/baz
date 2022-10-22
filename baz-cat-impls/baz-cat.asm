format ELF64 executable 3
segment readable executable

include "includes/libbaz.asm"

_start:
    ;; int read_bytes =
    mov rax, SYS_read       ;; SYS_read(
    mov rdi, STDIN_FILENO   ;;     fd=stdin,
    mov rsi, buffer         ;;     buf=&buffer,
    mov rdx, BUF_SIZE       ;;     count=BUF_SIZE
    syscall                 ;; );

    mov ebx, eax            ;; read_bytes_real = read_bytes;
                            ;; // Because the buffer is not always full

    cmp eax, 0              ;; if (read_bytes == 0)
    je .exit                ;;     goto .exit; // break

    mov rax, SYS_write      ;; SYS_write(
    mov rdi, STDOUT_FILENO  ;;     fd=stdout,
    mov rsi, buffer         ;;     buf=&buffer,
    movsxd rdx, ebx         ;;     count=(unsigned)bytes_read_real,
    syscall                 ;; );

    jmp _start              ;; LOOP_AGAIN;

.exit:
    mov rax, SYS_exit       ;; SYS_exit(
    mov rdi, EXIT_OK        ;;     status=EXIT_OK,
    syscall                 ;; );

segment readable writable
    buffer: rb BUF_SIZE     ;; char buffer[BUF_SIZE];
