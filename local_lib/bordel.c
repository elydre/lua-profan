#include <syscall.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <bordel.h>

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

// __udivdi3 definition
unsigned long long __udivdi3(unsigned long long num, unsigned long long den) {
    unsigned long long quot = 0, qbit = 1;

    if (den == 0) {
        return 0; // divide by zero
    }

    // Left-justify denominator and count shift
    while ((signed long long)den >= 0) {
        den <<= 1;
        qbit <<= 1;
    }

    while (qbit) {
        if (den <= num) {
            num -= den;
            quot += qbit;
        }
        den >>= 1;
        qbit >>= 1;
    }

    return quot;
}

// __umoddi3 definition
unsigned long long __umoddi3(unsigned long long num, unsigned long long den) {
    if (den == 0) {
        return num; // divide by zero
    }

    // Left-justify denominator and count shift
    while ((signed long long)den >= 0) {
        den <<= 1;
    }

    while (den > num) {
        den >>= 1;
    }

    return num - den;
}

typedef struct fd_to_path_t {
    int fd;
    char path[128];
    int pos;
    int size;
} fd_to_path_t;

fd_to_path_t *fd_to_path;

void init_bordel() {
    // init fd_to_path
    fd_to_path = malloc(sizeof(fd_to_path_t) * 256);
    for (int i = 0; i < 256; i++) {
        fd_to_path[i].fd = -1;
    }
    serial_debug("bordel initialized\n");
}

int open(const char *path, ...) {
    // add the path to the fd_to_path
    int dispo = -1;
    for (int i = 0; i < 256; i++) {
        if (fd_to_path[i].fd == -1) {
            dispo = i;
            break;
        }
    }

    if (dispo == -1) {
        serial_debug("open: no more fd available\n");
        return -1;
    }

    // if the start of the path is not /, add it
    char *new_path = malloc(strlen(path) + 2);
    if (path[0] != '/') {
        new_path[0] = '/';
        strcpy(new_path + 1, path);
    } else {
        strcpy(new_path, path);
    }

    // check if the file exists
    if (!c_fs_does_path_exists(new_path)) {
        serial_debug("open: creating %s in /\n", new_path + 1);
        c_fs_make_file("/", new_path + 1);
    }

    fd_to_path[dispo].fd = dispo;
    strcpy(fd_to_path[dispo].path, new_path);
    fd_to_path[dispo].pos = 0;
    fd_to_path[dispo].size = c_fs_get_file_size(new_path);

    serial_debug("open: %s -> fd %d\n", new_path, dispo);
    return dispo;
}

off_t lseek(int fd, off_t offset, int whence) {
    if (fd_to_path[fd].fd == -1) {
        serial_debug("lseek: fd %d not found\n", fd);
        return -1;
    }
    char *path = fd_to_path[fd].path;

    // change the position
    switch (whence) {
        case SEEK_SET:
            fd_to_path[fd].pos = offset;
            break;
        case SEEK_CUR:
            fd_to_path[fd].pos += offset;
            break;
        case SEEK_END:
            fd_to_path[fd].pos = fd_to_path[fd].size + offset;
            break;
        default:
            return -1;
    }
    return fd_to_path[fd].pos;
}

int close(int fd) {
    serial_debug("close: fd %d\n", fd);
    // remove the path from the fd_to_path
    if (fd_to_path[fd].fd == -1) {
        return -1;
    }

    fd_to_path[fd].fd = -1;
    return 0;
}

ssize_t read(int fd, void *buf, size_t count) {
    serial_debug("read: fd %d, count %d\n", fd, count);

    if (fd_to_path[fd].fd == -1) {
        return -1;
    }

    char *path = fd_to_path[fd].path;
    int pos = fd_to_path[fd].pos;
    int size = fd_to_path[fd].size;

    if (pos >= size) {
        return 0;
    }

    int read = min(count, size - pos);
    serial_debug("read: %d bytes read\n", read);

    uint8_t *buffer = malloc(size + 1);
    c_fs_read_file(path, buffer);
    
    for (int i = 0; i < read; i++) {
        ((uint8_t *) buf)[i] = buffer[pos + i];
    }

    fd_to_path[fd].pos += read;
    free(buffer);
    return read;
}

int unlink(const char *pathname) {
    serial_debug("unlink: %s\n", (char *) pathname);
    // file deletion is not implemented in FSv2
    return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    for (int i = 0; i < n; i++) {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }
    }
    return 0;
}

int str_len(char s[]) {
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}

void str_reverse(char s[]) {
    int i = 0;
    int j = str_len(s) - 1;
    char tmp;
    while (i < j) {
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i++;
        j--;
    }
}

void int2str(int n, char s[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) s[i++] = '-';
    s[i] = '\0';

    str_reverse(s);
}

void hex2str(uint32_t n, char s[]) {
    int i = 0;
    int tmp;
    char hex[] = "0123456789abcdef";
    do {
        tmp = n % 16;
        s[i++] = hex[tmp];
    } while ((n /= 16) > 0);
    s[i] = '\0';
    str_reverse(s);
}

void serial_debug(char *fmt, ...) {
    char *args = (char *) &fmt;
    args += 4;
    int i = 0;
    char *char_buffer = malloc(0x1000);
    int buffer_i = 1;
    char_buffer[0] = ' ';
    while (fmt[i] != '\0') {
        if (fmt[i] == '%') {
            i++;
            if (fmt[i] == 's') {
                char *s = *((char **) args);
                args += 4;
                for (int j = 0; s[j] != '\0'; j++) {
                    char_buffer[buffer_i] = s[j];
                    buffer_i++;
                }                
            } else if (fmt[i] == 'c') {
                char c = *((char *) args);
                args += 4;
                char_buffer[buffer_i] = c;
                buffer_i++;
            } else if (fmt[i] == 'd') {
                int n = *((int *) args);
                args += 4;
                char s[20];
                int2str(n, s);
                for (int j = 0; s[j] != '\0'; j++) {
                    char_buffer[buffer_i] = s[j];
                    buffer_i++;
                }
            } else if (fmt[i] == 'x') {
                uint32_t n = *((int *) args);
                args += 4;
                char s[20];
                hex2str(n, s);
                for (int j = 0; s[j] != '\0'; j++) {
                    char_buffer[buffer_i] = s[j];
                    buffer_i++;
                }
            } else if (fmt[i] == '%') {
                char_buffer[buffer_i] = '%';
                buffer_i++;
            }
        } else {
            char_buffer[buffer_i] = fmt[i];
            buffer_i++;
        }
        i++;
    }
    char_buffer[buffer_i] = '\0';
    c_serial_print(SERIAL_PORT_A, char_buffer);
    free(char_buffer);
}

void *memcpy(void *dest, const void *src, size_t n) {
    for (int i = 0; i < n; i++) {
        ((uint8_t *) dest)[i] = ((uint8_t *) src)[i];
    }
    return dest;
}

int fputs(const char *s, FILE *stream) {
    if (stream == stdout) {
        c_kprint((char *) s);
        return 0;
    }
    
    fwrite(s, 1, str_len((char *) s), stream);
    return 0;
}

int fputc(int c, FILE *stream) {
    char s[2];
    s[0] = c;
    s[1] = '\0';

    if (stream == stdout) {
        c_kprint(s);
        return 0;
    }

    // use fwrite to save the file content
    fwrite(s, 1, 1, stream);
    return 0;
}

int isalnum(int c) {
    return isalpha(c) || isdigit(c);

}
int isdigit(int c) {
    return c >= '0' && c <= '9';
}
int toupper(int c) {
    if (islower(c)) {
        return c - 32;
    }
    return c;
}

int isxdigit(int c) {
    return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

int isspace(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

int tolower(int c) {
    if (isupper(c)) {
        return c + 32;
    }
    return c;
}

int isalpha(int c) {
    return isupper(c) || islower(c);
}

int iscntrl(int c) {
    return c < 32 || c == 127;
}

int isgraph(int c) {
    return c > 32 && c < 127;
}

int islower(int c) {
    return c >= 'a' && c <= 'z';
}

int ispunct(int c) {
    return isgraph(c) && !isalnum(c);
}

int isupper(int c) {
    return c >= 'A' && c <= 'Z';
}

long double strtold(const char *theString, char **end) {
    c_kprint("strtold is not implemented yet...\n");
    return 0;
}

// fseek
int fseek(FILE *stream, long offset, int whence) {
    c_kprint("fseek is not implemented yet...\n");
    return 0;
}

// fdopen
FILE *fdopen(int fd, const char * mode) {
    // get path from fd
    char *path = fd_to_path[fd].path;
    serial_debug("fdopen: %s\n", path);
    // open file
    FILE *file = fopen(path, mode);
    return file;
}

// SLogMsg
void SLogMsg(const char * msg, ...) {
    c_kprint("SLogMsg is not implemented yet...\n");
    return;
}

// gettimeofday
int GetTickCount() {
    return c_timer_get_ms();
}
