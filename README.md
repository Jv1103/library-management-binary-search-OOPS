# 📚 Library Book Management System

A console-based C++ application to manage library books and basic issue/return operations, built as the **OOPS Lab (CIC-211) Mini Project** at Maharaja Agrasen Institute of Technology (MAIT), Delhi (GGSIPU).

## Features

- **Add books** with a unique Book ID, title, author and availability status
- **Display** all books in a table
- **Search** a book by ID or by title (case-insensitive)
- **Issue** a book only if it is available (marks it as *Issued*)
- **Return** a book and update its availability back to *Available*
- **File handling:** records are saved to and loaded from a file, so data persists between runs
- **Exception handling** for duplicate IDs, book not found, already-issued and not-issued cases

## OOP Concepts Demonstrated

| Concept | Where used |
|---|---|
| Classes and objects | `Book` and `Library` classes |
| Constructors | Parameterised `Book` constructor, default `Library` constructor |
| Encapsulation | Private data members with getters and setters |
| Arrays / vectors | `vector<Book>` stores the book collection |
| File handling | `ifstream` / `ofstream` for loading and saving records |
| Strings | Title/author handling, title search |
| Exception handling | `try` / `catch` for invalid operations |

## Tech Stack

- Language: **C++**
- Libraries: STL (`vector`, `string`, `fstream`, `exception`)
- Compiler: g++ (any C++11 or later compiler works)

## Project Structure

```
Library-Book-Management-System/
├── main.cpp        # Source code (menu + Book and Library classes)
├── books.txt       # Data file where book records are stored
└── README.md
```

> Update this section to match your actual file names.

## How to Run

```bash
# 1. Clone the repository
git clone https://github.com/your-username/your-repo-name.git
cd your-repo-name

# 2. Compile
g++ main.cpp -o library

# 3. Run
./library          # on Windows: library.exe
```

## Menu Options

```
1. Add Book
2. Display All Books
3. Search Book (by ID or Title)
4. Issue Book
5. Return Book
6. Exit
```

## Sample Output

```
Enter choice: 4
Enter Book ID: 101
Book "Data Structures" issued successfully.

Enter choice: 4
Enter Book ID: 101
Error: Book is already issued.
```

> Replace with your own output or add screenshots in a `screenshots/` folder.

## Future Scope

- Graphical or web-based interface
- Member accounts, due dates and fine calculation
- Database (SQLite / MySQL) instead of a text file
- Sorting, category-based search and reports

## SDG Mapping

- **SDG 4:** Quality Education
- **SDG 9:** Industry, Innovation and Infrastructure

## Author
Jagratt Varshney
B.Tech (Information Technology), 3rd Semester, MAIT, Delhi
Subject: Object Oriented Programming Lab (CIC-211)
