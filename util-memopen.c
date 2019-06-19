#include "util-memopen.h"

#ifdef USE_FMEM_WRAPPER

#ifdef OS_WIN32

#include <io.h>
#include <fcntl.h>
#include <windows.h>

/*
 * \brief portable version of SCFmemopen for Windows works on top of real temp files
 * \param buffer that holds the file content
 * \param size of the file buffer
 * \param mode mode of the file to open
 * \retval pointer to the file; NULL if something is wrong
 */
FILE *SCFmemopen(void *buf, size_t size, const char *mode) {
    char temppath[MAX_PATH - 13];
    if (0 == GetTempPath(sizeof(temppath), temppath)) return NULL;

    char filename[MAX_PATH + 1];

    if (0 == GetTempFileName(temppath, "SC", 0, filename)) return NULL;

    FILE *f = fopen(filename, "wb");

    if (NULL == f) return NULL;

    fwrite(buf, size, 1, f);

    fclose(f);

    return fopen(filename, mode);
}
#else
typedef struct SCFmem_ {
    size_t pos;
    size_t size;
    char *buffer;
} SCFmem;

/**
 * \brief Seek the mem file from offset and whence
 * \param handler pointer to the memfile
 * \param osffset number of bytes to move from whence
 * \param whence SEEK_SET, SEEK_CUR, SEEK_END
 * \retval pos the position by the last operation, -1 if sizes are out of bounds
 */
static fpos_t SeekFn(void *handler, fpos_t offset, int whence) {
    size_t pos = 0;
    SCFmem *mem = handler;

    switch (whence) {
        case SEEK_SET:
            if (offset >= 0 && (size_t)offset <= mem->size) {
                return mem->pos = offset;
            }
        break;

        case SEEK_CUR:
            if (mem->pos + offset <= mem->size)
                return mem->pos += offset;
        break;

        case SEEK_END:
            /* must be negative */
            if (mem->size + offset <= mem->size)
                return pos = mem->size + offset;
        break;
    }

    return -1;

}

/**
 * \brief Read from the buffer looking for the available memory limits
 * \param handler pointer to the memfile
 * \param buf buffer to read from the handler
 * \param number of bytes to read
 * \retval count , the number of bytes read
 */
static int ReadFn(void *handler, char *buf, int size) {
    size_t count = 0;
    SCFmem *mem = handler;
    size_t available = mem->size - mem->pos;
    int is_eof = 0;

    if (size < 0) return - 1;

    if ((size_t)size > available) {
        size = available;
    } else {
        is_eof = 1;
    }

    while (count < (size_t)size)
        buf[count++] = mem->buffer[mem->pos++];

    if (is_eof == 1)
        return 0;

    return count;
}

/**
 * \brief Write into the buffer looking for the available memory limits
 * \param handler pointer to the memfile
 * \param buf buffer to write in the handler
 * \param number of bytes to write
 * \retval count , the number of bytes writen
 */
static int WriteFn(void *handler, const char *buf, int size) {
    size_t count = 0;
    SCFmem *mem = handler;
    size_t available = mem->size - mem->pos;

    if (size < 0) return - 1;

    if ((size_t)size > available)
        size = available;

    while (count < (size_t)size)
        mem->buffer[mem->pos++] = buf[count++];

    return count;
}

/**
 * \brief close the mem file handler
 * \param handler pointer to the memfile
 * \retval 0 on succesful
 */
static int CloseFn(void *handler) {
    free(handler);
    return 0;
}

/**
 * \brief portable version of SCFmemopen for OS X / BSD built on top of funopen()
 * \param buffer that holds the file content
 * \param size of the file buffer
 * \param mode mode of the file to open
 * \retval pointer to the file; NULL if something is wrong
 */

FILE *SCFmemopen(void *buf, size_t size, const char *mode) {
    SCFmem *mem = (SCFmem *) malloc(sizeof(SCFmem));
    if (mem == NULL)
        return NULL;

    memset(mem, 0, sizeof(SCFmem));
    mem->size = size, mem->buffer = buf;

    return funopen(mem, ReadFn, WriteFn, SeekFn, CloseFn);
}

#endif /* OS_WIN32 */

#else
int SCFmemopen(void *buf, size_t size, const char *mode) {
    /* Create a new memory object */
    int imode = 0;
    if (0 == strstr("r", mode)) {
        imode = 0;
    }
    else if(0 == strstr("w", mode)) {
        imode = 1;
    }
    else if(0 == strstr("wr", mode)) {
        imode = 2;
    }
    else if(0 == strstr("rw", mode)) {
        imode = 2;
    }

    int fd = shm_open("czz239", O_WRONLY | O_CREAT, 0777);
    if (0 < fd) {
        ftruncate(fd, size);
        write(fd, buf, size);
        close(fd);
        fd = shm_open("czz239", (0 == imode ? O_RDONLY: 1 == imode ? O_WRONLY: O_RDWR), 0777);
    }

    return fd;
}

#endif /* USE_FMEM_WRAPPER */