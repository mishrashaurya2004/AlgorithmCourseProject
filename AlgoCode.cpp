#include <iostream>
#include <string>
#include <cctype> // For tolower() function
using namespace std;

// Updated struct to include a destructor
struct Book {
    string name;
    string author;
    int yearOfPublication;
    bool issued; // Flag indicating whether the book is issued
    Book* next;

    // Destructor to free memory when a book is deleted
    ~Book() {
        delete next;
    }
};

// Function to make a string lowercase
string toLowercase(const string& str) {
    string result = str;
    for (char& c : result) {
        c = tolower(c);
    }
    return result;
}

// Function to add a book to the linked list
void addBook(Book*& head, const std::string& name, const std::string& author, int yearOfPublication) {
    Book* newBook = new Book{ name, author, yearOfPublication, false, nullptr };

    if (head == nullptr || yearOfPublication < head->yearOfPublication) {
        newBook->next = head;
        head = newBook;
    } else {
        Book* current = head;
        while (current->next != nullptr && current->next->yearOfPublication < yearOfPublication) {
            current = current->next;
        }
        newBook->next = current->next;
        current->next = newBook;
    }
}

// Function to delete a book from the linked list
void deleteBook(Book*& head, const std::string& name) {
    if (head == nullptr) {
        cout << "Book not found. List is empty.\n";
        return;
    }

    if (head->name == name) {
        Book* temp = head;
        head = head->next;
        delete temp;
        cout << "Book deleted successfully.\n";
        return;
    }

    Book* current = head;
    while (current->next != nullptr && current->next->name != name) {
        current = current->next;
    }

    if (current->next == nullptr) {
        std::cout << "Book not found.\n";
    } else {
        Book* temp = current->next;
        current->next = current->next->next;
        delete temp;
        cout << "Book deleted successfully.\n";
    }
}

// Updated searchBook function to be case-insensitive
const Book* searchBook(const Book* head, const std::string& name) {
    string lowerCaseName = toLowercase(name);
    while (head != nullptr) {
        if (toLowercase(head->name) == lowerCaseName) {
            return head;
        }
        head = head->next;
    }
    return nullptr;
}

// Function to count the number of books in the library
int countBooks(const Book* head) {
    int count = 0;
    while (head != nullptr) {
        count++;
        head = head->next;
    }
    return count;
}

// Updated bubbleSort function to be case-insensitive
void bubbleSort(Book*& head, int sortBy) {
    if (head == nullptr || head->next == nullptr) {
        return; // No or one book, no need to sort
    }

    bool swapped;
    Book* last = nullptr;
    do {
        swapped = false;
        Book* current = head;
        Book* prev = nullptr;

        while (current->next != last) {
            bool swapCondition = false;
            switch (sortBy) {
                case 1: // Sort by publishing year
                    swapCondition = (current->yearOfPublication > current->next->yearOfPublication);
                    break;
                case 2: // Sort by author
                    swapCondition = (toLowercase(current->author) > toLowercase(current->next->author));
                    break;
                case 3: // Sort by title
                    swapCondition = (toLowercase(current->name) > toLowercase(current->next->name));
                    break;
            }

            if (swapCondition) {
                if (prev == nullptr) {
                    head = current->next;
                } else {
                    prev->next = current->next;
                }

                current->next = current->next->next;
                if (prev == nullptr) {
                    head = current;
                } else {
                    prev->next->next = current;
                }
                swapped = true;
            } else {
                prev = current;
                current = current->next;
            }
        }
        last = current;
    } while (swapped);
}

// Function to issue a book
void issueBook(Book* head, const std::string& name) {
    Book* foundBook = const_cast<Book*>(searchBook(head, name));

    if (foundBook != nullptr) {
        if (!foundBook->issued) {
            foundBook->issued = true;
            cout << "Book '" << foundBook->name << "' issued successfully.\n";
        } else {
            cout << "Book '" << foundBook->name << "' is already issued.\n";
        }
    } else {
        cout << "Book not found!\n";
    }
}

// Function to submit a book
void submitBook(Book* head, const std::string& name) {
    Book* foundBook = const_cast<Book*>(searchBook(head, name));

    if (foundBook != nullptr) {
        if (foundBook->issued) {
            foundBook->issued = false;
            cout << "Book '" << foundBook->name << "' submitted successfully.\n";
        } else {
            cout << "Book '" << foundBook->name << "' is not issued.\n";
        }
    } else {
        cout << "Book not found!\n";
    }
}

// Function to display books in the linked list
void displayBooks(const Book* head) {
    while (head != nullptr) {
        cout << "Title: " << head->name << "\n";
        cout << "Author: " << head->author << "\n";
        cout << "Year of Publication: " << head->yearOfPublication << "\n";
        cout << "Issued: " << (head->issued ? "Yes" : "No") << "\n";
        cout << "--------------------------\n";
        head = head->next;
    }
}

// Function to free memory allocated for the linked list
void deleteLibrary(Book*& head) {
    while (head != nullptr) {
        Book* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Head of the linked list
    Book* library = nullptr;

    // Main loop for the library management system
    while (true) {
        cout << "\n1. Add a book\n";
        cout << "2. Delete a book\n";
        cout << "3. Search for a book\n";
        cout << "4. Display books\n";
        cout << "5. Sort books\n";
        cout << "6. Issue a book\n";
        cout << "7. Submit a book\n";
        cout << "8. Count books\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            // Add a book
            string name, author;
            int year;

            cout << "Enter the title of the book: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter the author of the book: ";
            getline(cin, author);
            cout << "Enter the year of publication: ";
            cin >> year;

            addBook(library, name, author, year);
            cout << "Book added successfully!\n";
        } else if (choice == 2) {
            // Delete a book
            string name;
            cout << "Enter the title of the book to delete: ";
            cin.ignore();
            getline(cin, name);

            deleteBook(library, name);
        } else if (choice == 3) {
            // Search for a book
            string name;
            cout << "Enter the title of the book to search: ";
            cin.ignore();
            getline(cin, name);

            const Book* result = searchBook(library, name);
            if (result != nullptr) {
                cout << "Book found:\n";
                cout << "Title: " << result->name << "\n";
                cout << "Author: " << result->author << "\n";
                cout << "Year of Publication: " << result->yearOfPublication << "\n";
                cout << "Issued: " << (result->issued ? "Yes" : "No") << "\n";
                cout << "--------------------------\n";
            } else {
                cout << "Book not found!\n";
            }
        } else if (choice == 4) {
            // Display books
            displayBooks(library);
        } else if (choice == 5) {
            // Sort books
            cout << "1. Sort by publishing year\n";
            cout << "2. Sort by author\n";
            cout << "3. Sort by title\n";
            cout << "Enter your choice for sorting: ";

            int sortChoice;
            cin >> sortChoice;

            bubbleSort(library, sortChoice);
            displayBooks(library);
            cout << "Books sorted successfully!\n";
        } else if (choice == 6) {
            // Issue a book
            string name;
            cout << "Enter the title of the book to issue: ";
            cin.ignore();
            getline(cin, name);

            issueBook(library, name);
        } else if (choice == 7) {
            // Submit a book
            string name;
            cout << "Enter the title of the book to submit: ";
            cin.ignore();
            getline(cin, name);

            submitBook(library, name);
        } else if (choice == 8) {
            // Count books
            cout << "Total number of books in the library: " << countBooks(library) << "\n";
        } else if (choice == 9) {
            // Exit the program
            deleteLibrary(library);
            break;
        } else {
            cout << "Invalid choice. Please enter a valid option.\n";
        }
    }

    return 0;
}




********************************************************


#include <iostream>
#include <string>
using namespace std;

// Define a struct to represent a book
struct Book {
    string name;
    string author;
    int yearOfPublication;
    Book* next; // Pointer to the next book in the linked list
};

// Function to add a book to the linked list
void addBook(Book*& head, const std::string& name, const std::string& author, int yearOfPublication) {
    Book* newBook = new Book{ name, author, yearOfPublication, nullptr };

    if (head == nullptr || yearOfPublication < head->yearOfPublication) {
        newBook->next = head;
        head = newBook;
    } else {
        Book* current = head;
        while (current->next != nullptr && current->next->yearOfPublication < yearOfPublication) {
            current = current->next;
        }
        newBook->next = current->next;
        current->next = newBook;
    }
}

// Function to delete a book from the linked list
void deleteBook(Book*& head, const std::string& name) {
    if (head == nullptr) {
        cout << "Book not found. List is empty.\n";
        return;
    }

    if (head->name == name) {
        Book* temp = head;
        head = head->next;
        delete temp;
        cout << "Book deleted successfully.\n";
        return;
    }

    Book* current = head;
    while (current->next != nullptr && current->next->name != name) {
        current = current->next;
    }

    if (current->next == nullptr) {
        std::cout << "Book not found.\n";
    } else {
        Book* temp = current->next;
        current->next = current->next->next;
        delete temp;
        cout << "Book deleted successfully.\n";
    }
}

// Function to search for a book in the linked list
const Book* searchBook(const Book* head, const std::string& name) {
    while (head != nullptr) {
        if (head->name == name) {
            return head;
        }
        head = head->next;
    }
    return nullptr; // Book not found
}

// Function to sort books using bubble sort based on a given criteria
void bubbleSort(Book*& head, int sortBy) {
    if (head == nullptr || head->next == nullptr) {
        return; // No or one book, no need to sort
    }

    bool swapped;
    Book* last = nullptr;
    do {
        swapped = false;
        Book* current = head;
        Book* prev = nullptr;

        while (current->next != last) {
            bool swapCondition = false;
            switch (sortBy) {
                case 1: // Sort by publishing year
                    swapCondition = (current->yearOfPublication > current->next->yearOfPublication);
                    break;
                case 2: // Sort by author
                    swapCondition = (current->author > current->next->author);
                    break;
                case 3: // Sort by title
                    swapCondition = (current->name > current->next->name);
                    break;
            }

            if (swapCondition) {
    if (prev == nullptr) {
        head = current->next;
    } else {
        prev->next = current->next;
    }

    current->next = current->next->next;
    if (prev == nullptr) {
        head = current;
    } else {
        prev->next->next = current;
    }
    swapped = true;
} else {
    prev = current;
    current = current->next;
}
        }
        last = current;
    } while (swapped);
}

// Function to display books in the linked list
void displayBooks(const Book* head) {
    while (head != nullptr) {
        cout << "Title: " << head->name << "\n";
        cout << "Author: " << head->author << "\n";
        cout << "Year of Publication: " << head->yearOfPublication << "\n";
        cout << "--------------------------\n";
        head = head->next;
    }
}

// Function to free memory allocated for the linked list
void deleteLibrary(Book*& head) {
    while (head != nullptr) {
        Book* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    // Head of the linked list
    Book* library = nullptr;

    // Main loop for the library management system
    while (true) {
        cout << "1. Add a book\n";
        cout << "2. Delete a book\n";
        cout << "3. Search for a book\n";
        cout << "4. Display books\n";
        cout << "5. Sort books\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            // Add a book
            string name, author;
            int year;

            cout << "Enter the title of the book: ";
            cin >> name;
            cout << "Enter the author of the book: ";
            cin >> author;
            cout << "Enter the year of publication: ";
            cin >> year;

            addBook(library, name, author, year);
            cout << "Book added successfully!\n";
        } else if (choice == 2) {
            // Delete a book
            string name;
            cout << "Enter the title of the book to delete: ";
            cin >> name;

            deleteBook(library, name);
        } else if (choice == 3) {
            // Search for a book
            string name;
            cout << "Enter the title of the book to search: ";
            cin >> name;

            const Book* result = searchBook(library, name);
            if (result != nullptr) {
                cout << "Book found:\n";
                cout << "Title: " << result->name << "\n";
                cout << "Author: " << result->author << "\n";
                cout << "Year of Publication: " << result->yearOfPublication << "\n";
                cout << "--------------------------\n";
            } else {
                cout << "Book not found!\n";
            }
        } else if (choice == 4) {
            // Display books
            displayBooks(library);
        } else if (choice == 5) {
            // Sort books
            cout << "1. Sort by publishing year\n";
            cout << "2. Sort by author\n";
            cout << "3. Sort by title\n";
            cout << "Enter your choice for sorting: ";

            int sortChoice;
            cin >> sortChoice;

            bubbleSort(library, sortChoice);
            displayBooks(library);
            cout << "Books sorted successfully!\n";
        } else if (choice == 6) {
            // Exit the program
            deleteLibrary(library);
            break;
        } else {
            cout << "Invalid choice. Please enter a valid option.\n";
        }
    }

    return 0;
}

