#ifndef ID3V1_H
#define ID3V1_H

#include "types.h"

int read_id3v1(const char *filename, MP3Tag *tag);

int write_id3v1(const char *filename, MP3Tag *tag);

int id3v1_exists(const char *filename);

#endif