/*
 * ============================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Main file of the project
 *
 * ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "colors.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>

int
main (int argc, char* argv[])
{

    /* Read command line options */
    options_t options;
    options_parser(argc, argv, &options);

#ifdef DEBUG
    fprintf(stdout, BLUE "Command line options:\n" NO_COLOR);
    fprintf(stdout, BROWN "help: %d\n" NO_COLOR, options.help);
    fprintf(stdout, BROWN "version: %d\n" NO_COLOR, options.version);
    fprintf(stdout, BROWN "server_mode: %d\n" NO_COLOR, options.server_mode);
    fprintf(stdout, BROWN "client_mode: %d\n" NO_COLOR, options.client_mode);
#endif

    /* Common fd */
    int selectedfd;
    
    /* Create new socket */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /* Define socket structure */
    struct sockaddr_in address = {
        AF_INET,
        htons(9999),
        0
    };

    /* Init specific operation mode */
    if (options.server_mode)
    {
        /* Server side */
        fprintf(stdout, GREEN "Running in server_mode \n" NO_COLOR);
        bind(sockfd, &address, sizeof(address));
        listen(sockfd, 10);
        selectedfd = accept(sockfd, 0, 0);
    } else if (options.client_mode && !options.server_mode)
    {
        /* Client side */
        fprintf(stdout, GREEN "Running in client_mode \n" NO_COLOR);
        connect(sockfd, &address, sizeof(address));
        selectedfd = !selectedfd ? sockfd : selectedfd;
    } else
    {
        fprintf(stdout, RED "No mode selected! \n" NO_COLOR);
        return EXIT_FAILURE;
    }
    
    // stdin - 0
    struct pollfd fds[2] = {
        {
            0,
            POLLIN,
            0
        },
        {
            selectedfd,
            POLLIN,
            0
        }
    };
    
    /* Poll file descriptor findind new messages */
    for (;;) {
        char buffer[256] = { 0 };

        poll(fds, 2, 50000);

        if (fds[0].revents & POLLIN) {
            read(0, buffer, 255);
            send(selectedfd, buffer, 255, 0);
        } else if (fds[1].revents & POLLIN) {
            if (recv(selectedfd, buffer, 255, 0) == 0) {
                return EXIT_SUCCESS;
            }

            printf("%s\n", buffer);
        }
    }

    return EXIT_SUCCESS;
}

