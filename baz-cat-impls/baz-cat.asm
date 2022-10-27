format ELF64 executable 3
segment readable executable

include "includes/libbaz.asm"

_start:
    ;; int read_bytes =
    xor eax, eax            ;; SYS_read[0](
    xor edi, edi            ;;     fd=STDIN_FILENO[0],
    mov esi, buffer         ;;     buf=&buffer,
    mov edx, BUF_SIZE       ;;     count=BUF_SIZE,
    syscall                 ;; );

    test eax, eax           ;; if (read_bytes == 0)
    jz .exit                ;;     goto .exit;

    mov edx, eax            ;; int write_count = read_bytes;

    mov eax, SYS_write      ;; SYS_write(
    mov edi, STDOUT_FILENO  ;;     fd=stdout,
    mov esi, buffer         ;;     buf=&buffer,
                            ;;     count=write_count,
    syscall                 ;; );

    jmp _start              ;; LOOP_AGAIN;

.exit:
    mov eax, SYS_exit       ;; SYS_exit(
    xor edi, edi            ;;     status=EXIT_OK[0],
    syscall                 ;; );

segment readable writable
    buffer: rb BUF_SIZE     ;; char buffer[BUF_SIZE];
