# own-c-shell

My own shell for learning purposes. I am looking for practice C and clarifying questions about how a shell interpreter handles processes in depth.

## Objectives

### Basic
- [X] Main loop with EOF detection
- [ ] User input reading with long line support
- [ ] Command parsing with quote and escape handling
- [ ] External command execution via PATH
- [ ] Error handling with descriptive messages

### Intermediate
- [ ] Built-in commands (exit, ls, cd, echo, pwd)
- [ ] Command history with file persistence
- [ ] Environment variables management (local/global)
- [ ] Alias support
- [ ] Autocomplete for commands and files

### Advanced
- [ ] I/O redirect (>, <, >>, 2>, &>)
- [ ] Multi-stage pipes (ls | grep txt | wc)
- [ ] Wildcards expansion with pattern support (*, ?, [abc])
- [ ] Background jobs (jobs, fg, bg, kill)
- [ ] Signals (SIGINT, SIGSTRIP, SIGHOLD)
- [ ] Subshells support

### Expert
- [ ] Script support
- [ ] User-Defined functions
- [ ] User-Defined file configuration
- [ ] TTY integration for colors and ANSI formatting
- [ ] Process sandboxing and input validation