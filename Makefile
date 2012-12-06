CC		= g++
CFLAGS		= -c -Wall
LDFLAGS		=
SOURCES		= main.cpp \
		Grid.cpp \
		Grid1D.cpp \
		Grid2D.cpp \
		Grid3D.cpp

OBJECTS		= $(SOURCES:.cpp=.o)
EXECUTABLE	= awesome

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(EXECUTABLE)