# Simple Log Parser

A command-line log parser written in C++ for filtering and analyzing log files.

- Specific search patterns
- Output colored by log level (ERROR=red, WARN=yellow, INFO=green, DEBUG=blue)
- Case-insensitive search option with '-i' flag
- Sustainable for large log files (Tested on a 322 MB log file)
- Line numbers and match counting

## Build
```bash
g++ main.cpp -o logparser
```

## Usage

**Basic Search**
```bash
./logparser <example_name>.log "ERROR"
```

**Multiple Patterns**
```bash
./logparser server.log "ERROR" "WARNING" "FATAL"
```

**Case Insensitive**
```bash
./logparser server.log "error" "warning" -i
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
- [ ] Version 1.1
- [ ] Version 1.2
- [ ] Version 1.3
- [ ] Version 1.4
- [ ] Version 1.5
- [ ] Version 1.6
- [ ] Version 1.7
- [ ] Version 1.8
- [ ] Version 1.9
- [ ] Version 2.0

## Learning and Improvements
Built this project as part of my systems programming learning journey, please feel free to give feedback for future improvements! 

## License

MIT - See [LICENSE](LICENSE) file for details