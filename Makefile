CC=gcc
EDJE_CC=edje_cc
CFLAGS= -g $(shell pkg-config --cflags glib-2.0 elementary)
LDFLAGS= -g $(shell pkg-config --libs glib-2.0 elementary)
SOURCES=src/main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=laysim.bin
THEMES=res/edc/theme.edc
EDJS=$(THEMES:.edc=.edj)

all: $(EXECUTABLE) $(EDJS)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -pthread -o $@ $(LDFLAGS)

.o:
	$(CC) $< -pthread -o $@ $(CFLAGS)

$(EDJS):
	$(EDJE_CC) $(THEMES)

clean:
	rm $(OBJECTS) $(EXECUTABLE) $(EDJS)
