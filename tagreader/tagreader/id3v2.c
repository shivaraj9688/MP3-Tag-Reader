#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "id3v2.h"

static int syncsafe_to_int(unsigned char *data)
{
    return ((data[0] & 0x7F) << 21) |
           ((data[1] & 0x7F) << 14) |
           ((data[2] & 0x7F) << 7) |
           (data[3] & 0x7F);
}

static void read_text(char *dest,
                      unsigned char *data,
                      int size,
                      int max_size)
{
    int i;
    int len;

    if (size <= 1)
    {
        dest[0] = '\0';
        return;
    }

    /*
     * First byte is text encoding.
     */
    len = size - 1;

    if (len >= max_size)
    {
        len = max_size - 1;
    }

    /*
     * Most basic ID3v2.3 files use ISO-8859-1.
     */
    for (i = 0; i < len; i++)
    {
        if (data[i + 1] == '\0')
        {
            dest[i] = '\0';
            return;
        }

        dest[i] = data[i + 1];
    }

    dest[len] = '\0';
}

int id3v2_exists(const char *filename)
{
    FILE *fp;
    unsigned char header[3];

    fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        return 0;
    }

    if (fread(header, 1, 3, fp) != 3)
    {
        fclose(fp);
        return 0;
    }

    fclose(fp);

    if (memcmp(header, "ID3", 3) == 0)
    {
        return 1;
    }

    return 0;
}

int read_id3v2(const char *filename, MP3Tag *tag)
{
    FILE *fp;

    unsigned char header[10];

    memset(tag, 0, sizeof(MP3Tag));

    fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        return 0;
    }

    if (fread(header, 1, 10, fp) != 10)
    {
        fclose(fp);
        return 0;
    }

    if (memcmp(header, "ID3", 3) != 0)
    {
        fclose(fp);
        return 0;
    }

    /*
     * ID3 version
     */
    int major_version = header[3];

    if (major_version != 3 && major_version != 4)
    {
        fclose(fp);
        return 0;
    }

    /*
     * ID3 tag size
     */
    int tag_size = syncsafe_to_int(&header[6]);

    int bytes_read = 0;

    while (bytes_read < tag_size)
    {
        unsigned char frame_header[10];

        if (fread(frame_header, 1, 10, fp) != 10)
        {
            break;
        }

        bytes_read += 10;

        /*
         * Empty frame
         */
        if (frame_header[0] == 0)
        {
            break;
        }

        char frame_id[5];

        memcpy(frame_id, frame_header, 4);
        frame_id[4] = '\0';

        int frame_size;

        if (major_version == 4)
        {
            frame_size = syncsafe_to_int(&frame_header[4]);
        }
        else
        {
            frame_size =
                ((unsigned char)frame_header[4] << 24) |
                ((unsigned char)frame_header[5] << 16) |
                ((unsigned char)frame_header[6] << 8) |
                ((unsigned char)frame_header[7]);
        }

        if (frame_size <= 0 ||
            frame_size > tag_size)
        {
            break;
        }

        unsigned char *data;

        data = malloc(frame_size);

        if (data == NULL)
        {
            fclose(fp);
            return 0;
        }

        if (fread(data, 1, (size_t)frame_size, fp) != (size_t)frame_size)
        {
            free(data);
            break;
        }

        bytes_read += frame_size;

        /*
         * TIT2 = Title
         */
        if (strcmp(frame_id, "TIT2") == 0)
        {
            read_text(tag->title,
                      data,
                      frame_size,
                      TITLE_SIZE);
        }

        /*
         * TPE1 = Artist
         */
        else if (strcmp(frame_id, "TPE1") == 0)
        {
            read_text(tag->artist,
                      data,
                      frame_size,
                      ARTIST_SIZE);
        }

        /*
         * TALB = Album
         */
        else if (strcmp(frame_id, "TALB") == 0)
        {
            read_text(tag->album,
                      data,
                      frame_size,
                      ALBUM_SIZE);
        }

        /*
         * TYER = Year in ID3v2.3
         * TDRC = Recording time in ID3v2.4
         */
        else if (strcmp(frame_id, "TYER") == 0 ||
                 strcmp(frame_id, "TDRC") == 0)
        {
            read_text(tag->year,
                      data,
                      frame_size,
                      YEAR_SIZE);
        }

        /*
         * TCON = Genre
         */
        else if (strcmp(frame_id, "TCON") == 0)
        {
            read_text(tag->genre,
                      data,
                      frame_size,
                      GENRE_SIZE);
        }

        /*
         * COMM = Comment
         */
        else if (strcmp(frame_id, "COMM") == 0)
        {
            /*
             * COMM structure:
             *
             * encoding = 1 byte
             * language = 3 bytes
             * description = variable
             * comment = variable
             *
             * For simple ASCII comments, find the last part.
             */
            if (frame_size > 4)
            {
                int start = 4;

                while (start < frame_size &&
                       data[start] != '\0')
                {
                    start++;
                }

                start++;

                if (start < frame_size)
                {
                    int len = frame_size - start;

                    if (len >= COMMENT_SIZE)
                    {
                        len = COMMENT_SIZE - 1;
                    }

                    memcpy(tag->comment,
                           data + start,
                           len);

                    tag->comment[len] = '\0';
                }
            }
        }

        free(data);
    }

    fclose(fp);

    return 1;
}

void display_tags(MP3Tag *tag)
{
    printf("\n");
    printf("------------------------------------------------\n");
    printf("                 MP3 TAG DETAILS\n");
    printf("------------------------------------------------\n");

    printf("TITLE   : %s\n",
           tag->title[0] ? tag->title : "Not Available");

    printf("ARTIST  : %s\n",
           tag->artist[0] ? tag->artist : "Not Available");

    printf("ALBUM   : %s\n",
           tag->album[0] ? tag->album : "Not Available");

    printf("YEAR    : %s\n",
           tag->year[0] ? tag->year : "Not Available");

    printf("GENRE   : %s\n",
           tag->genre[0] ? tag->genre : "Not Available");

    printf("COMMENT : %s\n",
           tag->comment[0] ? tag->comment : "Not Available");

    printf("------------------------------------------------\n");
}