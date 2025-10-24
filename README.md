# Simple Log Parser

A command-line log parser written in C++ for filtering and analyzing log files.

- Specific search patterns
- Log timestamp filtering with '-from' and '-to' flags
- Stack trace preservation
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

**Date Range Filtering** (New)
```bash
# errors btw specific times
./logparser server.log "ERROR" -from "2025-10-21 08:30:00" -to "2025-10-21 09:00:00"

# errors after specific time
./logparser server.log "ERROR" -from "2025-10-21 08:45:00"

# warnings before specific time
./logparser server.log "WARN" -to "2025-10-21 08:30:00"

# combination with other flags
./logparser server.log "error|warning" -r -i -from "2025-10-21 08:00:00"
```

**Supported Date Formats**
- 'YYYY-MM-DD HH:MM:SS' (e.g., '2025-10-21 08:30:00')
- 'DD-MM-YYYY HH:MM:SS' (e.g., '21-10-2025 08:30:00')
- 'MM-DD-YYYY HH:MM:SS' (e.g., '10-21-2025 08:30:00')

Lines without timestamps (stack traces, multi-line messages, etc.) are included if they match the search pattern, even if date filtering is enabled.

## Example Output
```
[0:L20] 2025-10-21 08:34:42.100 [ERROR] [SecurityService] Failed to notify admin: SMTP connection timeout
[1:L32] 2025-10-21 08:38:12.111 [ERROR] [OrderService] Database timeout while updating orderId=1023
[2:L33] 2025-10-21 08:38:12.112 [ERROR] [OrderService] Stack trace:
[3:L47] 2025-10-21 08:47:10.223 [ERROR] [PaymentService] Payment declined: transactionId=TXN98455

Total Matches: 4
```

## Roadmap
- [x] Version 1.0 - Basic pattern search, colored display
- [x] Version 1.1 - Case-insensitive search, regex support
- [x] Version 1.2 - Date range filtering
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