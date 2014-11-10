CC = g++
CFLAGS = -c -Wall $(shell pkg-config opencv --cflags)
LDFLAGS = $(shell pkg-config opencv --libs)
SOURCES = main.cpp ../libsvm/svm.cpp
OBJS = $(SOURCES:.cpp = .o)
BUILDIR = ../build
TARGET = $(BUILDIR)/hog_svm_hand_tracking

all: $(SOURCES) $(TARGET)

$(TARGET) : $(OBJS)
	test -d $(BUILDIR) || mkdir $(BUILDIR)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(TARGET)
