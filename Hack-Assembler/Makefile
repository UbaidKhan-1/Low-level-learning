CC = clang
CFLAGS = -Wall -Wextra -Wno-int-conversion -std=c17

TARGET = assemble

OBJECTS = assembler.o \
          DictParser.o \
          hash_map.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

assembler.o: assembler.c
	$(CC) $(CFLAGS) -c $< -o $@

DictParser.o: ../Dict-Parser/DictParser.c ../Dict-Parser/DictParser.h
	$(CC) $(CFLAGS) -c $< -o $@

hash_map.o: ../DSA/Linear-Structures/hash_map.c \
            ../DSA/Linear-Structures/hash_map.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)