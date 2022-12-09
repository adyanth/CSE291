#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define BACKEND_IP "192.168.64.2"
#define BACKEND_IP_FAIL "192.168.64.3"
#define BACKEND_PORT 8080
#define SERVER_PORT 8000
#define BUFFER_SIZE 2049

int main(int argc, char const* argv[]) {
    int backend_sock = 0, valread, backend_conn;
    struct sockaddr_in backend_sockaddr, backend_src_sockaddr;
    // char* hello = "GET / HTTP/1.1 \\r Host: localhost:8088 \\r User-Agent: curl/7.74.0 \\r Accept: */* \\r";
    char accept_buffer[BUFFER_SIZE] = {0};

    int server_sock, accepted_conn;  //, valread;
    struct sockaddr_in server_sockaddr;
    int opt = 1;
    int addrlen = sizeof(server_sockaddr);
    char recvbuf[BUFFER_SIZE] = {0};

    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_addr.s_addr = INADDR_ANY;
    server_sockaddr.sin_port = htons(SERVER_PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_sock, (struct sockaddr*)&server_sockaddr, sizeof(server_sockaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((accepted_conn = accept(server_sock, (struct sockaddr*)&server_sockaddr, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    valread = read(accepted_conn, accept_buffer, BUFFER_SIZE - 1);
    printf("Data received:\n %s\n", accept_buffer);

    if ((backend_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    backend_sockaddr.sin_family = AF_INET;
    backend_sockaddr.sin_port = htons(BACKEND_PORT);
    if (inet_pton(AF_INET, BACKEND_IP, &backend_sockaddr.sin_addr) <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }

    backend_src_sockaddr.sin_family = AF_INET;
    backend_src_sockaddr.sin_addr.s_addr= htonl(INADDR_ANY);
    backend_src_sockaddr.sin_port=htons(33000);
    bind(backend_sock, (struct sockaddr *)&backend_src_sockaddr, sizeof(backend_src_sockaddr));
    perror("BIND");
    struct timeval timeout = { 1, 0 };
    if (setsockopt(backend_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Starting 1st
    if ((backend_conn = connect(backend_sock, (struct sockaddr*)&backend_sockaddr,
                                sizeof(backend_sockaddr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(backend_sock, accept_buffer, strlen(accept_buffer), 0);
    printf("Message sent\n");
    while (valread > 0) {
        int sz = sizeof(backend_src_sockaddr);
        valread = recvfrom(backend_sock, recvbuf, BUFFER_SIZE, 0, (struct sockaddr*)&backend_src_sockaddr, &sz);
        recvbuf[valread] = '\0';
        perror("Error");
        printf("Message received: %s valread: %d\n", recvbuf, valread);
        printf("Relaying message\n");
        send(accepted_conn, recvbuf, valread, 0);
    }
    close(backend_conn);




    // Migration started
    // Starting 2nd

    if (inet_pton(AF_INET, BACKEND_IP_FAIL, &backend_sockaddr.sin_addr) <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((backend_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    backend_src_sockaddr.sin_family = AF_INET;
    backend_src_sockaddr.sin_addr.s_addr= htonl(INADDR_ANY);
    backend_src_sockaddr.sin_port=htons(33000);
    bind(backend_sock, (struct sockaddr *)&backend_src_sockaddr, sizeof(backend_src_sockaddr));
    perror("BIND");
    if (setsockopt(backend_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    while ((backend_conn = connect(backend_sock, (struct sockaddr*)&backend_sockaddr, sizeof(backend_sockaddr))) < 0) {
        printf("\nConnection Failed \n");
    }
    close(backend_conn);
    // send(backend_sock, accept_buffer, strlen(accept_buffer), 0);
    valread = 1;
    backend_src_sockaddr.sin_family = AF_INET;
    backend_src_sockaddr.sin_addr.s_addr= htonl(INADDR_ANY);
    backend_src_sockaddr.sin_port=htons(33000);
    bind(backend_sock, (struct sockaddr *)&backend_src_sockaddr, sizeof(backend_src_sockaddr));
    perror("BIND");
    if (setsockopt(backend_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    while ((backend_conn = connect(backend_sock, (struct sockaddr*)&backend_sockaddr, sizeof(backend_sockaddr))) < 0) {
        perror("\nConnection Failed \n");
    }
    while (valread > 0 || errno) {
        int sz = sizeof(backend_src_sockaddr);
        valread = recvfrom(backend_sock, recvbuf, BUFFER_SIZE, 0, (struct sockaddr*)&backend_src_sockaddr, &sz);
        recvbuf[valread] = '\0';
        perror("Error");
        printf("Message received: %s valread: %d\n", recvbuf, valread);
        printf("Relaying message\n");
        send(accepted_conn, recvbuf, valread, 0);
    }
    // closing the connected socket
    close(accepted_conn);
    // closing the listening socket
    shutdown(server_sock, SHUT_RDWR);
    return 0;
}
