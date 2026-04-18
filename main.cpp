#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

struct Book {
    std::string title;
    std::string author;
    int id;
    double price;
    int yearPublished;
    int pageCount;
    std::string genre;
    std::string summary;
    int copies;
};

std::vector<Book> loadBooksFromCSV(const std::string& filename) {
    std::vector<Book> books;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file.\n";
        return books;
    }

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line)) {
        std::stringstream ss(line);

        std::string title, author, genre, tmp;
        std::string rest;
        double price;
        int year, pages, copies;

        std::getline(ss, title, ',');
        std::getline(ss, author, ',');

        std::getline(ss, tmp, ',');
        price = std::stod(tmp);

        std::getline(ss, tmp, ',');
        year = std::stoi(tmp);

        std::getline(ss, tmp, ',');
        pages = std::stoi(tmp);

        std::getline(ss, genre, ',');

        std::getline(ss, rest); // summary + copies

        size_t lastComma = rest.find_last_of(',');
        std::string summary = rest.substr(0, lastComma);
        copies = std::stoi(rest.substr(lastComma + 1));

        Book b;
        b.title = title;
        b.author = author;
        b.id = books.size();
        b.price = price;
        b.yearPublished = year;
        b.pageCount = pages;
        b.genre = genre;
        b.summary = summary;
        b.copies = copies;

        books.push_back(b);
    }

    return books;
}

void printFull(const Book& b) {
    std::cout << "Title: " << b.title << " | "
              << "Author: " << b.author << " | "
              << "ID: " << b.id << " | "
              << "Price: " << b.price << " | "
              << "Year Published: " << b.yearPublished << " | "
              << "Page Count: " << b.pageCount << " | "
              << "Genre: " << b.genre << " | "
              << "Summary: " << b.summary << " | "
              << "Copies: " << b.copies << "\n";
}

void printCompact(const Book& b) {
    std::cout << b.id << "; "
              << b.title << "; "
              << b.author << "; "
              << b.copies << "\n";
}

void printAlphabetical(std::vector<Book> books, bool full) {
    std::sort(books.begin(), books.end(),
              [](const Book& a, const Book& b) {
                  return a.title < b.title;
              });

    for (const Book& b : books) {
        if (full) printFull(b);
        else printCompact(b);
    }
}

std::vector<Book> filterPrice(const std::vector<Book>& books, double maxPrice) {
    std::vector<Book> result;
    for (const Book& b : books)
        if (b.price < maxPrice) result.push_back(b);
    return result;
}

std::vector<Book> filterGenre(const std::vector<Book>& books, const std::string& genre) {
    std::vector<Book> result;
    for (const Book& b : books)
        if (b.genre == genre) result.push_back(b);
    return result;
}

std::vector<Book> filterAuthorAndPages(const std::vector<Book>& books,
                                       const std::string& author,
                                       int minPages) {
    std::vector<Book> result;
    for (const Book& b : books)
        if (b.author == author && b.pageCount > minPages)
            result.push_back(b);
    return result;
}

std::vector<Book> searchTitle(const std::vector<Book>& books,
                              const std::string& text) {
    std::vector<Book> result;
    for (const Book& b : books)
        if (b.title.find(text) != std::string::npos)
            result.push_back(b);
    return result;
}

void sellBook(Book& b) {
    if (b.copies <= 0) {
        std::cout << "ERROR: 0 copies of book '" << b.title << "' available\n";
        return;
    }

    b.copies--;

    std::ofstream file("sales.txt", std::ios::app);
    file << b.id << "; "
         << b.title << "; "
         << b.author << "; $"
         << b.price << "; "
         << b.copies << "\n";

    std::cout << b.title << " sold.\n";
}

int main(int argc, char* argv[]) {

    std::string bookFile = (argc > 1) ? argv[1] : "books.csv";

    std::cout << "=== 1. Load the list of books from a file. ===\n";
    std::vector<Book> books = loadBooksFromCSV(bookFile);

    std::cout << "\n=== 2. Print full information of book with ID=0. ===\n";
    printFull(books[0]);

    std::cout << "\n=== 3. Print compact form (ID, title, author, count) of book with ID=0. ===\n";
    printCompact(books[0]);

    std::cout << "\n=== 4. Update inventory count of book with ID=1 to `2`. ===\n";
    books[1].copies = 2;

    std::cout << "\n=== 5. Print the full information of book with ID=1 ===\n";
    printFull(books[1]);

    std::cout << "\n=== 6. Print the full alphabetized list of books in compact form ===\n";
    printAlphabetical(books, false);

    std::cout << "\n=== 7. Print compact info of all books that cost less than $10.51 ===\n";
    for (const Book& b : filterPrice(books, 10.51))
        printCompact(b);

    std::cout << "\n=== 8. Print compact info of all books that belong to the 'Fantasy' genre ===\n";
    for (const Book& b : filterGenre(books, "Fantasy"))
        printCompact(b);

    std::cout << "\n=== 9. Print compact info of all books written by 'J.R.R. Tolkien' AND longer than 325 pages ===\n";
    for (const Book& b : filterAuthorAndPages(books, "J.R.R. Tolkien", 325))
        printCompact(b);

    std::cout << "\n=== 10. Print compact info of all books whose titles contain the text 'and' ===\n";
    for (const Book& b : searchTitle(books, "and"))
        printCompact(b);

    std::cout << "\n=== 11. Perform a 'sale' of the book with ID=0 ===\n";
    sellBook(books[0]);

    std::cout << "\n=== 12. Perform a 'sale' of the book: 'The Lord of the Rings: The Two Towers' ===\n";
    for (Book& b : books)
        if (b.title == "The Lord of the Rings: The Two Towers")
            sellBook(b);

    std::cout << "\n=== 13. Attempt two more sales of the book with ID=0, second 'sale' should fail. ===\n";
    sellBook(books[0]);
    sellBook(books[0]);

    std::cout << "\n=== 14. Print the full info of all books in alphabetical order ===\n";
    printAlphabetical(books, true);

    return 0;
}
