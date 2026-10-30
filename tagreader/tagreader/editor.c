#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "editor.h"
#include "id3v1.h"

static void copy_string(char *dest,
                        const char *src,
                        int size)
{
    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
}

static int edit_id3v1_tag(const char *filename,
                          const char *option,
                          const char *value)
{
    MP3Tag tag;

    memset(&tag, 0, sizeof(MP3Tag));

    /*
     * Read existing ID3v1 tag
     */
    if (!read_id3v1(filename, &tag))
    {
        printf("ERROR: ID3v1 tag not found\n");
        return 0;
    }

    /*
     * Edit Title
     */
    if (strcmp(option, "-t") == 0)
    {
        copy_string(tag.title, value, TITLE_SIZE);
    }

    /*
     * Edit Artist
     */
    else if (strcmp(option, "-a") == 0)
    {
        copy_string(tag.artist, value, ARTIST_SIZE);
    }

    /*
     * Edit Album
     */
    else if (strcmp(option, "-A") == 0)
    {
        copy_string(tag.album, value, ALBUM_SIZE);
    }

    /*
     * Edit Year
     */
    else if (strcmp(option, "-y") == 0)
    {
        copy_string(tag.year, value, YEAR_SIZE);
    }

    /*
     * Edit Genre
     */
    else if (strcmp(option, "-G") == 0)
    {
        copy_string(tag.genre, value, GENRE_SIZE);
    }

    /*
     * Edit Comment
     */
    else if (strcmp(option, "-c") == 0)
    {
        copy_string(tag.comment, value, COMMENT_SIZE);
    }

    else
    {
        printf("ERROR: Invalid edit option\n");
        return 0;
    }

    /*
     * Write updated tag to MP3
     */
    if (!write_id3v1(filename, &tag))
    {
        printf("ERROR: Unable to update ID3v1 tag\n");
        return 0;
    }

    return 1;
}


int edit_tags(const char *filename,
              const char *option,
              const char *value)
{
    FILE *fp;

    /*
     * Check whether MP3 file exists
     */
    fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf("ERROR: File does not exist: %s\n",
               filename);

        return 0;
    }

    fclose(fp);

    /*
     * Edit ID3v1 tag
     */
    if (edit_id3v1_tag(filename,
                       option,
                       value))
    {
        printf("\nTag updated successfully.\n");
        printf("Option : %s\n", option);
        printf("Value  : %s\n", value);

        return 1;
    }

    return 0;
}