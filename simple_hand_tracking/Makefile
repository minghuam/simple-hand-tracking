CC = g++
CFLAGS = -c -Wall $(shell pkg-config opencv --cflags)
LDFLAGS = $(shell pkg-config opencv --libs)
SOURCES = main.cpp
OBJS = $(SOURCES:.cpp = .o)
BUILDIR = ../build
TARGET = $(BUILDIR)/simple_hand_tracking

all: $(SOURCES) $(TARGET)
	cp palm.xml $(BUILDIR)/palm.xml

$(TARGET) : $(OBJS)
	test -d $(BUILDIR) || mkdir $(BUILDIR)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(BUILDIR)/palm.xml
	rm $(TARGET)
