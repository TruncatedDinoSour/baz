format ELF64 executable 3
segment readable executable

include "includes/libbaz.asm"

_start:
    ;; int read_bytes =
    xor rax, rax            ;; SYS_read[0](
    xor rdi, rdi            ;;     fd=STDIN_FILENO[0],
    mov rsi, buffer         ;;     buf=&buffer,
    mov rdx, BUF_SIZE       ;;     count=BUF_SIZE,
    syscall                 ;; );

    test eax, eax           ;; if (read_bytes == 0)
    jz .exit                ;;     goto .exit;

    movsxd rdx, eax         ;; int write_count = read_bytes;

    mov rax, SYS_write      ;; SYS_write(
    mov rdi, STDOUT_FILENO  ;;     fd=stdout,
    mov rsi, buffer         ;;     buf=&buffer,
                            ;;     count=write_count,
    syscall                 ;; );

    jmp _start              ;; LOOP_AGAIN;

.exit:
    mov rax, SYS_exit       ;; SYS_exit(
    xor rdi, rdi            ;;     status=EXIT_OK[0],
    syscall                 ;; );

segment readable writable
    buffer: rb BUF_SIZE     ;; char buffer[BUF_SIZE];
