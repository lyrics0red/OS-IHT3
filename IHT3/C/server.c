#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == -1) {
        perror("Socket failed\n");
        exit(-1);
    }

    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(atoi(argv[1]));
    int bindfd = bind(server, (struct sockaddr *)&adr, sizeof adr);
    if (bindfd == -1) {
        perror("Bind failed\n");
        exit(-1);
    }

    int listenfd = listen(server, 4);
    if (listenfd == -1) {
        perror("Listen failed\n");
        exit(-1);
    }


    socklen_t adrlen = sizeof adr;
    
    int dentistfd = accept(server, (struct sockaddr *) &adr, &adrlen);
    if (dentistfd == -1) {
        perror("Dentist failed\n");
        exit(-1);
    }
    
    int surgeonfd = accept(server, (struct sockaddr *) &adr, &adrlen);
    if (surgeonfd == -1) {
        perror("Surgeon failed\n");
        exit(-1);
    }
    
    int therapistfd = accept(server, (struct sockaddr *) &adr, &adrlen);
    if (therapistfd == -1) {
        perror("Therapist failed\n");
        exit(-1);
    }
    
    int patientsfd = accept(server, (struct sockaddr *) &adr, &adrlen);
    if (patientsfd == -1) {
        perror("Patients failed\n");
        exit(-1);
    }

    for (int i = 0; i < 10; ++i) {
        ssize_t nread;
        char buf[50];
        char tooth[50] = "tooth";
        char injury[50] = "injury";
        nread = read(patientsfd, buf, 50);
        if (nread == -1) {
            perror("Read failed\n");
            exit(-1);
        }
        if (nread == 0) {
            perror("End of file occured\n");
        }
        if (buf[0] == 't' && buf[1] == 'o' && buf[2] == 'o' && buf[3] == 't' && buf[4] == 'h') {
            write(STDOUT_FILENO, "Dentist started his work\n", 25);
            write(dentistfd, "tooth", 5);
            nread = read(dentistfd, buf, 50);
            if (nread == -1) {
                perror("Read failed\n");
                exit(-1);
            }	
        } else if (buf[0] == 'i' && buf[1] == 'n' && buf[2] == 'j' && buf[3] == 'u' && buf[4] == 'r' && buf[5] == 'y') {
            write(STDOUT_FILENO, "Surgeon started his work\n", 25);
            write(surgeonfd, "injury", 6);
            nread = read(surgeonfd, buf, 50);
            if (nread == -1) {
                perror("Read failed\n");
                exit(-1);
            }
        } else {
            write(STDOUT_FILENO, "Therapist started his work\n", 27);
            write(therapistfd, "another reason", 14);
            nread = read(therapistfd, buf, 50);
            if (nread == -1) {
                perror("Read failed\n");
                exit(-1);
            }
        }

        write(patientsfd, "Doctor made his work", 20);
        write(STDOUT_FILENO, "Doctor made his work\n", 21);
    }
    write(dentistfd, "done", 4);
    write(surgeonfd, "done", 4);
    write(therapistfd, "done", 4);
    sleep(1);
    close(dentistfd);
    close(surgeonfd);
    close(therapistfd);
    close(patientsfd);
    close(server);
    return 0;
}
