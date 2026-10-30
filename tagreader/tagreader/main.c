#include <stdio.h>
#include <string.h>

#include "mp3_tag.h"
#include "id3v1.h"
#include "id3v2.h"
#include "editor.h"

int main(int argc, char *argv[])
{
    OperationType operation;

    /*
     * No arguments
     */
    if (argc < 2)
    {
        print_help();
        return 0;
    }

    operation = check_operation_type(argv);

    /*
     * HELP
     */
    if (operation == HELP)
    {
        print_help();
        return 0;
    }

    /*
     * VIEW
     */
    if (operation == VIEW)
    {
        if (argc != 3)
        {
            print_error();
            return 1;
        }

        char *filename = argv[2];

        MP3Tag tag;

        /*
         * Prefer ID3v2.
         */
        if (read_id3v1(filename, &tag))
        {
            display_tags(&tag);
        }
        else if (read_id3v2(filename, &tag))
        {
            display_tags(&tag);
        }
        else
        {
            printf("ERROR: No valid ID3 tag found.\n");
            return 1;
        }

        return 0;
    }

    /*
     * EDIT
     */
    if (operation == EDIT)
    {
        if (argc != 5)
        {
            print_error();
            return 1;
        }

        char *filename = argv[2];
        char *option = argv[3];
        char *value = argv[4];

        if (edit_tags(filename,
                      option,
                      value))
        {
            return 0;
        }

        return 1;
    }

    /*
     * Unknown operation
     */
    print_error();

    return 1;
}