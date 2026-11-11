# Library Management System (C++)

A console application for managing a small library, built with C++ and object-oriented programming.

## Features
- Add, list, and delete books
- Search by title or author (case-insensitive)
- Borrow and return books
- Data is saved permanently in `books.txt`

## Concepts used
- OOP: `Book` and `Library` classes, encapsulation
- STL: `vector`, `string`, `algorithm`
- File I/O: `ifstream` / `ofstream`
- Input validation

## How to run
```bash
g++ -std=c++17 main.cpp -o library
./library
```
On Windows, run `library.exe` after compiling.

## Author
Adel Abdelrahman
