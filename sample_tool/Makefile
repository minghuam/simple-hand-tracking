CC = g++
CFLAGS = -c -Wall $(shell pkg-config opencv --cflags)
LDFLAGS = $(shell pkg-config opencv --libs)
SOURCES = main.cpp
OBJS = $(SOURCES:.cpp = .o)
BUILDIR = ../build
RAWDIR = $(BUILDIR)/raw
TARGET = $(BUILDIR)/sample_tool

all: $(SOURCES) $(TARGET)

$(TARGET) : $(OBJS)
	test -d $(BUILDIR) || mkdir $(BUILDIR)
	test -d $(RAWDIR) || mkdir $(RAWDIR)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(TARGET)
