/***************************************************************
 * Name:        Spencer Belknap
 * Course:      CS-300: Data Structures and Algorithms
 * Project:     Project Two – Advising Assistance Program
 * Date:        October 15, 2025

 ***************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <thread>
#include <chrono>
using namespace std;

// Struct for course info
struct Course {
    string id;
    string title;
    vector<string> prerequisites;
};

// Binary Search Tree for courses
class CourseBST {
private:
    struct Node {
        Course data;
        Node* left;
        Node* right;
        Node(const Course& c) : data(c), left(nullptr), right(nullptr) {}
    };
    Node* root = nullptr;
    int count = 0;

    void insert(Node*& node, const Course& c) {
        if (!node) { node = new Node(c); count++; }
        else if (c.id < node->data.id) insert(node->left, c);
        else insert(node->right, c);
    }
    void inOrder(Node* node) const {
        if (!node) return;
        inOrder(node->left);
        cout << node->data.id << ", " << node->data.title << endl;
        inOrder(node->right);
    }
    Course* search(Node* node, const string& id) const {
        if (!node) return nullptr;
        if (node->data.id == id) return &node->data;
        if (id < node->data.id) return search(node->left, id);
        return search(node->right, id);
    }
    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
public:
    ~CourseBST() { destroy(root); }
    void insert(const Course& c) { insert(root, c); }
    void printInOrder() const { inOrder(root); }
    Course* find(const string& id) const { return search(root, id); }
    int size() const { return count; }
};

// Split CSV line
vector<string> tokenize(const string& line, char delim = ',') {
    vector<string> tokens; string token; stringstream ss(line);
    while (getline(ss, token, delim)) if (!token.empty()) tokens.push_back(token);
    return tokens;
}

// Load courses from file
void loadFromFile(const string& filename, CourseBST& bst) {
    ifstream file(filename);
    if (!file.is_open()) { cerr << "Error: could not open " << filename << endl; return; }
    string line;
    while (getline(file, line)) {
        vector<string> parts = tokenize(line);
        if (parts.size() < 2) continue;
        Course c; c.id = parts[0]; c.title = parts[1];
        for (size_t i = 2; i < parts.size(); ++i) c.prerequisites.push_back(parts[i]);
        bst.insert(c);
    }
}

// Display a course
void showCourse(const Course& c) {
    cout << c.id << ", " << c.title << endl;
    cout << "Prerequisites: ";
    if (c.prerequisites.empty()) cout << "None";
    else for (size_t i = 0; i < c.prerequisites.size(); ++i) {
        cout << c.prerequisites[i];
        if (i < c.prerequisites.size() - 1) cout << ", ";
    }
    cout << endl;
}

// Main menu
int main() {
    CourseBST bst;
    string filename = "ProjectTwoInput.csv";
    int choice = 0;

    while (choice != 9) {
        cout << "\nMenu:\n"
             << "  1. Load Courses\n"
             << "  2. Display All Courses\n"
             << "  3. Find Course\n"
             << "  9. Exit\n"
             << "Enter choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n"; continue;
        }
        switch (choice) {
        case 1:
            loadFromFile(filename, bst);
            cout << bst.size() << " courses loaded.\n";
            break;
        case 2:
            bst.printInOrder();
            break;
        case 3: {
            cout << "Enter course ID: ";
            string id; cin >> id;
            for (auto& ch : id) ch = toupper(ch);
            Course* c = bst.find(id);
            if (c) showCourse(*c);
            else cout << "Course not found.\n";
            break;
        }
        case 9:
            cout << "Goodbye!\n"; break;
        default:
            cout << "Invalid option.\n";
        }
        this_thread::sleep_for(chrono::milliseconds(300));
    }
    return 0;
}
