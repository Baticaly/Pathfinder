CC = g++
CFLAGS = -std=c++17 -Wall -I$(STAGING_DIR)/usr/include/opencv4
LDFLAGS = -pthread -lstdc++fs -lstdc++ -lopencv_core -lopencv_imgcodecs -lopencv_videoio

EXECUTABLE = pathfinder
SOURCES = $(wildcard *.cpp)

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean