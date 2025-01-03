#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd1 = open("test.txt", O_RDONLY);
    if (fd1 < 0) {
        printf("Test 1: Cannot open test.txt\n");
    } else {
        printf("Test 1: Successfully opened test.txt\n");
        close(fd1);
    }

    int fd2 = open("/tmp/test.txt", O_RDONLY);
    if (fd2 < 0) {
        printf("Test 2: Access to /tmp/test.txt blocked (expected)\n");
    } else {
        printf("Test 2: WARNING - Successfully opened /tmp/test.txt\n");
        close(fd2);
    }

    return 0;
}
