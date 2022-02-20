#
# Specify build dir and executable target name.
#
BUILD_DIR=build
TARGET = $(BUILD_DIR)/clox
TESTS_DIR=tests
TESTS_OUT_DIR=build-tests

# Specify header and implementation files
HEADERS = $(shell find ./src -type f -name '*.h')
SOURCES  = $(shell find ./src -type f -name '*.c')
SOURCES_WITHOUT_MAIN  = $(shell find ./src -type f ! -name 'main.c' -name '*.c')

INCLUDES = -I./include
CCFLAGS = -g -Wall $(INCLUDES)

.PHONY: all compile run clean

all: compile

compile: $(SOURCES) $(HEADERS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CCFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	@./$(TARGET) examples/script.clox

# See https://www.ccs.neu.edu/home/skotthe/classes/cs5600/fall/2015/labs/intro-check.html
test:
	rm -rf $(TESTS_OUT_DIR)
	mkdir -p $(TESTS_OUT_DIR)

	$(CC) $(CCFLAGS) -c $(TESTS_DIR)/chunk_test.c -o $(TESTS_OUT_DIR)/chunk_test.o
	$(CC) $(CCFLAGS) $(SOURCES_WITHOUT_MAIN) -v $(TESTS_OUT_DIR)/chunk_test.o -lcheck -lm -lpthread -o $(TESTS_OUT_DIR)/test_chunk

	$(CC) $(CCFLAGS) -c $(TESTS_DIR)/scanner_test.c -o $(TESTS_OUT_DIR)/scanner_test.o
	$(CC) $(CCFLAGS) $(SOURCES_WITHOUT_MAIN) -v $(TESTS_OUT_DIR)/scanner_test.o -lcheck -lm -lpthread -o $(TESTS_OUT_DIR)/scanner_test

	@./$(TESTS_OUT_DIR)/test_chunk
	@./$(TESTS_OUT_DIR)/scanner_test

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(TESTS_OUT_DIR)
