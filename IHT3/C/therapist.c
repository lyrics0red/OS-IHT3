#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("Socket failed\n");
        exit(-1);
    }

    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(atoi(argv[1]));
    int inetfd = inet_pton(AF_INET, argv[2], &adr.sin_addr);
    if (inetfd == 0) {
        printf("inet_pton failed: src does not contain a character\n");
        exit(-1);
    }
    if (inetfd == -1) {
        perror("inet_pton failed\n");
        exit(-1);
    }
    int connectfd = connect(fd, (struct sockaddr *) &adr, sizeof adr);
    if (connectfd == -1) {
        perror("Connect failed\n");
        exit(-1);
    }
    
    char buf[50];
    ssize_t nread = read(fd, buf, 50);
    if (nread == -1) {
        perror("Read failed\n");
        exit(-1);
    }
    while (buf != "done") {
        printf("Therapist finished with patient\n");
        write(fd, "done", 4);
        ssize_t nread = read(fd, buf, 50);
        if (nread == -1) {
            perror("Read failed\n");
            exit(-1);
        }
    }
    close(fd);
    return 0;
}
