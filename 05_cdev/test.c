#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int fd;
    if(argc < 2) {
        printf("file must be open as an argument");
        return 0;
    }

    fd = open(argv[1], O_RDONLY);
    if(fd < 0) {
        perror("open");
        return 0;
    }

    close(fd);

    fd = open(argv[1], O_RDWR | O_SYNC);
    if(fd < 0) {
        perror("open");
        return 0;
    }

    close(fd);
    
    fd = open(argv[1], O_WRONLY | O_NONBLOCK);
    if(fd < 0) {
        perror("open");
        return 0;
    }

    close(fd);
    return 0;
}
