format ELF64 executable 3
segment readable executable

include "includes/libbaz.asm"

;; Code
_start:
    ;; unsigned read_bytes =
    mov rax, SYS_read       ;; SYS_read(
    mov rdi, STDIN_FILENO   ;;     fd=stdin,
    mov rsi, c              ;;     buf=&c,
    mov rdx, 1              ;;     count=1
    syscall                 ;; );

    cmp eax, 0              ;; if (read_bytes == 0)
    je .exit                ;;     goto .exit; // break

    mov rax, SYS_write      ;; SYS_write(
    mov rdi, STDOUT_FILENO  ;;     fd=stdout,
    mov rsi, c              ;;     buf=&c,
    mov rdx, 1              ;;     count=1,
    syscall                 ;; );

    jmp _start              ;; LOOP_AGAIN;

.exit:
    mov rax, SYS_exit       ;; SYS_exit(
    mov rdi, EXIT_OK        ;;     status=EXIT_OK,
    syscall                 ;; );

segment readable writable
    c: rb 1                 ;; char c;
