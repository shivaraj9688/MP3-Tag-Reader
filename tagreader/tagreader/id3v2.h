#ifndef ID3V2_H
#define ID3V2_H

#include "types.h"

int read_id3v2(const char *filename, MP3Tag *tag);

int id3v2_exists(const char *filename);

void display_tags(MP3Tag *tag);

#endif