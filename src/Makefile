CC = g++
CFLAGS = -std=c++17 -Wall
LDFLAGS = -pthread -lstdc++

EXECUTABLE = pathfinder
SOURCES = $(wildcard *.cpp)

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean
