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

- [x] Main loop with EOF detection
- [x] **User input reading with long line support**
  - _Learn_: Dynamic memory allocation, buffer management
  - _Key concepts_: `realloc()`, buffer overflow prevention
  - _Resources_: [GNU getline documentation](https://www.gnu.org/software/libc/manual/html_node/Line-Input.html)
- [x] **Command parsing with quote and escape handling**
  - _Learn_: State machines, lexical analysis
  - _Key concepts_: Tokenization, escape sequences, quote handling
  - _Challenge_: Handle nested quotes: `echo "He said 'hello'"`
- [x] **External command execution via PATH**
  - _Learn_: `fork()`, `exec()` family, `waitpid()`
  - _Key concepts_: Process creation, program loading
  - _Debug tip_: Use `strace` to trace system calls
- [x] **Error handling with descriptive messages**
  - _Learn_: `errno`, `perror()`, custom error reporting
  - _Key concepts_: Error propagation, user-friendly messages

### 🟡 Intermediate (Process Management - Est. 3-4 weeks)

**Core Concepts**: Built-ins, persistence, environment management

- [x] **Built-in commands (exit, ls, cd, echo, pwd)**
  - _Learn_: Function pointers, command dispatch tables
  - _Key concepts_: `chdir()`, `getcwd()`, built-in vs external
  - _Architecture_: Design extensible command system
- [x] **Command history with file persistence**
  - _Learn_: File I/O, data serialization, circular buffers
  - _Key concepts_: `~/.mosh_history`, efficient storage
  - _Challenge_: Implement history search and expansion
- [x] **Environment variables management (local/global)**
  - _Learn_: `environ`, `getenv()`, `setenv()`, variable scope
  - _Key concepts_: Process environment inheritance
  - _Challenge_: Implement variable expansion: `echo $HOME/docs`
- [x] **Alias support**
  - _Learn_: Hash tables, string replacement, recursive expansion
  - _Key concepts_: Alias resolution order, infinite recursion prevention
- [x] **Autocomplete for commands and files**
  - _Learn_: Directory traversal, string matching algorithms
  - _Key concepts_: `readdir()`, prefix matching, performance optimization

### 🔥 Advanced (IPC & Job Control - Est. 4-5 weeks)

**Core Concepts**: Pipes, redirection, signals, job control

- [ ] **I/O redirect (>, <, >>, 2>, &>)**
  - _Learn_: File descriptors, `dup2()`, stream redirection
  - _Key concepts_: `STDOUT_FILENO`, `STDERR_FILENO`, file creation modes
  - _Debug_: Use `ls -la /proc/self/fd/` to inspect open files
- [ ] **Multi-stage pipes (ls | grep txt | wc)**
  - _Learn_: `pipe()`, process chains, file descriptor inheritance
  - _Key concepts_: Inter-process communication, data flow
  - _Challenge_: Handle N-stage pipes dynamically
- [ ] **Wildcards expansion with pattern support (\*, ?, [abc])**
  - _Learn_: `glob()`, pattern matching, directory traversal
  - _Key concepts_: Filename generation, regex basics
- [ ] **Background jobs (jobs, fg, bg, kill)**
  - _Learn_: Process groups, session management, job tables
  - _Key concepts_: `setpgid()`, `tcsetpgrp()`, process states
  - _Challenge_: Implement job control like bash
- [ ] **Signals (SIGINT, SIGTSTP, SIGCHLD)**
  - _Learn_: Signal handling, async-safe functions, signal masks
  - _Key concepts_: `sigaction()`, signal delivery, race conditions
  - _Critical_: Handle zombie processes with SIGCHLD
- [ ] **Subshells support**
  - _Learn_: Process isolation, variable scoping, command grouping
  - _Key concepts_: `(command)` vs `{ command; }`

### 🎯 Expert (Language Features - Est. 5-6 weeks)

**Core Concepts**: Language design, terminal control, security

- [ ] **Script support**
  - _Learn_: File parsing, control structures, interpreters
  - _Key concepts_: Recursive descent parsing, AST building
  - _Challenge_: Implement if/while/for constructs
- [ ] **User-Defined functions**
  - _Learn_: Symbol tables, scope management, parameter passing
  - _Key concepts_: Local variables, function call stack
- [ ] **User-Defined file configuration**
  - _Learn_: Configuration parsing, initialization scripts
  - _Key concepts_: `~/.shellrc`, startup sequence
- [ ] **TTY integration for colors and ANSI formatting**
  - _Learn_: Terminal capabilities, `termios`, raw mode
  - _Key concepts_: `tcgetattr()`, escape sequences, cursor control
  - _Resources_: [ANSI escape codes](https://en.wikipedia.org/wiki/ANSI_escape_code)
- [ ] **Process sandboxing and input validation**
  - _Learn_: `chroot()`, `setuid()`, input sanitization
  - _Key concepts_: Security boundaries, privilege dropping
  - _Critical_: Prevent code injection attacks

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

- [x] Can execute simple commands: `ls`, `pwd`, `echo hello`
- [x] Proper error handling for invalid commands
- [x] Clean exit with Ctrl+D

### Week 4-6: Command Features

- [x] Command history works: `history`, `!!`, `!5`
- [x] Environment variables: `export VAR=value`, `echo $VAR`
- [x] Built-in commands work correctly

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
