#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <filesystem>
#include <algorithm>
#include <windows.h>
#include <cstdlib>
#include <limits>

void setConsoleTextColor(int colorCode) {
    std::cout << "\033[" << colorCode << "m";
}

struct JournalEntry {
    std::string date;
    std::string content;
    size_t wordCount;
};

std::vector<JournalEntry> loadEntries(const std::string& journalFilename) {
    std::vector<JournalEntry> entries;
    std::ifstream infile(journalFilename);

    std::string date, content;
    size_t wordCount;
    while (std::getline(infile, date)) {
        std::getline(infile, content);
        infile >> wordCount;
        infile.ignore();
        entries.push_back({ date, content, wordCount });
    }

    return entries;
}

void saveEntries(const std::string& journalFilename, const std::vector<JournalEntry>& entries) {
    std::ofstream outfile(journalFilename, std::ios::out | std::ios::trunc);

    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open the file for writing." << std::endl;
        return;
    }

    for (const auto& entry : entries) {
        outfile << entry.date << std::endl
            << entry.content << std::endl
            << "-----" << std::endl;
    }

    outfile.close();
}


void saveEntry(const JournalEntry& entry, const std::string& journalFilename) {
    std::ofstream outfile(journalFilename, std::ios::app);

    outfile << entry.date << std::endl;
    outfile << entry.content << std::endl;
    outfile << entry.wordCount << std::endl;
}

void deleteEntry(std::vector<JournalEntry>& entries) {
    if (entries.empty()) {
        std::cout << "There are no entries to delete." << std::endl;
        return;
    }

    int entryNumber;
    std::cout << "Enter the entry number you want to delete: ";
    std::cin >> entryNumber;
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    int maxEntryNumber = std::distance(entries.begin(), std::prev(entries.end()));

    if (entryNumber < 0 || entryNumber > maxEntryNumber) {
        std::cout << "Invalid entry number. Please enter a number between 0 and " << maxEntryNumber << std::endl;
        return;
    }

    entries.erase(entries.begin() + entryNumber);
    std::cout << "Entry " << entryNumber << " has been deleted." << std::endl;
}


std::vector<std::string> listJournalFiles() {
    std::vector<std::string> journalFiles;

    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        if (entry.path().extension() == ".txt") {
            journalFiles.push_back(entry.path().filename().string());
        }
    }

    return journalFiles;
}

bool deleteJournalFile(const std::string& journalFilename) {
    return std::filesystem::remove(journalFilename);
}

size_t wordCount(const std::string& content) {
    size_t count = 0;
    bool inWord = false;

    for (char c : content) {
        if (std::isspace(c)) {
            inWord = false;
        }
        else if (!inWord) {
            inWord = true;
            ++count;
        }
    }

    return count;
}

std::string truncateContent(const std::string& content, size_t maxLength) {
    if (content.length() <= maxLength) {
        return content;
    }

    return content.substr(0, maxLength - 3) + "...";
}

void setColor(int colorCode) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorCode);
}

bool journalApp(const std::string& journalFilename);

int main() {
    std::cout << "Welcome to the Command Line Journal App!" << std::endl;

    while (true) {
        std::cout << "Choose an option:" << std::endl;
        std::cout << "1. Create a new journal" << std::endl;
        std::cout << "2. Open an existing journal" << std::endl;
        std::cout << "3. Delete a journal" << std::endl;
        std::cout << "4. Quit" << std::endl;

        int option;
        std::cin >> option;
        std::cin.ignore();

        std::string journalFilename;

        switch (option) {
        case 1: {
            std::cout << "Enter the name of the new journal: ";
            std::string newJournalName;
            std::getline(std::cin, newJournalName);
            journalFilename = newJournalName + ".txt";
            std::ofstream newJournalFile(journalFilename);
            newJournalFile.close();
            break;
        }
        case 2: {
            std::vector<std::string> journalFiles = listJournalFiles();
            if (journalFiles.empty()) {
                std::cout << "No existing journal files found.  Enter the name of the new journal: ";
                std::string newJournalName;
                std::getline(std::cin, newJournalName);
                journalFilename = newJournalName + ".txt";
                std::ofstream newJournalFile(journalFilename);
                newJournalFile.close();
                break;
            }
            std::cout << "Select a journal file:" << std::endl;
            for (size_t i = 0; i < journalFiles.size(); ++i) {
                std::cout << (i + 1) << ". " << journalFiles[i] << std::endl;
            }
            int fileIndex;
            std::cin >> fileIndex;
            std::cin.ignore();
            if (fileIndex > 0 && fileIndex <= journalFiles.size()) {
                journalFilename = journalFiles[fileIndex - 1];
            }
            else {
                std::cout << "Invalid selection. Exiting." << std::endl;
                return 1;
            }
            break;
        }
        case 3: {
            std::vector<std::string> journalFiles = listJournalFiles();
            if (journalFiles.empty()) {
                std::cout << "No existing journal files found." << std::endl;
                break;
            }
            std::cout << "Select a journal file to delete:" << std::endl;
            for (size_t i = 0; i < journalFiles.size(); ++i) {
                std::cout << (i + 1) << ". " << journalFiles[i] << std::endl;
            }
            int fileIndex;
            std::cin >> fileIndex;
            std::cin.ignore();
            if (fileIndex > 0 && fileIndex <= journalFiles.size()) {
                journalFilename = journalFiles[fileIndex - 1];
                if (deleteJournalFile(journalFilename)) {
                    std::cout << "Successfully deleted " << journalFilename << std::endl;
                    std::cout << journalFiles.size() << std::endl;
                }
                else {
                    std::cout << "Failed to delete " << journalFilename << std::endl;
                }
            }
            else {
                std::cout << "Invalid selection." << std::endl;
            }
            break;
        }
        case 4:
            return 0;
        default:
            std::cout << "Invalid option. Exiting." << std::endl;
            return 1;
        }

        if (!journalFilename.empty()) {
            if (!journalApp(journalFilename)) {
                deleteJournalFile(journalFilename);
                std::cout << "Deleted " << journalFilename << std::endl;
                journalFilename.clear();
            }
        }

        //journalApp(journalFilename);
    }

    return 0;

}

bool journalApp(const std::string& journalFilename) {
    std::vector<JournalEntry> entries = loadEntries(journalFilename);

    while (true) {
        std::cout << "Enter a command (new, view, read, wordcount, current, change, delete, deleteEntry, quit): ";
        std::string command;
        std::getline(std::cin, command);

        if (command == "new") {
            std::cout << "Here's a writing prompt to get you started: " << std::endl;
            setConsoleTextColor(31); // Set text color to red
            system("C:\\Users\\Steve\\AppData\\Local\\Programs\\Python\\Python311\\python.exe writing_prompt.py");
            setConsoleTextColor(0); // Reset text color to default
            std::cout << ">";
            std::string content;
            std::getline(std::cin, content);

            time_t rawtime;
            time(&rawtime);
            struct tm timeinfo;
            localtime_s(&timeinfo, &rawtime);
            char buffer[80];
            strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &timeinfo);
            std::string date(buffer);

            JournalEntry entry{ date, content, wordCount(content) };
            entries.push_back(entry);
            saveEntry(entry, journalFilename);

        }
        else if (command == "view") {
            for (size_t i = 0; i < entries.size(); ++i) {
                const JournalEntry& entry = entries[i];
                std::cout << (i + 1) << ". ";
                setColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
                std::cout << entry.date;
                setColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                std::cout << " - " << entry.wordCount << " words - ";
                setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                std::cout << truncateContent(entry.content, 20) << std::endl;
            }
        }
        else if (command == "read") {
            size_t entryNumber;
            std::cout << "Enter the entry number: ";
            std::cin >> entryNumber;
            std::cin.ignore();

            if (entryNumber > 0 && entryNumber <= entries.size()) {
                const JournalEntry& entry = entries[entryNumber - 1];
                std::cout << entry.date << std::endl;
                std::cout << entry.content << std::endl;
            }
            else {
                std::cout << "Invalid entry number." << std::endl;
            }
        }
        else if (command == "wordcount") {
            size_t entryNumber;
            std::cout << "Enter the entry number: ";
            std::cin >> entryNumber;
            std::cin.ignore();

            if (entryNumber > 0 && entryNumber <= entries.size()) {
                const JournalEntry& entry = entries[entryNumber - 1];
                std::cout << "Word count for entry " << entryNumber << ": " << entry.wordCount << std::endl;
            }
            else {
                std::cout << "Invalid entry number." << std::endl;
            }
        }
        else if (command == "current") {
            std::cout << "Current journal file: " << journalFilename << std::endl;
        }
        else if (command == "change") {
            break;
        }
        else if (command == "deleteEntry") {
            deleteEntry(entries);
            saveEntries(journalFilename, entries);
        }
        else if (command == "delete") {
            return false;
        }
        else if (command == "quit") {
            exit(0);
        }
        else {
            std::cout << "Invalid command." << std::endl;
        }
    }

    return true;
}