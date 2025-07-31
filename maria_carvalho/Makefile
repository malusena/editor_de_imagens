CC = gcc

SRCS = main.c estrutura.c imagens_ppm.c

TARGET = main

$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) $(SRCS:.c=.o)
