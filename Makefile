#
# Specify build dir and executable target name.
#
BUILD_DIR=build
TARGET = $(BUILD_DIR)/clox

# Specify header and implementation files
HEADS = $(shell find ./src -type f -name *.h)
SRCS  = $(shell find ./src -type f -name *.c)

INCLUDES = -I./include
CCFLAGS = -g -Wall $(INCLUDES)

.PHONY: all compile run clean

all: compile

compile: $(SRCS) $(HEADS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CCFLAGS) $(SRCS) -o $(TARGET)

run: $(TARGET)
	@./$(TARGET) examples/script.clox

clean:
	rm -rf $(BUILD_DIR)
