# Unix-Like Command Shell for Windows

A lightweight, interactive command-line interface (CLI) written in C++ that allows users to execute common Unix/Linux commands natively within a Windows environment.

This project was built to demonstrate an understanding of C++ memory constraints, Standard Template Library (STL) data structures, string parsing algorithms, and native Windows OS APIs.

## Features

- **Command Translation:** Parses standard Unix commands (e.g., `ls`, `grep`, `cat`) and translates them into their equivalent Windows commands (e.g., `dir`, `findstr`, `type`) before passing them to the operating system for execution.
- **Optimized Lookups:** Utilizes `std::unordered_map` to process command translations with O(1) time complexity.
- **Memory Efficient Input Handling:** Implements constant references (`const std::vector<std::string>&`) during array passing to prevent unnecessary memory reallocation and reduce execution overhead.
- **Custom Built-ins:** 
  - `cd`: Implements state-persistent directory traversal using the `_chdir()` Windows API, allowing the shell's active working directory to update rather than instantiating a localized temporary subprocess.
  - `pwd`: Utilizes the `_getcwd()` function to return the true working directory.
  - `touch`: Implements custom C++ `<fstream>` logic to create empty files directly, bypassing OS-level shell calls.

## Supported Commands

| Unix Command | Windows Equivalent | Description |
| :--- | :--- | :--- |
| `ls` | `dir` | Lists directory contents |
| `clear` | `cls` | Clears the terminal screen |
| `rm` | `del` | Removes (deletes) a file |
| `rmdir` | `rmdir` | Removes an empty directory |
| `cp` | `copy` | Copies files to a new location |
| `mv` | `move` | Moves or renames files/directories |
| `cat` | `type` | Prints the contents of a text file |
| `mkdir` | `mkdir` | Creates a new directory |
| `grep` | `findstr` | Searches for text within files |
| `echo` | `echo` | Prints text to the terminal window |
| `cd` | *Built-in* | Changes the current working directory |
| `pwd` | *Built-in* | Prints the current working directory |
| `touch` | *Built-in* | Creates an empty file |
| `help` | *Built-in* | Displays all available commands |
| `exit` | *Built-in* | Closes the shell application |

## Compilation and Execution

This project uses standard C++ features and native Windows headers (`<direct.h>`). It requires a C++ compiler such as GCC (MinGW) or MSVC.

### GCC / MinGW
1. Open a terminal in the project directory.
2. Compile the source code using C++17 or later:
   ```bash
   g++ -std=c++17 -o myshell.exe main.cpp
   ```
3. Run the compiled executable:
   ```bash
   .\myshell.exe
   ```

## Technical Architecture

The application runs an infinite `while` loop that listens for strings over `std::cin`. Upon receiving a buffered input (terminated by a newline), the following sequence occurs:

1. **Tokenization:** The raw string is passed by constant reference to an `std::istringstream` engine, which splits the command into an `std::vector` of individual string arguments.
2. **Built-in Filtering:** The `runCommand` function analyzes the index 0 element. If it matches a built-in operation (`cd`, `pwd`, `touch`, `help`, `exit`), custom C++ execution paths are triggered and the function returns early.
3. **OS Fallback:** If the command is not a built-in, the application checks the `std::unordered_map` for an existing Windows translation. If a translation exists, the leading command string is replaced, the arguments are concatenated, and the final combined string is dispatched to the standard `system()` function.
