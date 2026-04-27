//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Tierra Needum
// Description : ABCU Advising Assistance Program
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>

using namespace std;

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(const Course& aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, const Course& course) {
        if (course.courseNumber < node->course.courseNumber) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            }
            else {
                addNode(node->left, course);
            }
        }
        else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            }
            else {
                addNode(node->right, course);
            }
        }
    }

    void inOrder(Node* node) const {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
            inOrder(node->right);
        }
    }

    void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        destroyTree(root);
    }

    void Clear() {
        destroyTree(root);
        root = nullptr;
    }

    void Insert(const Course& course) {
        if (root == nullptr) {
            root = new Node(course);
        }
        else {
            addNode(root, course);
        }
    }

    Course Search(const string& courseNumber) const {
        Node* current = root;

        while (current != nullptr) {
            if (current->course.courseNumber == courseNumber) {
                return current->course;
            }
            else if (courseNumber < current->course.courseNumber) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        return Course();
    }

    void PrintCourseList() const {
        inOrder(root);
    }

    bool IsEmpty() const {
        return root == nullptr;
    }
};

string trim(const string& text) {
    size_t start = text.find_first_not_of(" \t\r\n");
    size_t end = text.find_last_not_of(" \t\r\n");

    if (start == string::npos) {
        return "";
    }

    return text.substr(start, end - start + 1);
}

string toUpperCase(string text) {
    for (size_t i = 0; i < text.length(); ++i) {
        text[i] = static_cast<char>(toupper(static_cast<unsigned char>(text[i])));
    }
    return text;
}

vector<string> splitLine(const string& line) {
    vector<string> tokens;
    string token;
    stringstream stream(line);

    while (getline(stream, token, ',')) {
        tokens.push_back(trim(token));
    }

    return tokens;
}

bool loadCoursesFromFile(const string& fileName, BinarySearchTree& courseTree) {
    ifstream inputFile(fileName);
    string line;

    if (!inputFile.is_open()) {
        cout << "Error: Could not open file.\n";
        return false;
    }

    while (getline(inputFile, line)) {
        line = trim(line);
        if (line.empty()) {
            continue;
        }

        vector<string> fields = splitLine(line);
        if (fields.size() < 2) {
            continue;
        }

        Course course;
        course.courseNumber = toUpperCase(fields[0]);
        course.courseTitle = fields[1];

        for (size_t i = 2; i < fields.size(); ++i) {
            if (!fields[i].empty()) {
                course.prerequisites.push_back(toUpperCase(fields[i]));
            }
        }

        courseTree.Insert(course);
    }

    inputFile.close();
    return true;
}

void printCourseInformation(const Course& course) {
    if (course.courseNumber.empty()) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course.courseNumber << ", " << course.courseTitle << endl;
    cout << "Prerequisites: ";

    if (course.prerequisites.empty()) {
        cout << "None" << endl;
    }
    else {
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

void printMenu() {
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << "What would you like to do? ";
}

int main() {
    BinarySearchTree courseTree;
    string fileName;
    string courseNumberInput;
    int choice = 0;
    bool dataLoaded = false;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        printMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            cout << "Enter the file name: ";
            getline(cin, fileName);
            fileName = trim(fileName);

            courseTree.Clear();
            if (loadCoursesFromFile(fileName, courseTree)) {
                dataLoaded = true;
            }
            else {
                dataLoaded = false;
            }
            break;

        case 2:
            if (!dataLoaded || courseTree.IsEmpty()) {
                cout << "Please load the data file first." << endl;
            }
            else {
                cout << "Here is a sample schedule:" << endl;
                courseTree.PrintCourseList();
            }
            break;

        case 3:
            if (!dataLoaded || courseTree.IsEmpty()) {
                cout << "Please load the data file first." << endl;
            }
            else {
                cout << "What course do you want to know about? ";
                getline(cin, courseNumberInput);
                courseNumberInput = toUpperCase(trim(courseNumberInput));
                printCourseInformation(courseTree.Search(courseNumberInput));
            }
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}
