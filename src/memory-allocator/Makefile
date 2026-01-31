# Makefile for Custom Memory Allocator
# 
# This Makefile helps you compile and test the memory allocator
# with different configurations and debugging options.

# Compiler and flags
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
DEBUG_FLAGS = -g -DDEBUG -fsanitize=address
RELEASE_FLAGS = -O2 -DNDEBUG

# Source files
SOURCES = allocator.c test_allocator.c
HEADERS = allocator.h
TARGET = test_allocator

# Default target
all: $(TARGET)

# Build the test program
$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

# Debug build with extra checking
debug: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $(TARGET)_debug $(SOURCES)

# Release build with optimizations
release: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o $(TARGET)_release $(SOURCES)

# Run the test program
test: $(TARGET)
	./$(TARGET)

# Run with memory debugging (requires valgrind)
memcheck: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TARGET)_debug $(TARGET)_release *.o

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the test program (default)"
	@echo "  debug    - Build with debug symbols and AddressSanitizer"
	@echo "  release  - Build optimized release version"
	@echo "  test     - Build and run the test program"
	@echo "  memcheck - Run with Valgrind memory checking"
	@echo "  clean    - Remove build artifacts"
	@echo "  help     - Show this help message"

# Phony targets
.PHONY: all debug release test memcheck clean help