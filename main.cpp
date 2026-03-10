// ============================================================
// Unix-Like Command Shell for Windows
// ============================================================

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <direct.h>   // _chdir() and _getcwd() — Windows only
#include <fstream>    // For 'touch' functionality

using namespace std;

// ── Step 1: Unix → Windows command translations ──
unordered_map<string, string> commandMap = {
    { "ls",    "dir"  },
    { "clear", "cls"  },
    { "rm",    "del"  },
    { "cp",    "copy" },
    { "mv",    "move" },
    // --- New custom commands ---
    { "cat",   "type" },        // Read a file
    { "mkdir", "mkdir" },       // Make a folder
    { "rmdir", "rmdir" },       // Remove a folder (needs to be empty)
    { "grep",  "findstr" },     // Search for text in files
    { "echo",  "echo" }         // Print text to terminal
    // NOTE: "cd", "pwd", and "touch" are handled as real built-ins below
    //       so they are NOT in this map
};

// ── Step 2: Split input line into words ──
// Passing line by reference (const string&) prevents copying the whole string memory!
vector<string> splitInput(const string& line) {
    vector<string> words;
    string word;
    istringstream stream(line);
    while (stream >> word)
        words.push_back(word);
    return words;
}

// ── Step 3: Get the shell's current directory ──
string getCurrentDir() {
    char buffer[512];
    _getcwd(buffer, 512);
    return string(buffer);
}

// ── Step 4: Show help screen ──
void showHelp() {
    cout << "\n  Available commands:\n\n";
    cout << "  Unix      Windows Equivalents\n";
    cout << "  ----      -------------------\n";
    for (auto pair : commandMap)
        cout << "  " << pair.first << "  ->  " << pair.second << "\n";
    
    cout << "\n  Built-ins (Handled internally by C++):\n";
    cout << "  cd <path> - change directory (moves the shell)\n";
    cout << "  pwd       - print current directory\n";
    cout << "  touch <f> - create an empty file\n";
    cout << "  help      - show this screen\n";
    cout << "  exit      - quit the shell\n\n";
}

// ── Step 5: Translate and run the command ──
// Passing words by reference (const vector<string>&) is memory-efficient!
void runCommand(const vector<string>& words) {
    string cmd = words[0];

    // --- Built-in: help ---
    if (cmd == "help") {
        showHelp();
        return;
    }

    // --- Built-in: pwd ---
    if (cmd == "pwd") {
        cout << getCurrentDir() << "\n";
        return;
    }

    // --- Built-in: cd ---
    if (cmd == "cd") {
        if (words.size() < 2) {
            cout << getCurrentDir() << "\n";  
            return;
        }
        if (_chdir(words[1].c_str()) != 0)
            cout << "[myshell] Cannot find path: " << words[1] << "\n";
        return;
    }

    // --- Built-in: touch ---
    // Windows doesn't have a simple 'touch' command. We can easily implement 
    // it in C++ using an output file stream (ofstream).
    if (cmd == "touch") {
        if (words.size() < 2) {
            cout << "[myshell] touch requires a filename (e.g. touch file.txt)\n";
            return;
        }
        // std::ofstream automatically creates an empty file if it doesn't exist
        std::ofstream file(words[1]); 
        if (file) {
            cout << "[myshell] Created file: " << words[1] << "\n";
            file.close();
        } else {
             cout << "[myshell] Error creating file: " << words[1] << "\n";
        }
        return;
    }

    // --- Translate Unix command to Windows if needed ---
    if (commandMap.count(cmd))
        cmd = commandMap[cmd];   // e.g. "cat" → "type"

    // --- Attach any arguments ---
    for (size_t i = 1; i < words.size(); i++)
        cmd += " " + words[i];

    // --- Run it ---
    int result = system(cmd.c_str());
    if (result != 0)
        cout << "[myshell] Command failed (code " << result << ")\n";
}

// ── Step 6: Main loop ──
int main() {
    cout << "Unix-Like Command Shell for Windows\n";
    cout << "Type 'help' to see commands, 'exit' to quit.\n\n";

    string inputLine;

    while (true) {
        // Show current directory above prompt so you always know where you are
        cout << getCurrentDir() << "\n";
        cout << "myshell> ";

        if (!getline(cin, inputLine)) {
            cout << "\n[myshell] Input stream closed. Exiting...\n";
            break;
        }

        vector<string> words = splitInput(inputLine);

        if (words.empty())
            continue;

        if (words[0] == "exit") {
            cout << "Goodbye!\n";
            break;
        }

        runCommand(words);
    }

    return 0;
}