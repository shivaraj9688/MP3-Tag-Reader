CC = gcc
CFLAGS = -Wall -Wextra
TARGET = mp3tag
SRCS = main.c mp3_tag.c id3v1.c id3v2.c editor.c
OBJS = $(SRCS:.c=.o)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
main.o: main.c mp3_tag.h id3v1.h id3v2.h editor.h types.h
	$(CC) $(CFLAGS) -c main.c
mp3_tag.o: mp3_tag.c mp3_tag.h types.h
	$(CC) $(CFLAGS) -c mp3_tag.c
id3v1.o: id3v1.c id3v1.h types.h
	$(CC) $(CFLAGS) -c id3v1.c
id3v2.o: id3v2.c id3v2.h types.h
	$(CC) $(CFLAGS) -c id3v2.c
editor.o: editor.c editor.h id3v1.h id3v2.h types.h
	$(CC) $(CFLAGS) -c editor.c
clean:
	rm -f $(OBJS) $(TARGET)