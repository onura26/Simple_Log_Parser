# Simple Log Parser

A command-line log parser written in C++ for filtering and analyzing log files.

- Specific search patterns
- Output colored by log level (ERROR=red, WARN=yellow, INFO=green, DEBUG=blue)
- Case-insensitive search option with '-i' flag
- Regular expression search with '-r' flag
- Sustainable for large log files (Tested on a 322 MB log file)
- Line numbers and match counting
- Modular structure

## Build

**Using Make (recommended):**
```bash
make
```

**Manual compilation:**
```bash
g++ -std=c++17 main.cpp src/*.cpp -o logparser
```

## Usage

**Basic Search**
```bash
./logparser <example_name>.log "ERROR"
```

**Multiple Patterns**
```bash
./logparser server.log "ERROR" "WARNING" "INFO"
```

**Case Insensitive**
```bash
./logparser server.log "error" "warning" -i
```

**Regular Expression**
```bash
./logparser server.log "ERROR.*Payment" -r

# phone numbers
./logparser server.log "\d{3}-\d{3}-\d{4}" -r

# user or session ids
./logparser server.log "(userId|sessionId)=\d+" -r

# case-insensitive regex
./logparser server.log "error|warning" -r -i

# and etc.
```

## Example Output
```
[0] 2025-10-21 08:34:42.100 [ERROR] [SecurityService] Failed to notify admin
[1] 2025-10-21 08:38:12.111 [ERROR] [OrderService] Database timeout
[2] 2025-10-21 08:47:10.223 [ERROR] [PaymentService] Payment declined

Total matches: 3 lines
```

## Roadmap
- [x] Version 1.0
- [x] Version 1.1
- [ ] Version 1.2
- [ ] Version 1.3
- [ ] Version 1.4
- [ ] Version 1.5
- [ ] Version 1.6
- [ ] Version 1.7
- [ ] Version 1.8
- [ ] Version 1.9
- [ ] Version 2.0

## Requirements
- C++17 compiler (g++, clang++)
- Linux/Unix terminal with ANSI color support

## Learning and Improvements
Built this project as part of my systems programming learning journey, please feel free to give feedback for future improvements! 

## License

MIT - See [LICENSE](LICENSE) file for details