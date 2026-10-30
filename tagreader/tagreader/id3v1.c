#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "id3v1.h"
/*
 * ID3v1 structure:
 *
 * TAG     3 bytes
 * Title   30 bytes
 * Artist  30 bytes
 * Album   30 bytes
 * Year    4 bytes
 * Comment 30 bytes
 * Genre   1 byte
 *
 * Total = 128 bytes
 */

static void copy_field(char *dest, const char *src, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        dest[i] = src[i];
    }

    dest[size] = '\0';

    for (i = size - 1; i >= 0; i--)
    {
        if (dest[i] == ' ' || dest[i] == '\0')
        {
            dest[i] = '\0';
        }
        else
        {
            break;
        }
    }
}

int id3v1_exists(const char *filename)
{
    FILE *fp;
    char header[3];

    fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        return 0;
    }

    if (fseek(fp, -128, SEEK_END) != 0)
    {
        fclose(fp);
        return 0;
    }

    if (fread(header, 1, 3, fp) != 3)
    {
        fclose(fp);
        return 0;
    }

    fclose(fp);

    if (header[0] == 'T' &&
        header[1] == 'A' &&
        header[2] == 'G')
    {
        return 1;
    }

    return 0;
}

int read_id3v1(const char *filename, MP3Tag *tag)
{
    FILE *fp;

    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[30];
    unsigned char genre;

    fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        return 0;
    }

    if (fseek(fp, -128, SEEK_END) != 0)
    {
        fclose(fp);
        return 0;
    }

    char header[3];

    if (fread(header, 1, 3, fp) != 3)
    {
        fclose(fp);
        return 0;
    }

    if (memcmp(header, "TAG", 3) != 0)
    {
        fclose(fp);
        return 0;
    }

    fread(title, 1, 30, fp);
    fread(artist, 1, 30, fp);
    fread(album, 1, 30, fp);
    fread(year, 1, 4, fp);
    fread(comment, 1, 30, fp);
    fread(&genre, 1, 1, fp);

    copy_field(tag->title, title, 30);
    copy_field(tag->artist, artist, 30);
    copy_field(tag->album, album, 30);
    copy_field(tag->year, year, 4);
    copy_field(tag->comment, comment, 30);

    /*
     * ID3v1 genre is numeric.
     * Store it as a number in the genre string.
     */
    sprintf(tag->genre, "%u", genre);

    fclose(fp);

    return 1;
}

int write_id3v1(const char *filename, MP3Tag *tag)
{
    FILE *fp;

    char buffer[30];

    fp = fopen(filename, "r+b");

    if (fp == NULL)
    {
        return 0;
    }

    /*
     * Check whether ID3v1 already exists.
     */
    if (fseek(fp, -128, SEEK_END) != 0)
    {
        fclose(fp);
        return 0;
    }

    fwrite("TAG", 1, 3, fp);

    /*
     * Title
     */
    memset(buffer, ' ', 30);
    memcpy(buffer, tag->title,
           strlen(tag->title) > 30 ? 30 : strlen(tag->title));
    fwrite(buffer, 1, 30, fp);

    /*
     * Artist
     */
    memset(buffer, ' ', 30);
    memcpy(buffer, tag->artist,
           strlen(tag->artist) > 30 ? 30 : strlen(tag->artist));
    fwrite(buffer, 1, 30, fp);

    /*
     * Album
     */
    memset(buffer, ' ', 30);
    memcpy(buffer, tag->album,
           strlen(tag->album) > 30 ? 30 : strlen(tag->album));
    fwrite(buffer, 1, 30, fp);

    /*
     * Year
     */
    memset(buffer, ' ', 30);
    memset(buffer, ' ', 4);

    memcpy(buffer, tag->year,
           strlen(tag->year) > 4 ? 4 : strlen(tag->year));

    fwrite(buffer, 1, 4, fp);

    /*
     * Comment
     */
    memset(buffer, ' ', 30);

    memcpy(buffer, tag->comment,
           strlen(tag->comment) > 30 ? 30 : strlen(tag->comment));

    fwrite(buffer, 1, 30, fp);

    /*
     * Genre
     */
    unsigned char genre = 0;

    genre = (unsigned char)atoi(tag->genre);

    fwrite(&genre, 1, 1, fp);

    fclose(fp);

    return 1;
}