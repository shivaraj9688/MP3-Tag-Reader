#include <stdio.h>
#include <string.h>

#include "mp3_tag.h"

OperationType check_operation_type(char *argv[])
{
    if (argv[1] == NULL)
    {
        return UNKNOWN;
    }

    if (strcmp(argv[1], "-h") == 0)
    {
        return HELP;
    }

    if (strcmp(argv[1], "-v") == 0)
    {
        return VIEW;
    }

    if (strcmp(argv[1], "-e") == 0)
    {
        return EDIT;
    }

    return UNKNOWN;
}

void print_help(void)
{
    printf("\n");
    printf("-------------------------------------------------\n");
    printf("             MP3 TAG READER AND EDITOR\n");
    printf("-------------------------------------------------\n");

    printf("\nUSAGE:\n");

    printf("\nView tags:\n");
    printf("./mp3tag -v file.mp3\n");

    printf("\nEdit tags:\n");
    printf("./mp3tag -e file.mp3 -t \"Title\"\n");
    printf("./mp3tag -e file.mp3 -a \"Artist\"\n");
    printf("./mp3tag -e file.mp3 -A \"Album\"\n");
    printf("./mp3tag -e file.mp3 -y \"Year\"\n");
    printf("./mp3tag -e file.mp3 -G \"Genre\"\n");
    printf("./mp3tag -e file.mp3 -c \"Comment\"\n");

    printf("\nOPTIONS:\n");
    printf("-h    Help\n");
    printf("-v    View MP3 tags\n");
    printf("-e    Edit MP3 tags\n");

    printf("\nEDIT OPTIONS:\n");
    printf("-t    Title\n");
    printf("-a    Artist\n");
    printf("-A    Album\n");
    printf("-y    Year\n");
    printf("-G    Genre\n");
    printf("-c    Comment\n");

    printf("\n-------------------------------------------------\n");
}

void print_error(void)
{
    printf("\nERROR: Invalid arguments\n");
    printf("Use './mp3tag -h' for help.\n");
}