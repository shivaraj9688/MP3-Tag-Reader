#ifndef TYPES_H
#define TYPES_H

#define TITLE_SIZE   128
#define ARTIST_SIZE  128
#define ALBUM_SIZE   128
#define YEAR_SIZE    16
#define GENRE_SIZE   128
#define COMMENT_SIZE 128

typedef struct
{
    char title[TITLE_SIZE];
    char artist[ARTIST_SIZE];
    char album[ALBUM_SIZE];
    char year[YEAR_SIZE];
    char genre[GENRE_SIZE];
    char comment[COMMENT_SIZE];

} MP3Tag;

typedef enum
{
    VIEW,
    EDIT,
    HELP,
    UNKNOWN
} OperationType;

#endif