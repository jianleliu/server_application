/**
* NAME : Jian Le Liu
*
* HOMEWORK : 4
*
* CLASS : ICS 451
*
* DATE : October 06, 2023
*
* FILE : server.c
*
**/

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT 25398
#define MESSAGE "Hello Jian Le Liu. The server is working!"

/**
 * Logs a message with a timestamp and program state to a log file in the same directory.
 *
 * @param state   The state of the log entry (e.g., "INFO" or "ERROR").
 * @param message The message to be logged.
 */
void server_log(const char* state, const char* message) {
    time_t current_time;
    struct tm* time_info;
    char time_buffer[30];

    time(&current_time);
    time_info = localtime(&current_time);

    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", time_info);

    FILE* log_file = fopen("output_server.txt", "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        return;
    }

    fprintf(log_file, "%s %s -- %s\n", time_buffer, state, message);
    fclose(log_file);
}

/**
 * The main method of server.c. It establishes a connection with a client and
 * sends the a preset message before terminating. The server then continues to 
 * listen for connection.
 * 
 * MESSAGE and PORT are defined at the start of the file.
 * 
 * @return 0 keep open until terminated manually 
*/
int main() {
    server_log("INFO", "Server socket creation...");
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1) {
        perror("Socket creation failed");
        server_log("ERROR", "Socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_log("INFO", "Server socket created");

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    char log_message[100];
    sprintf(log_message, "Assigned server address to port: %d", PORT);
    server_log("INFO", log_message);

    server_log("INFO", "Binding server address to port...");
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        server_log("ERROR", "Socket binding failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    server_log("INFO", "Socket binding sucess");
    while (1) {
        server_log("INFO", "Server socket listening...");
        if (listen(server_socket, 1) == -1) {
            perror("Listen failed");
            server_log("ERROR", "Listen failed");
            continue;
        }

        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        server_log("INFO", "Establishing connection with client socket...");

        if (client_socket == -1) {
            perror("Accept failed");
            server_log("ERROR", "Accept failed");
            continue;
        }
        server_log("INFO", "Connection established");
        
        server_log("INFO", "Sending packet to the client...");
        if (send(client_socket, MESSAGE, strlen(MESSAGE), 0) == -1) {
            perror("Send failed");
            server_log("ERROR", "Send failed");
        }

        printf("Message sent to client\n");
        server_log("INFO", "Message sent");

        server_log("INTO", "Terminating connection...");
        close(client_socket);
        server_log("INTO", "Connection terminated");
    }

    server_log("INFO", "Terminating server socket...");
    close(server_socket);
    server_log("INFO", "Server socket terminated");
    return 0;
}

