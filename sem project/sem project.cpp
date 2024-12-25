#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

// ANSI escape codes for styling
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"
#define BG_BLUE "\033[44m"
#define BG_GREEN "\033[42m"
#define BG_MAGENTA "\033[45m"

// Node for Doubly Linked List
struct Node {
    char data;
    Node* prev;
    Node* next;
    Node(char c) : data(c), prev(nullptr), next(nullptr) {}
};

class VersaTextEditor {
private:
    Node* head;
    Node* tail;
    stack<string> undoStack;
    stack<string> redoStack;

    string linkedListToString() {
        string result;
        Node* temp = head;
        while (temp) {
            result += temp->data;
            temp = temp->next;
        }
        return result;
    }
   



public:
    VersaTextEditor() : head(nullptr), tail(nullptr) {}

    // Remove const here
    void displayBanner(const string& title) {
        cout << BG_GREEN << "******************************************" << RESET << "\n";
        cout << BG_GREEN << "* " << title << string(40 - title.size(), ' ') << "*" << RESET << "\n";
        cout << BG_GREEN << "******************************************" << RESET << "\n";
    }

    void addText(const string& text) {
        undoStack.push(linkedListToString());
        for (char c : text) {
            Node* newNode = new Node(c);
            if (!head) {
                head = tail = newNode;
            }
            else {
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            }
        }
        while (!redoStack.empty()) redoStack.pop();
        cout << GREEN << "Text added successfully." << RESET << "\n";
    }

    void deleteText(int count) {
        if (count <= 0) {
            cout << RED << "Invalid delete operation." << RESET << "\n";
            return;
        }
        undoStack.push(linkedListToString());
        while (count-- && tail) {
            Node* temp = tail;
            tail = tail->prev;
            if (tail) tail->next = nullptr;
            else head = nullptr;
            delete temp;
        }
        while (!redoStack.empty()) redoStack.pop();
        cout << GREEN << "Characters deleted successfully." << RESET << "\n";
    }

    void undo() {
        if (undoStack.empty()) {
            cout << RED << "Nothing to undo." << RESET << "\n";
            return;
        }
        redoStack.push(linkedListToString());
        string prevText = undoStack.top();
        undoStack.pop();
        clearText();
        addText(prevText);
        cout << CYAN << "Undo operation performed." << RESET << "\n";
    }

    void redo() {
        if (redoStack.empty()) {
            cout << RED << "Nothing to redo." << RESET << "\n";
            return;
        }
        undoStack.push(linkedListToString());
        string nextText = redoStack.top();
        redoStack.pop();
        clearText();
        addText(nextText);
        cout << CYAN << "Redo operation performed." << RESET << "\n";
    }

    void displayText() const {
        cout << BG_MAGENTA << "Current Text:" << RESET << "\n";
        Node* temp = head;
        while (temp) {
            cout << BOLD << temp->data << RESET;
            temp = temp->next;
        }
        cout << "\n";
    }

    void searchText(const string& pattern) {
        string text = linkedListToString();
        size_t pos = text.find(pattern);
        if (pos != string::npos) {
            cout << GREEN << "Pattern found at position: " << pos + 1 << RESET << "\n";
        }
        else {
            cout << RED << "Pattern not found." << RESET << "\n";
        }
    }

    void clearText() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }
};

void printMenu() {
    cout << BG_BLUE << "=============== MENU ================" << RESET << "\n";
    cout << YELLOW << "1. Add Text\n"
        << "2. Delete Text\n"
        << "3. Undo\n"
        << "4. Redo\n"
        << "5. Display Current Text\n"
        << "6. Search Text\n"
        << "7. Clear Text\n"
        << "8. Exit\n" << RESET;
    cout << BOLD << "Enter your choice: " << RESET;
}

int main() {
    VersaTextEditor editor;
    int choice;

    do {
        editor.displayBanner("VERSA TEXT EDITOR");
        printMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: {
            string text;
            cout << BOLD << "Enter text to add: " << RESET;
            getline(cin, text);
            editor.addText(text);
            break;
        }
        case 2: {
            int count;
            cout << BOLD << "Enter number of characters to delete: " << RESET;
            cin >> count;
            editor.deleteText(count);
            break;
        }
        case 3:
            editor.undo();
            break;
        case 4:
            editor.redo();
            break;
        case 5:
            editor.displayText();
            break;
        case 6: {
            string pattern;
            cout << BOLD << "Enter text to search: " << RESET;
            cin >> pattern;
            editor.searchText(pattern);
            break;
        }
        case 7:
            editor.clearText();
            cout << YELLOW << "All text cleared." << RESET << "\n";
            break;
        case 8:
            cout << GREEN << "Exiting editor. Goodbye!" << RESET << "\n";
            break;
        default:
            cout << RED << "Invalid choice. Please try again." << RESET << "\n";
        }
    } while (choice != 8);

    return 0;
}
