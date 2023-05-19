CC = g++
CFLAGS = -std=c++17 -Wall
LDFLAGS = -lpthread

EXECUTABLE = pathfinder
SOURCES = $(wildcard *.cpp)

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean
