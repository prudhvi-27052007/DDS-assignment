#include <iostream>
#include <string>
#include <stack>
using namespace std;

// ------------------------------
// Book Node
// ------------------------------
class Book {
public:
    string title;
    string author;
    bool available;
    Book* next;

    Book(string t, string a) {
        title = t;
        author = a;
        available = true;
        next = nullptr;
    }
};

// ------------------------------
// Inventory (Linked List)
// ------------------------------
class Inventory {
private:
    Book* head;

public:
    Inventory() {
        head = nullptr;
    }

    void addBook(string title, string author) {
        Book* newBook = new Book(title, author);
        if (!head) {
            head = newBook;
        } else {
            Book* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newBook;
        }
        cout << "📚 Added: " << title << " by " << author << endl;
    }

    void displayBooks() {
        if (!head) {
            cout << "⚠️ No books in the library." << endl;
            return;
        }
        cout << "\n--- 📖 Library Inventory ---" << endl;
        Book* temp = head;
        while (temp) {
            string status = temp->available ? "✅ Available" : "❌ Borrowed";
            cout << temp->title << " by " << temp->author << " - " << status << endl;
            temp = temp->next;
        }
    }

    Book* findBook(string title) {
        Book* temp = head;
        while (temp) {
            string lowTitle = temp->title, inputTitle = title;
            for (auto &c : lowTitle) c = tolower(c);
            for (auto &c : inputTitle) c = tolower(c);

            if (lowTitle == inputTitle) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    void search(string keyword) {
        cout << "\n🔍 Search results for '" << keyword << "':" << endl;
        Book* temp = head;
        bool found = false;
        while (temp) {
            string lowTitle = temp->title, lowAuthor = temp->author, key = keyword;
            for (auto &c : lowTitle) c = tolower(c);
            for (auto &c : lowAuthor) c = tolower(c);
            for (auto &c : key) c = tolower(c);

            if (lowTitle.find(key) != string::npos || lowAuthor.find(key) != string::npos) {
                string status = temp->available ? "✅ Available" : "❌ Borrowed";
                cout << temp->title << " by " << temp->author << " - " << status << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found) cout << "⚠️ No matches found." << endl;
    }
};

// ------------------------------
// Action Record
// ------------------------------
class Action {
public:
    string actionType; // "borrow" or "return"
    Book* book;

    Action(string type, Book* b) {
        actionType = type;
        book = b;
    }
};

// ------------------------------
// E-Library
// ------------------------------
class ELibrary {
private:
    Inventory inventory;
    stack<Action> undoStack;

public:
    ELibrary() {
        // preload some books
        inventory.addBook("The Alchemist", "Paulo Coelho");
        inventory.addBook("1984", "George Orwell");
        inventory.addBook("Python Programming", "John Zelle");
    }

    void borrowBook(string title) {
        Book* book = inventory.findBook(title);
        if (!book) {
            cout << "⚠️ Book not found." << endl;
            return;
        }
        if (!book->available) {
            cout << "❌ Book is already borrowed." << endl;
            return;
        }
        book->available = false;
        undoStack.push(Action("borrow", book));
        cout << "📕 Borrowed: " << book->title << endl;
    }

    void returnBook(string title) {
        Book* book = inventory.findBook(title);
        if (!book) {
            cout << "⚠️ Book not found." << endl;
            return;
        }
        if (book->available) {
            cout << "⚠️ Book was not borrowed." << endl;
            return;
        }
        book->available = true;
        undoStack.push(Action("return", book));
        cout << "📗 Returned: " << book->title << endl;
    }

    void undo() {
        if (undoStack.empty()) {
            cout << "⚠️ Nothing to undo." << endl;
            return;
        }
        Action action = undoStack.top();
        undoStack.pop();

        if (action.actionType == "borrow") {
            action.book->available = true;
            cout << "↩️ Undo: Borrow undone for '" << action.book->title << "'" << endl;
        } else if (action.actionType == "return") {
            action.book->available = false;
            cout << "↩️ Undo: Return undone for '" << action.book->title << "'" << endl;
        }
    }

    Inventory& getInventory() {
        return inventory;
    }
};

// ------------------------------
// Main Console Program
// ------------------------------
int main() {
    ELibrary library;
    string choice;

    while (true) {
        cout << "\n=== 📚 E-Library Book Management ===" << endl;
        cout << "1) Display all books" << endl;
        cout << "2) Borrow a book" << endl;
        cout << "3) Return a book" << endl;
        cout << "4) Undo last action" << endl;
        cout << "5) Search book by title/author" << endl;
        cout << "0) Exit" << endl;

        cout << "Choose option: ";
        getline(cin, choice);

        if (choice == "0") {
            cout << "👋 Exiting E-Library. Goodbye!" << endl;
            break;
        } else if (choice == "1") {
            library.getInventory().displayBooks();
        } else if (choice == "2") {
            string title;
            cout << "Enter book title to borrow: ";
            getline(cin, title);
            library.borrowBook(title);
        } else if (choice == "3") {
            string title;
            cout << "Enter book title to return: ";
            getline(cin, title);
            library.returnBook(title);
        } else if (choice == "4") {
            library.undo();
        } else if (choice == "5") {
            string keyword;
            cout << "Enter keyword (title/author): ";
            getline(cin, keyword);
            library.getInventory().search(keyword);
        } else {
            cout << "⚠️ Invalid choice. Try again." << endl;
        }
    }
    return 0;
}
