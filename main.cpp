/*
 * Library Book Management System
 * OOPS Lab Mini Project (CIC-211)
 *
 * Features : add / display books, search by ID or title,
 *            issue a book (only if available), return a book.
 * Concepts : classes & objects, constructors, encapsulation, vectors,
 *            file handling, strings, exception handling.
 */

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

/* ---------------------------------------------------------------------
 * Book : one library book (data is private -> encapsulation)
 * ------------------------------------------------------------------- */
class Book {
private:
    int id;
    string title;
    string author;
    bool available;

public:
    // Default constructor
    Book() : id(0), title(""), author(""), available(true) {}

    // Parameterised constructor
    Book(int bookId, const string &bookTitle, const string &bookAuthor,
         bool isAvailable = true)
        : id(bookId), title(bookTitle), author(bookAuthor),
          available(isAvailable) {}

    // Getters
    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool isAvailable() const { return available; }

    // Setter (only availability changes after a book is created)
    void setAvailable(bool status) { available = status; }

    void display() const {
        cout << left << setw(8) << id << setw(32) << title << setw(24)
             << author << (available ? "Available" : "Issued") << "\n";
    }
};

/* ---------------------------------------------------------------------
 * Helper functions for strings and input
 * ------------------------------------------------------------------- */
string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return s;
}

string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// The '|' character is used as the file separator, so keep it out of text.
string cleanText(string s) {
    replace(s.begin(), s.end(), '|', '-');
    return trim(s);
}

int readInt(const string &prompt) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) throw runtime_error("Input closed.");
        try {
            size_t pos = 0;
            int value = stoi(trim(line), &pos);
            if (pos == trim(line).size()) return value;
        } catch (const exception &) {
            // fall through to the error message below
        }
        cout << "Please enter a valid number.\n";
    }
}

string readText(const string &prompt) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) throw runtime_error("Input closed.");
        line = cleanText(line);
        if (!line.empty()) return line;
        cout << "This field cannot be empty.\n";
    }
}

/* ---------------------------------------------------------------------
 * Library : manages the collection of books and the file
 * ------------------------------------------------------------------- */
class Library {
private:
    vector<Book> books;
    string fileName;

    int findIndexById(int id) const {
        for (size_t i = 0; i < books.size(); i++)
            if (books[i].getId() == id) return static_cast<int>(i);
        return -1;
    }

    void printHeader() const {
        cout << left << setw(8) << "ID" << setw(32) << "Title" << setw(24)
             << "Author" << "Status\n";
        cout << string(76, '-') << "\n";
    }

    // Write all books to the file
    void save() const {
        ofstream out(fileName);
        if (!out) throw runtime_error("Could not save data to " + fileName);
        for (const Book &b : books)
            out << b.getId() << '|' << b.getTitle() << '|' << b.getAuthor()
                << '|' << (b.isAvailable() ? 1 : 0) << '\n';
    }

    // Read all books from the file (if it exists)
    void load() {
        ifstream in(fileName);
        if (!in) return;  // first run: no file yet

        string line;
        while (getline(in, line)) {
            if (trim(line).empty()) continue;
            stringstream ss(line);
            string idStr, title, author, availStr;
            if (getline(ss, idStr, '|') && getline(ss, title, '|') &&
                getline(ss, author, '|') && getline(ss, availStr, '|')) {
                try {
                    books.push_back(
                        Book(stoi(idStr), title, author, availStr == "1"));
                } catch (const exception &) {
                    // skip corrupted line
                }
            }
        }
    }

public:
    explicit Library(const string &file = "books.txt") : fileName(file) {
        load();
    }

    void addBook(int id, const string &title, const string &author) {
        if (id <= 0) throw invalid_argument("Book ID must be a positive number.");
        if (findIndexById(id) != -1)
            throw invalid_argument("A book with ID " + to_string(id) +
                                   " already exists.");
        books.push_back(Book(id, title, author, true));
        save();
    }

    void displayAll() const {
        if (books.empty()) {
            cout << "No books in the library yet.\n";
            return;
        }
        printHeader();
        for (const Book &b : books) b.display();
    }

    void searchById(int id) const {
        int idx = findIndexById(id);
        if (idx == -1) throw runtime_error("No book found with ID " + to_string(id) + ".");
        printHeader();
        books[idx].display();
    }

    void searchByTitle(const string &title) const {
        string key = toLower(title);
        bool found = false;
        for (const Book &b : books) {
            if (toLower(b.getTitle()).find(key) != string::npos) {
                if (!found) printHeader();
                b.display();
                found = true;
            }
        }
        if (!found) throw runtime_error("No book found with title containing \"" + title + "\".");
    }

    void issueBook(int id) {
        int idx = findIndexById(id);
        if (idx == -1) throw runtime_error("No book found with ID " + to_string(id) + ".");
        if (!books[idx].isAvailable())
            throw runtime_error("Book \"" + books[idx].getTitle() + "\" is already issued.");
        books[idx].setAvailable(false);
        save();
        cout << "Book \"" << books[idx].getTitle() << "\" issued successfully.\n";
    }

    void returnBook(int id) {
        int idx = findIndexById(id);
        if (idx == -1) throw runtime_error("No book found with ID " + to_string(id) + ".");
        if (books[idx].isAvailable())
            throw runtime_error("Book \"" + books[idx].getTitle() + "\" was not issued.");
        books[idx].setAvailable(true);
        save();
        cout << "Book \"" << books[idx].getTitle() << "\" returned successfully.\n";
    }
};

/* ---------------------------------------------------------------------
 * Menu
 * ------------------------------------------------------------------- */
void showMenu() {
    cout << "\n===== LIBRARY BOOK MANAGEMENT SYSTEM =====\n"
         << "1. Add Book\n"
         << "2. Display All Books\n"
         << "3. Search Book (by ID or Title)\n"
         << "4. Issue Book\n"
         << "5. Return Book\n"
         << "6. Exit\n";
}

int main() {
    Library library("books.txt");

    try {
        while (true) {
            showMenu();
            int choice = readInt("Enter choice: ");

            try {
                switch (choice) {
                case 1: {
                    int id = readInt("Enter Book ID: ");
                    string title = readText("Enter Title: ");
                    string author = readText("Enter Author: ");
                    library.addBook(id, title, author);
                    cout << "Book added successfully.\n";
                    break;
                }
                case 2:
                    library.displayAll();
                    break;
                case 3: {
                    int mode = readInt("Search by (1) ID or (2) Title: ");
                    if (mode == 1) {
                        library.searchById(readInt("Enter Book ID: "));
                    } else if (mode == 2) {
                        library.searchByTitle(readText("Enter Title: "));
                    } else {
                        cout << "Invalid search option.\n";
                    }
                    break;
                }
                case 4:
                    library.issueBook(readInt("Enter Book ID to issue: "));
                    break;
                case 5:
                    library.returnBook(readInt("Enter Book ID to return: "));
                    break;
                case 6:
                    cout << "Thank you for using the Library System. Goodbye!\n";
                    return 0;
                default:
                    cout << "Invalid choice. Please select 1-6.\n";
                }
            } catch (const exception &e) {
                // Errors from one operation do not stop the program
                cout << "Error: " << e.what() << "\n";
            }
        }
    } catch (const exception &e) {
        cout << "\n" << e.what() << " Exiting.\n";
    }
    return 0;
}
