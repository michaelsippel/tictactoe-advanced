INCLUDE = -I./include

ifeq (,$(TARGET))
TARGET = linux64
endif

ifeq ($(TARGET),linux32)
AS = $(CXX) -m32
CC = $(CXX) -m32
LD = $(CXX) -m32
BIN = tictactoe_linux32
else ifeq ($(TARGET),linux64)
AS = $(CXX) -m64
CC = $(CXX) -m64
LD = $(CXX) -m64
BIN = tictactoe_linux64
else ifeq ($(TARGET),win32)
AS = i686-w64-mingw32-g++
CC = i686-w64-mingw32-g++
LD = i686-w64-mingw32-g++
BIN = tictactoe_win32.exe
else ifeq ($(TARGET),win64)
AS = x86_64-w64-mingw32-g++
CC = x86_64-w64-mingw32-g++
LD = x86_64-w64-mingw32-g++
BIN = tictactoe_win64.exe
endif

CFLAGS = -std=c++11 $(INCLUDE)
ASMFLAGS =
LDFLAGS = -lSDL2 -lGL

C_SRCS = $(shell find -name '*.cpp')
A_SRCS = $(shell find -name '*.S')
C_HDRS = $(shell find -name '*.h')
SRCS = $(C_SRCS) $(A_SRCS)
OBJS = $(addsuffix .$(TARGET).o,$(basename $(SRCS)))

$(BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.$(TARGET).o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

style: $(C_SRCS) $(C_HDRS)
	astyle --style=allman --indent-classes --indent-switches $^

clean:
	rm $(OBJS)
	rm $(BIN)
