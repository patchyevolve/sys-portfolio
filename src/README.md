# Systems Programming Learning Projects

This directory contains complete, educational implementations of fundamental systems programming concepts. Each project is designed to teach core concepts through hands-on coding and detailed explanations.

## 🎯 Learning Objectives

By working through these projects, you'll learn:

- **Memory Management**: How malloc/free work internally
- **Cryptography**: Core algorithms like AES and SHA-256
- **Operating Systems**: Task scheduling and context switching
- **Low-level Programming**: Bit manipulation, pointer arithmetic
- **System Design**: How complex systems are built from simple components

## 📚 Projects Overview

### 1. Custom Memory Allocator (`memory-allocator/`)

**What you'll learn:**
- How dynamic memory allocation works
- Memory fragmentation and coalescing
- Linked list data structures
- Pointer manipulation in C

**Key concepts:**
- Block headers and metadata
- First-fit allocation strategy
- Memory alignment requirements
- Heap management techniques

**Files:**
- `allocator.h` - Complete API definition with detailed comments
- `allocator.c` - Full implementation with educational explanations
- `test_allocator.c` - Comprehensive test suite
- `Makefile` - Build system with debug options

### 2. Cryptographic Primitives (`crypto-primitives/`)

**What you'll learn:**
- How encryption algorithms work internally
- Cryptographic hash functions
- Bit manipulation and mathematical operations
- Security principles and best practices

**Key concepts:**
- AES (Advanced Encryption Standard)
- SHA-256 hash function
- S-boxes and substitution-permutation networks
- Key expansion algorithms

**Files:**
- `crypto.h` - Complete cryptographic API
- `aes.c` - Full AES implementation with detailed comments
- `sha256.c` - Complete SHA-256 implementation
- `utils.c` - Utility functions for hex conversion, etc.

### 3. RTOS Kernel (`rtos-kernel/`)

**What you'll learn:**
- How operating systems manage tasks
- Context switching and scheduling algorithms
- Preemptive multitasking concepts
- Synchronization primitives

**Key concepts:**
- Task Control Blocks (TCB)
- Round-robin scheduling with priorities
- Stack management
- Interrupt handling simulation

**Files:**
- `rtos.h` - Complete RTOS API definition
- `scheduler.c` - Task scheduler implementation
- `tasks.c` - Task management functions
- `sync.c` - Synchronization primitives

## 🚀 Getting Started

### Prerequisites

```bash
# Install GCC compiler
sudo apt-get install gcc make

# Optional: Install debugging tools
sudo apt-get install gdb valgrind

# Optional: Install static analysis tools
sudo apt-get install cppcheck clang-tools
```

### Building and Running

Each project has its own Makefile with multiple targets:

```bash
# Navigate to a project directory
cd memory-allocator/

# Build the project
make

# Run the test program
make test

# Build with debug symbols
make debug

# Run with memory checking (requires valgrind)
make memcheck

# Clean build artifacts
make clean
```

## 📖 Learning Path

### Beginner Level

1. **Start with Memory Allocator**
   - Understand basic C concepts
   - Learn about pointers and memory management
   - Practice debugging techniques

2. **Study the Code Structure**
   - Read header files first to understand APIs
   - Follow the implementation step by step
   - Run tests to see how everything works

### Intermediate Level

3. **Explore Cryptographic Primitives**
   - Learn about bit manipulation
   - Understand mathematical operations in programming
   - Study security considerations

4. **Modify and Experiment**
   - Add new features to existing code
   - Implement additional algorithms
   - Write your own test cases

### Advanced Level

5. **Dive into RTOS Concepts**
   - Understand operating system principles
   - Learn about concurrent programming
   - Study real-time system requirements

6. **Build Your Own Projects**
   - Combine concepts from multiple projects
   - Implement additional features
   - Optimize for performance

## 🔧 Code Organization

Each project follows a consistent structure:

```
project-name/
├── *.h           # Header files with API definitions
├── *.c           # Implementation files with detailed comments
├── test_*.c      # Test programs and examples
├── Makefile      # Build system
└── README.md     # Project-specific documentation
```

### Code Style Guidelines

- **Extensive Comments**: Every function and complex operation is explained
- **Educational Focus**: Code prioritizes clarity over optimization
- **Step-by-Step Learning**: Complex algorithms are broken into digestible pieces
- **Real-World Relevance**: Concepts mirror actual system implementations

## 🧪 Testing and Debugging

### Running Tests

```bash
# Basic functionality test
make test

# Memory leak detection
make memcheck

# Static analysis
cppcheck *.c

# Debug with GDB
gdb ./test_program
```

### Understanding Output

Each program provides detailed output showing:
- Step-by-step algorithm execution
- Memory allocation/deallocation traces
- Performance statistics
- Error conditions and handling

## 📚 Additional Learning Resources

### Books
- "Computer Systems: A Programmer's Perspective" by Bryant & O'Hallaron
- "Operating Systems: Three Easy Pieces" by Remzi Arpaci-Dusseau
- "Applied Cryptography" by Bruce Schneier

### Online Resources
- [OSDev Wiki](https://wiki.osdev.org/) - Operating system development
- [Cryptopals Challenges](https://cryptopals.com/) - Cryptography exercises
- [ARM Developer Documentation](https://developer.arm.com/) - ARM architecture

### Practice Exercises

1. **Memory Allocator Extensions**
   - Implement different allocation strategies (best-fit, worst-fit)
   - Add memory pool management
   - Implement garbage collection

2. **Cryptography Projects**
   - Add more encryption modes (CBC, CTR)
   - Implement RSA public-key cryptography
   - Build a simple TLS handshake

3. **RTOS Enhancements**
   - Add priority inheritance for mutexes
   - Implement message queues
   - Add real-time scheduling algorithms

## 🐛 Common Issues and Solutions

### Compilation Errors

```bash
# Missing headers
sudo apt-get install libc6-dev

# 32-bit compatibility (if needed)
sudo apt-get install gcc-multilib
```

### Runtime Issues

```bash
# Segmentation faults - use GDB
gdb ./program
(gdb) run
(gdb) bt  # backtrace when it crashes

# Memory leaks - use Valgrind
valgrind --leak-check=full ./program
```

### Understanding Complex Code

1. **Start with headers** - understand the API first
2. **Follow data flow** - trace how data moves through functions
3. **Use debugger** - step through code line by line
4. **Add print statements** - see what's happening at runtime
5. **Draw diagrams** - visualize data structures and algorithms

## 🎓 Assessment and Progress

### Self-Check Questions

After each project, ask yourself:

1. **Memory Allocator**
   - Can you explain how malloc() finds free memory?
   - What causes memory fragmentation?
   - How does coalescing work?

2. **Cryptographic Primitives**
   - What makes AES secure?
   - How do hash functions work?
   - Why is timing important in cryptography?

3. **RTOS Kernel**
   - How does the scheduler choose the next task?
   - What happens during a context switch?
   - How do synchronization primitives work?

### Practical Exercises

1. **Modify existing code** to add new features
2. **Write test cases** for edge conditions
3. **Optimize performance** while maintaining correctness
4. **Port code** to different architectures or platforms

## 🤝 Contributing and Feedback

This is an educational resource designed to help you learn systems programming. If you find issues or have suggestions:

1. **Document your learning** - keep notes on what you discover
2. **Share improvements** - suggest better explanations or examples
3. **Ask questions** - understanding is more important than just running code
4. **Teach others** - explaining concepts helps solidify your understanding

## 🏆 Next Steps

After mastering these concepts, consider:

1. **Real-world projects** - contribute to open-source systems software
2. **Advanced topics** - study specific architectures (ARM, x86, RISC-V)
3. **Specialized areas** - embedded systems, security, distributed systems
4. **Professional development** - systems programming roles in industry

Remember: The goal isn't just to make the code work, but to understand **why** it works and **how** it relates to real systems you use every day.

Happy learning! 🚀