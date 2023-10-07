CC = gcc
CFLAGS = -Wall
TARGET = server
SRCS = server.c


$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	rm -f $(TARGET)
