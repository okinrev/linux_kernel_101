#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#define DEVICE_FILE "/dev/kplayground"
#define KPLAYGROUND_SET_BUFFER_SIZE _IOW('k', 1, int)
#define KPLAYGROUND_GET_BUFFER_SIZE _IOR('k', 2, int)

int main() {
    int fd;
    int buffer_size;
    char write_buf[512] = "Hello from user space!";
    char read_buf[512] = {0};
    ssize_t bytes_written, bytes_read;

    // Open the device file
    fd = open(DEVICE_FILE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device file");
        return -1;
    }

    // Get the initial buffer size
    if (ioctl(fd, KPLAYGROUND_GET_BUFFER_SIZE, &buffer_size) < 0) {
        perror("Failed to get buffer size");
        close(fd);
        return -1;
    }
    printf("Initial buffer size: %d bytes\n", buffer_size);

    // Set a new buffer size
    int new_size = 512;
    if (ioctl(fd, KPLAYGROUND_SET_BUFFER_SIZE, &new_size) < 0) {
        perror("Failed to set buffer size");
        close(fd);
        return -1;
    }

    // Write to the device
    bytes_written = write(fd, write_buf, strlen(write_buf));
    if (bytes_written < 0) {
        perror("Failed to write to the device");
        close(fd);
        return -1;
    }
    printf("Written %zd bytes to the device: %s\n", bytes_written, write_buf);

    // Read from the device
    bytes_read = read(fd, read_buf, sizeof(read_buf) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from the device");
        close(fd);
        return -1;
    }
    read_buf[bytes_read] = '\0'; // Null-terminate the string
    printf("Read %zd bytes from the device: %s\n", bytes_read, read_buf);

    // Get buffer size after operations
    if (ioctl(fd, KPLAYGROUND_GET_BUFFER_SIZE, &buffer_size) < 0) {
        perror("Failed to get buffer size");
        close(fd);
        return -1;
    }
    printf("Buffer size after operations: %d bytes\n", buffer_size);

    // Close the device file
    close(fd);
    return 0;
}

