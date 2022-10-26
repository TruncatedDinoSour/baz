;; Assembly helpers for baz

define SYS_read      0
define SYS_write     1
define SYS_exit      60

define STDIN_FILENO  0
define STDOUT_FILENO 1

define EXIT_OK       0

define BUF_SIZE      128  ;; You really don't need much, this allocates 128 bytes
                          ;; in the memory, baz-cat usually gets called with smaller
                          ;; rather than bigger input, if expected the other way around
                          ;; I'd suggest using like 256 KB, or in this case -- 262144 bytes
