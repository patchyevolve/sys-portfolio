#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 4096

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int client_len = sizeof(client);

    char buffer[BUFFER_SIZE];

    printf("Starting C HTTP server on port %d...\n", PORT);

    // 1. Initialize WinSock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // 2. Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return 1;
    }

    // 3. Setup address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // 4. Bind
    if (bind(server_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        return 1;
    }

    // 5. Listen
    listen(server_socket, 5);

    printf("Server listening...\n");

    while (1) {

        // 6. Accept client
        client_socket = accept(server_socket, (struct sockaddr*)&client, &client_len);

        if (client_socket == INVALID_SOCKET) {
            printf("Accept failed\n");
            continue;
        }

        printf("Client connected\n");

        // 7. Receive request
        int received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

        if (received > 0) {
            buffer[received] = '\0';
            printf("Request:\n%s\n", buffer);
        }

        // 8. Send HTTP response
        const char* response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<html><body><h1>C Server Working</h1></body></html>";

        send(client_socket, response, strlen(response), 0);

        // 9. Close client
        closesocket(client_socket);
    }

    closesocket(server_socket);
    WSACleanup();

    return 0;
}