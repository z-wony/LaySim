CC=gcc
CFLAGS= -g $(shell pkg-config --cflags glib-2.0 elementary)
LDFLAGS= -g $(shell pkg-config --libs glib-2.0 elementary)
SOURCES=src/main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=laysim.bin

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -pthread -o $@ $(LDFLAGS)

.o:
	$(CC) $< -pthread -o $@  $(CFLAGS)

clean:
	rm $(OBJECTS) $(EXECUTABLE)
