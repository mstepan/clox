
   
TARGET = clox

SRCS  = $(shell find ./src -type f -name *.c)
HEADS = $(shell find ./src -type f -name *.h)
OBJS = $(SRCS:.c=.o)
DEPS = Makefile.depend

INCLUDES = -I./include
CXXFLAGS = -g -Wall $(INCLUDES)
LDFLAGS = -lm


all: $(TARGET)

$(TARGET): $(OBJS) $(HEADS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

run: all
	@./$(TARGET)

.PHONY: depend clean
depend:
	$(CXX) $(INCLUDES) -MM $(SRCS) > $(DEPS)
	@sed -i -E "s/^(.+?).o: ([^ ]+?)\1/\2\1.o: \2\1/g" $(DEPS)

clean:
	$(RM) $(OBJS) $(TARGET)

-include $(DEPS)