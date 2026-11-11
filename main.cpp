// Library Management System - C++ console app
// Features: OOP, STL vectors, file persistence, CRUD, search

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Book {
private:
    int id;
    string title;
    string author;
    bool borrowed;

public:
    Book(int id, string title, string author, bool borrowed = false)
        : id(id), title(move(title)), author(move(author)), borrowed(borrowed) {}

    int getId() const { return id; }
    const string& getTitle() const { return title; }
    const string& getAuthor() const { return author; }
    bool isBorrowed() const { return borrowed; }
    void setBorrowed(bool b) { borrowed = b; }

    void print() const {
        cout << "[" << id << "] " << title << " - " << author
             << (borrowed ? "  (Borrowed)" : "  (Available)") << "\n";
    }

    // File format: id|title|author|borrowed
    string serialize() const {
        return to_string(id) + "|" + title + "|" + author + "|" + (borrowed ? "1" : "0");
    }

    static bool deserialize(const string& line, Book& out) {
        stringstream ss(line);
        string idStr, title, author, flag;
        if (!getline(ss, idStr, '|') || !getline(ss, title, '|') ||
            !getline(ss, author, '|') || !getline(ss, flag, '|'))
            return false;
        try {
            out = Book(stoi(idStr), title, author, flag == "1");
        } catch (...) {
            return false;
        }
        return true;
    }
};

class Library {
private:
    vector<Book> books;
    string filename;
    int nextId = 1;

    Book* findById(int id) {
        for (auto& b : books)
            if (b.getId() == id) return &b;
        return nullptr;
    }

    static string toLower(string s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

public:
    explicit Library(string file) : filename(move(file)) { load(); }

    void load() {
        ifstream in(filename);
        string line;
        while (getline(in, line)) {
            Book b(0, "", "");
            if (Book::deserialize(line, b)) {
                books.push_back(b);
                nextId = max(nextId, b.getId() + 1);
            }
        }
    }

    void save() const {
        ofstream out(filename);
        for (const auto& b : books) out << b.serialize() << "\n";
    }

    void addBook(const string& title, const string& author) {
        books.emplace_back(nextId++, title, author);
        save();
        cout << "Book added.\n";
    }

    void listBooks() const {
        if (books.empty()) {
            cout << "No books yet.\n";
            return;
        }
        for (const auto& b : books) b.print();
    }

    void search(const string& keyword) const {
        string k = toLower(keyword);
        bool found = false;
        for (const auto& b : books) {
            if (toLower(b.getTitle()).find(k) != string::npos ||
                toLower(b.getAuthor()).find(k) != string::npos) {
                b.print();
                found = true;
            }
        }
        if (!found) cout << "No matches.\n";
    }

    void borrowBook(int id) {
        Book* b = findById(id);
        if (!b) cout << "Book not found.\n";
        else if (b->isBorrowed()) cout << "Already borrowed.\n";
        else { b->setBorrowed(true); save(); cout << "Enjoy your book!\n"; }
    }

    void returnBook(int id) {
        Book* b = findById(id);
        if (!b) cout << "Book not found.\n";
        else if (!b->isBorrowed()) cout << "This book was not borrowed.\n";
        else { b->setBorrowed(false); save(); cout << "Book returned.\n"; }
    }

    void deleteBook(int id) {
        auto it = remove_if(books.begin(), books.end(),
                            [id](const Book& b) { return b.getId() == id; });
        if (it == books.end()) {
            cout << "Book not found.\n";
            return;
        }
        books.erase(it, books.end());
        save();
        cout << "Book deleted.\n";
    }
};

int readInt(const string& prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a number.\n";
    }
}

string readLine(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

int main() {
    Library lib("books.txt");
    while (true) {
        cout << "\n=== Library Management ===\n"
                "1. Add book\n2. List books\n3. Search\n"
                "4. Borrow\n5. Return\n6. Delete\n0. Exit\n";
        int choice = readInt("Choice: ");
        switch (choice) {
            case 1: {
                string t = readLine("Title: ");
                string a = readLine("Author: ");
                lib.addBook(t, a);
                break;
            }
            case 2: lib.listBooks(); break;
            case 3: lib.search(readLine("Keyword: ")); break;
            case 4: lib.borrowBook(readInt("Book ID: ")); break;
            case 5: lib.returnBook(readInt("Book ID: ")); break;
            case 6: lib.deleteBook(readInt("Book ID: ")); break;
            case 0: cout << "Goodbye!\n"; return 0;
            default: cout << "Invalid choice.\n";
        }
    }
}
