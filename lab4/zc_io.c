#include "zc_io.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



// The zc_file struct is analogous to the FILE struct that you get from fopen.
struct zc_file {
    char* mmap_data;
    size_t mmap_offset; // can see offset as pos of next byte (0-indexing)
    size_t file_size;
    int fd;

    /* Some suggested fields :
        - pointer to the virtual memory space
        - offset from the start of the virtual memory
        - total size of the file
        - file descriptor to the opened file
        - mutex for access to the memory space and number of readers
    */
};

/**************
 * Exercise 1 *
 **************/

zc_file* zc_open(const char* path) {
    int fd = open(path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("cannot open file");
        return NULL;
    }

    struct stat fs;
    if (fstat(fd, &fs) == -1) {
        perror("cannot find file size");
        close(fd);
        return NULL;
    }

    size_t file_size_in_bytes = fs.st_size;

    char* file_data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    zc_file* file = (zc_file*)malloc(sizeof(zc_file));
    file->fd = fd;
    file->file_size = file_size_in_bytes;
    file->mmap_data = file_data;
    file->mmap_offset = 0;

    return file;
}

int zc_close(zc_file* file) {
    int flush_res = msync(file->mmap_data, file->file_size, MS_SYNC);
    munmap(file->mmap_data, file->file_size);
    close(file->fd);
    free(file);
    return flush_res;
}

const char* zc_read_start(zc_file* file, size_t* size) {
    size_t remaining_size = file->file_size - file->mmap_offset;
    if (remaining_size < *size) {
        *size = remaining_size;
    }

    char* res = &file->mmap_data[file->mmap_offset];
    file->mmap_offset += *size;
    return res;
}

void zc_read_end(zc_file* file) {
}

char* zc_write_start(zc_file* file, size_t size) {
    
    size_t required_size = file->mmap_offset + size;
    if (required_size > file->file_size) {
        size_t old_size = file->file_size;
        file->file_size = required_size;
        ftruncate(file->fd, required_size);
        munmap(file->mmap_data, old_size);
        file->mmap_data = mmap(NULL, required_size, PROT_READ | PROT_WRITE, MAP_SHARED, file->fd, 0);
        // file->mmap_data = mremap(file->mmap_data, old_size, required_size, MREMAP_MAYMOVE);
    }
    char* res = &file->mmap_data[file->mmap_offset];
    file->mmap_offset += size;
    return res;
}

void zc_write_end(zc_file* file) {
    msync(file->mmap_data, file->file_size, MS_SYNC);
}

/**************
 * Exercise 2 *
 **************/

off_t zc_lseek(zc_file* file, long offset, int whence) {
    // To implement
    return -1;
}

/**************
 * Exercise 3 *
 **************/

int zc_copyfile(const char* source, const char* dest) {
    // To implement
    return -1;
}

/**************
 * Bonus Exercise *
 **************/

const char* zc_read_offset(zc_file* file, size_t* size, long offset) {
    // To implement
    return NULL;
}

char* zc_write_offset(zc_file* file, size_t size, long offset) {
    // To implement
    return NULL;
}