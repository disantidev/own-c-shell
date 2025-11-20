# own-c-shell

A comprehensive shell implementation project for mastering C systems programming. This project progressively builds from basic I/O handling to advanced process management, providing hands-on experience with Unix internals.

## 🎯 Learning Goals

- **Systems Programming**: Master process creation, IPC, and signal handling
- **C Programming**: Advanced memory management, data structures, and error handling  
- **Unix Internals**: Deep understanding of shells, terminals, and process control
- **Software Architecture**: Design scalable, modular systems in C

## 📋 Implementation Roadmap

### 🟢 Basic (Foundation - Est. 2-3 weeks)
**Core Concepts**: Input handling, basic parsing, process basics

- [X] Main loop with EOF detection
- [X] **User input reading with long line support**
  - *Learn*: Dynamic memory allocation, buffer management
  - *Key concepts*: `realloc()`, buffer overflow prevention
  - *Resources*: [GNU getline documentation](https://www.gnu.org/software/libc/manual/html_node/Line-Input.html)
- [X] **Command parsing with quote and escape handling** 
  - *Learn*: State machines, lexical analysis
  - *Key concepts*: Tokenization, escape sequences, quote handling
  - *Challenge*: Handle nested quotes: `echo "He said 'hello'"`
- [X] **External command execution via PATH**
  - *Learn*: `fork()`, `exec()` family, `waitpid()`
  - *Key concepts*: Process creation, program loading
  - *Debug tip*: Use `strace` to trace system calls
- [X] **Error handling with descriptive messages**
  - *Learn*: `errno`, `perror()`, custom error reporting
  - *Key concepts*: Error propagation, user-friendly messages

### 🟡 Intermediate (Process Management - Est. 3-4 weeks)
**Core Concepts**: Built-ins, persistence, environment management

- [X] **Built-in commands (exit, ls, cd, echo, pwd)**
  - *Learn*: Function pointers, command dispatch tables
  - *Key concepts*: `chdir()`, `getcwd()`, built-in vs external
  - *Architecture*: Design extensible command system
- [X] **Command history with file persistence**
  - *Learn*: File I/O, data serialization, circular buffers
  - *Key concepts*: `~/.mosh_history`, efficient storage
  - *Challenge*: Implement history search and expansion
- [ ] **Environment variables management (local/global)**
  - *Learn*: `environ`, `getenv()`, `setenv()`, variable scope
  - *Key concepts*: Process environment inheritance
  - *Challenge*: Implement variable expansion: `echo $HOME/docs`
- [ ] **Alias support**
  - *Learn*: Hash tables, string replacement, recursive expansion
  - *Key concepts*: Alias resolution order, infinite recursion prevention
- [ ] **Autocomplete for commands and files**
  - *Learn*: Directory traversal, string matching algorithms
  - *Key concepts*: `readdir()`, prefix matching, performance optimization

### 🔥 Advanced (IPC & Job Control - Est. 4-5 weeks)  
**Core Concepts**: Pipes, redirection, signals, job control

- [ ] **I/O redirect (>, <, >>, 2>, &>)**
  - *Learn*: File descriptors, `dup2()`, stream redirection
  - *Key concepts*: `STDOUT_FILENO`, `STDERR_FILENO`, file creation modes
  - *Debug*: Use `ls -la /proc/self/fd/` to inspect open files
- [ ] **Multi-stage pipes (ls | grep txt | wc)**
  - *Learn*: `pipe()`, process chains, file descriptor inheritance
  - *Key concepts*: Inter-process communication, data flow
  - *Challenge*: Handle N-stage pipes dynamically
- [ ] **Wildcards expansion with pattern support (*, ?, [abc])**
  - *Learn*: `glob()`, pattern matching, directory traversal
  - *Key concepts*: Filename generation, regex basics
- [ ] **Background jobs (jobs, fg, bg, kill)**
  - *Learn*: Process groups, session management, job tables
  - *Key concepts*: `setpgid()`, `tcsetpgrp()`, process states
  - *Challenge*: Implement job control like bash
- [ ] **Signals (SIGINT, SIGTSTP, SIGCHLD)**
  - *Learn*: Signal handling, async-safe functions, signal masks
  - *Key concepts*: `sigaction()`, signal delivery, race conditions
  - *Critical*: Handle zombie processes with SIGCHLD
- [ ] **Subshells support**
  - *Learn*: Process isolation, variable scoping, command grouping
  - *Key concepts*: `(command)` vs `{ command; }`

### 🎯 Expert (Language Features - Est. 5-6 weeks)
**Core Concepts**: Language design, terminal control, security

- [ ] **Script support**
  - *Learn*: File parsing, control structures, interpreters
  - *Key concepts*: Recursive descent parsing, AST building
  - *Challenge*: Implement if/while/for constructs
- [ ] **User-Defined functions**
  - *Learn*: Symbol tables, scope management, parameter passing
  - *Key concepts*: Local variables, function call stack
- [ ] **User-Defined file configuration**
  - *Learn*: Configuration parsing, initialization scripts
  - *Key concepts*: `~/.shellrc`, startup sequence
- [ ] **TTY integration for colors and ANSI formatting**
  - *Learn*: Terminal capabilities, `termios`, raw mode
  - *Key concepts*: `tcgetattr()`, escape sequences, cursor control
  - *Resources*: [ANSI escape codes](https://en.wikipedia.org/wiki/ANSI_escape_code)
- [ ] **Process sandboxing and input validation**
  - *Learn*: `chroot()`, `setuid()`, input sanitization
  - *Key concepts*: Security boundaries, privilege dropping
  - *Critical*: Prevent code injection attacks

## 🛠️ Development Workflow

### Setup and Tools
```bash
# Development dependencies
brew install valgrind cunit gdb

# Recommended VS Code extensions
- C/C++ Extension Pack
- GitLens
- Error Lens
```

### Testing Strategy
```bash
# Memory leak detection
make valgrind

# Unit testing  
make test

# Manual testing with edge cases
echo 'echo "test with spaces"' | ./build/main
```

### Debugging Techniques
```bash
# Debug with GDB
gdb ./build/main
(gdb) set args < test_input.txt
(gdb) run

# Trace system calls
strace -e trace=process ./build/main

# Check open file descriptors
ls -la /proc/$(pgrep main)/fd/
```

## 📚 Essential Resources

### Systems Programming
- **Book**: "Advanced Programming in the UNIX Environment" (Stevens)
- **Book**: "The Linux Programming Interface" (Kerrisk)  
- **Online**: [Beej's Guide to Unix IPC](https://beej.us/guide/bgipc/)

### Shell Implementation
- **Reference**: [POSIX Shell Standard](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- **Study**: [bash source code](https://git.savannah.gnu.org/cgit/bash.git) (for inspiration)
- **Tutorial**: [Write a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/)

### C Programming  
- **Book**: "Expert C Programming" (van der Linden)
- **Reference**: [C99 Standard](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf)
- **Tool**: [Valgrind Manual](https://valgrind.org/docs/manual/manual.html)

## 🧪 Testing Challenges

### Unit Tests for Each Feature
```c
// Example test cases to implement
void test_quote_parsing(void);
void test_pipe_creation(void);  
void test_signal_handling(void);
void test_memory_management(void);
```

### Integration Tests
```bash
# Create comprehensive test suite
./tests/run_integration_tests.sh

# Test edge cases
echo 'ls | grep "file with spaces" | wc -l' | ./build/main
echo 'command_that_doesnt_exist' | ./build/main
```

### Performance Tests
```bash
# Test with large inputs
seq 1 10000 | ./build/main -c 'while read line; do echo $line; done'

# Memory usage profiling
time valgrind --tool=massif ./build/main < large_script.sh
```

## 🎯 Learning Milestones

### Week 2-3: Basic Shell
- [ ] Can execute simple commands: `ls`, `pwd`, `echo hello`
- [ ] Proper error handling for invalid commands
- [ ] Clean exit with Ctrl+D

### Week 4-6: Command Features  
- [ ] Command history works: `history`, `!!`, `!5`
- [ ] Environment variables: `export VAR=value`, `echo $VAR`
- [ ] Built-in commands work correctly

### Week 7-10: Process Control
- [ ] Pipes work: `ls | grep .c | wc -l`
- [ ] Redirection works: `ls > files.txt 2>&1`
- [ ] Background jobs: `sleep 10 &`, `jobs`, `fg`

### Week 11-15: Advanced Features
- [ ] Signal handling: Ctrl+C doesn't kill shell
- [ ] Wildcards: `ls *.c`, `echo file[1-3].txt`
- [ ] Subshells: `(cd /tmp && ls)`

### Week 16-20: Expert Level
- [ ] Script execution: `./myscript.sh`
- [ ] User functions: `myfunction() { echo $1; }`
- [ ] Color support and terminal control

## 🔧 Architecture Guidelines

### Code Organization
```
src/
├── main.c              # Entry point
├── mosh.c/.h          # Main shell loop
├── parser.c/.h         # Command parsing
├── executor.c/.h       # Command execution  
├── builtins.c/.h       # Built-in commands
├── history.c/.h        # Command history
├── jobs.c/.h           # Job control
├── signals.c/.h        # Signal handling
└── utils.c/.h          # Utility functions
```

### Design Patterns
- **Command Pattern**: For built-in commands
- **Observer Pattern**: For signal handling
- **Strategy Pattern**: For different execution modes
- **Factory Pattern**: For command creation

### Memory Management Rules
1. **Every malloc() needs a corresponding free()**
2. **Check all return values**
3. **Use valgrind regularly**
4. **Initialize all pointers to NULL**
5. **Free resources in reverse allocation order**

## 🚨 Common Pitfalls to Avoid

### Memory Issues
- Buffer overflows in string handling
- Memory leaks in error paths
- Double-free errors
- Use-after-free bugs

### Process Management
- Zombie processes from unreaped children
- Signal race conditions
- File descriptor leaks
- Incorrect process group handling

### Parsing Bugs
- Incorrect quote handling
- Command injection vulnerabilities  
- Buffer overflow in token parsing
- Infinite loops in recursive parsing

## 🏆 Project Completion Criteria

### Functional Requirements
- [ ] Passes all POSIX shell compliance tests
- [ ] Zero memory leaks (valgrind clean)
- [ ] Handles all edge cases gracefully
- [ ] Performance comparable to dash/bash for basic operations

### Code Quality
- [ ] 90%+ test coverage
- [ ] Comprehensive documentation
- [ ] Clean, readable, maintainable code
- [ ] Follows C coding standards

### Knowledge Demonstration
- [ ] Can explain every line of code
- [ ] Understands all system calls used
- [ ] Can debug complex issues independently
- [ ] Ready for systems programming roles

---

**Estimated Total Time**: 16-20 weeks of dedicated development
**Difficulty Level**: Advanced (suitable for senior C developers)
**Learning Value**: Exceptional - equivalent to advanced systems programming course
