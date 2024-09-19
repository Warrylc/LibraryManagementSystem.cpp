#include <iostream>
#include <string>
#include <unordered_map>
#include <list>

using namespace std;

// Book class
class Book {
public:
    string title;
    string author;
    string isbn;

    Book(string title, string author, string isbn) 
        : title(title), author(author), isbn(isbn) {}
};

// Transaction class
class Transaction {
public:
    string transactionID;
    string patronID;
    string bookISBN;

    Transaction(string id, string patronID, string bookISBN) 
        : transactionID(id), patronID(patronID), bookISBN(bookISBN) {}
};

// Patron class
class Patron {
public:
    string name;
    string id;
    list<Transaction> borrowedTransactions;

    Patron(string name, string id) : name(name), id(id) {}
};

// Library class
class Library {
private:
    Book* books[100]; // Array for books
    int bookCount;
    
    list<Patron*> patrons; // Linked list for patrons
    unordered_map<string, Transaction> transactions; // Hash table for transactions

public:
    Library() : bookCount(0) {}

    // Add a new book to the library
    void addBook(string title, string author, string isbn) {
        books[bookCount++] = new Book(title, author, isbn);
    }

    // Add a new patron
    void addPatron(string name, string id) {
        patrons.push_back(new Patron(name, id));
    }

    // Borrow a book
    void borrowBook(string patronID, string bookISBN, string transactionID) {
        // Find the patron
        for (auto patron : patrons) {
            if (patron->id == patronID) {
                // Create a transaction and add it to the hash table
                Transaction transaction(transactionID, patronID, bookISBN);
                transactions[transactionID] = transaction;

                // Add the transaction to the patron's borrowed list
                patron->borrowedTransactions.push_back(transaction);
                cout << patron->name << " borrowed book with ISBN: " << bookISBN << endl;
                return;
            }
        }
        cout << "Patron not found!" << endl;
    }

    // Return a book
    void returnBook(string transactionID) {
        if (transactions.find(transactionID) != transactions.end()) {
            string patronID = transactions[transactionID].patronID;

            // Find the patron and remove the transaction
            for (auto patron : patrons) {
                if (patron->id == patronID) {
                    patron->borrowedTransactions.remove_if([transactionID](Transaction &t) {
                        return t.transactionID == transactionID;
                    });
                    transactions.erase(transactionID);
                    cout << "Book returned by patron with ID: " << patronID << endl;
                    return;
                }
            }
        } else {
            cout << "Transaction not found!" << endl;
        }
    }

    // List all books
    void listBooks() {
        cout << "Available Books:" << endl;
        for (int i = 0; i < bookCount; ++i) {
            cout << "Title: " << books[i]->title << ", Author: " << books[i]->author << ", ISBN: " << books[i]->isbn << endl;
        }
    }

    // List all patrons
    void listPatrons() {
        cout << "Registered Patrons:" << endl;
        for (auto patron : patrons) {
            cout << "Name: " << patron->name << ", ID: " << patron->id << endl;
        }
    }
};

int main() {
    Library library;

    // Adding books
    library.addBook("1984", "George Orwell", "123456789");
    library.addBook("To Kill a Mockingbird", "Harper Lee", "987654321");

    // Adding patrons
    library.addPatron("Alice", "001");
    library.addPatron("Bob", "002");

    // Listing books and patrons
    library.listBooks();
    library.listPatrons();

    // Borrowing and returning books
    library.borrowBook("001", "123456789", "TX001");
    library.returnBook("TX001");

    return 0;
}
