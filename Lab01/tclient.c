#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main()
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result, count;
    char str[80], instr[80]; // we take input from instr

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    // address.sin_addr.s_addr = inet_addr("172.16.179.162");
    address.sin_port = 9129;
    len = sizeof(address);
    result = connect(sockfd, (struct sockaddr *)&address, len);

    if (result == -1)
    {
        printf("error\n");
        exit(1);
    }

    // printf("\nenter string :");
    // fgets(instr, 80, stdin);
    // write(sockfd, instr, strlen(instr) + 1);
    // count = read(sockfd, str, 20);
    // printf("received data from server\n");
    // printf("%s\n", str);

    while (1)
    {
        fflush(stdout);
        printf("\nEnter string (type 'exit' to quit): ");
        fgets(instr, 80, stdin);
        write(sockfd, instr, strlen(instr) + 1);
        if (strncmp(instr, "exit", 4) == 0)
        {
            printf("Exiting the client...\n");
            break;
        }
        count = read(sockfd, str, sizeof(str));
        printf("Received data from server: %s\n", str);
    }
    close(sockfd);
    exit(0);
}
