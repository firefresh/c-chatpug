/*
 * ============================================================================
 *
 *       Filename:  args.h
 *
 *    Description:  Header file of the command line options parser
 *
 * ============================================================================
 */



#ifndef ARGS_H
#define ARGS_H


#include <stdbool.h>
#include <getopt.h>



/* Max size of a file name */
#define FILE_NAME_SIZE 512



/* Defines the command line allowed options struct */
struct options
{
    bool help;
    bool version;
    bool client_mode;
    bool server_mode;
    //char file_name[FILE_NAME_SIZE];
};



/* Exports options as a global type */
typedef struct options options_t;



/* Public functions section */
void options_parser(int argc, char* argv[], options_t* options);


#endif // ARGS_H
