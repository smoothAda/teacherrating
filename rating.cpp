#include <iostream>
#include <string>
#include <cctype> // For tolower function
#include <algorithm> // For sort function
#include <cstdlib> // For system function

using namespace std;

// Constants for user roles
const string ADMIN_USERNAME = "admin";
const string ADMIN_PASSWORD = "admin123";

// Function to clear the console screen
void clearScreen() {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls"); // Clear screen for Windows
    #else
        system("clear"); // Clear screen for Linux/Mac
    #endif
}

//used to store and manage information about a teacher
struct Teacher {
    int id; // Unique ID for the teacher
    string name; // Name of the teacher
    double totalRating; // Cumulative rating given by students
    int ratingCount; // Number of ratings received
    int goodComments; // Number of good comments received
    int badComments; // Number of bad comments received

    // Function to calculate and return the average rating
    double getAverageRating() const {
        return ratingCount > 0 ? totalRating / ratingCount : 0;
    }
};

// used to store and manage information about a student
struct Student {
    int id; // Unique ID for the student
    string username; // User-name for login
    string password; // Password for login
    int allowedTeachers[5]; // IDs of teachers the student is allowed to rate
    int ratedTeachers[5]; // IDs of teachers the student has already rated
    int ratedCount; // Count of rated teachers
};

// Array of teachers (7 teachers in total)
Teacher teachers[7] = {
    {1, "Mr. Smith", 0, 0, 0, 0},
    {2, "Mrs. Johnson", 0, 0, 0, 0},
    {3, "Ms. Brown", 0, 0, 0, 0},
    {4, "Mr. Lee", 0, 0, 0, 0},
    {5, "Mrs. Garcia", 0, 0, 0, 0},
    {6, "Dr. Wilson", 0, 0, 0, 0},
    {7, "Prof. Taylor", 0, 0, 0, 0}
};

// Array of students (14 students in total)
Student students[14] = {
    {1, "student1", "pass1", {1, 2, 3, 4, 5}, {}, 0},
    {2, "student2", "pass2", {2, 3, 4, 5, 6}, {}, 0},
    {3, "student3", "pass3", {1, 3, 5, 6, 7}, {}, 0},
    {4, "student4", "pass4", {4, 5, 6, 7, 1}, {}, 0},
    {5, "student5", "pass5", {1, 2, 3, 6, 7}, {}, 0},
    {6, "student6", "pass6", {2, 4, 5, 6, 7}, {}, 0},
    {7, "student7", "pass7", {3, 4, 5, 6, 7}, {}, 0},
    {8, "student8", "pass8", {1, 2, 6, 7, 4}, {}, 0},
    {9, "student9", "pass9", {3, 5, 6, 1, 7}, {}, 0},
    {10, "student10", "pass10", {4, 5, 6, 2, 3}, {}, 0},
    {11, "student11", "pass11", {1, 2, 3, 4, 7}, {}, 0},
    {12, "student12", "pass12", {2, 3, 4, 5, 6}, {}, 0},
    {13, "student13", "pass13", {1, 3, 4, 6, 7}, {}, 0},
    {14, "student14", "pass14", {2, 4, 5, 6, 7}, {}, 0}
};

// Helper function to convert a string to lowercase
string toLowerCase(const string &str) {
    string result;
    for (char ch : str) {
        result += tolower(ch);
    }
    return result;
}

// Function to analyze comments
void analyzeComment(const string &comment, int teacherId) {
    // Keywords for comment analysis
    string goodKeywords[] = {"good", "excellent", "amazing", "great", "awesome"};
    string badKeywords[] = {"bad", "poor", "terrible", "awful", "horrible"};
    string negations[] = {"not", "never", "no"};

    string lowerComment = toLowerCase(comment); // Convert comment to lowercase for case-insensitive analysis

    // Analyze good keywords
    for (const string &keyword : goodKeywords) {
        size_t pos = lowerComment.find(keyword);
        while (pos != string::npos) {
            bool negated = false;
            for (const string &neg : negations) {
                size_t negPos = lowerComment.rfind(neg, pos);
                if (negPos != string::npos && pos - negPos <= 5) {
                    negated = true;
                    break;
                }
            }
            if (negated) {
                teachers[teacherId - 1].badComments++;
            } else {
                teachers[teacherId - 1].goodComments++;
            }
            pos = lowerComment.find(keyword, pos + 1);
        }
    }

    // Analyze bad keywords
    for (const string &keyword : badKeywords) {
        size_t pos = lowerComment.find(keyword);
        while (pos != string::npos) {
            bool negated = false;
            for (const string &neg : negations) {
                size_t negPos = lowerComment.rfind(neg, pos);
                if (negPos != string::npos && pos - negPos <= 5) {
                    negated = true;
                    break;
                }
            }
            if (negated) {
                teachers[teacherId - 1].goodComments++;
            } else {
                teachers[teacherId - 1].badComments++;
            }
            pos = lowerComment.find(keyword, pos + 1);
        }
    }
}

// Function to display teacher rankings
void displayRankings() {
    clearScreen(); // Clear the console screen

    // Sort teachers by average rating in descending order
    sort(begin(teachers), end(teachers), [](const Teacher &a, const Teacher &b) {
        return a.getAverageRating() > b.getAverageRating();
    });

    // Display rankings table
    cout << "\n--- Teacher Rankings ---\n";
    cout << "Rank | ID | Name           | Avg. Rating | Good | Bad\n";
    cout << "-----+----+---------------+-------------+------+-----\n";
    for (int i = 0; i < 7; i++) {
        double averageRating = teachers[i].getAverageRating();
        cout << (i + 1) << "    | " << teachers[i].id << "  | " << teachers[i].name;
        cout << string(15 - teachers[i].name.length(), ' ') << " | " << averageRating;
        cout << "         | " << teachers[i].goodComments << "    | " << teachers[i].badComments << "\n";
    }
    cout << "\nPress Enter to return to the main menu...";
    cin.ignore();
    cin.get();
}

// Function to allow students to rate a teacher
void rateTeacher(Student &student) {
    clearScreen(); // Clear the console screen

    // Display available teachers
    cout << "\n--- Available Teachers to Rate ---\n";
    for (int i = 0; i < 5; i++) {
        int teacherId = student.allowedTeachers[i];
        cout << teacherId << ". " << teachers[teacherId - 1].name << "\n";
    }

    int teacherId;
    cout << "Enter the Teacher ID to rate: ";
    cin >> teacherId;

    // Check if teacher is allowed for this student
    bool isAllowed = false;
    for (int i = 0; i < 5; i++) {
        if (student.allowedTeachers[i] == teacherId) {
            isAllowed = true;
            break;
        }
    }

    if (!isAllowed) {
        cout << "You are not allowed to rate this teacher. Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    // Check if the teacher has already been rated
    bool alreadyRated = false;
    for (int i = 0; i < student.ratedCount; i++) {
        if (student.ratedTeachers[i] == teacherId) {
            alreadyRated = true;
            break;
        }
    }

    if (alreadyRated || teacherId < 1 || teacherId > 7) {
        cout << "Invalid or already rated. Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    // Get rating from the student
    double rating;
    cout << "Enter rating (1-5): ";
    cin >> rating;
    if (rating < 1 || rating > 5) {
        cout << "Invalid rating. Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    cin.ignore(); // Clear input buffer

    // Get comment from the student
    string comment;
    cout << "Enter your comment (10-100 characters): ";
    getline(cin, comment);

    if (comment.length() < 10 || comment.length() > 100) {
        cout << "Comment not submitted successfully. Rating has been recorded. Press Enter to continue...";
        teachers[teacherId - 1].totalRating += rating;
        teachers[teacherId - 1].ratingCount++;
        student.ratedTeachers[student.ratedCount++] = teacherId;
        cin.get();
        return;
    }

    // Analyze comment and record rating
    analyzeComment(comment, teacherId);
    teachers[teacherId - 1].totalRating += rating;
    teachers[teacherId - 1].ratingCount++;
    student.ratedTeachers[student.ratedCount++] = teacherId;

    cout << "Rating and comment submitted successfully! Press Enter to continue...";
    cin.get();
}

// Function for admin portal
void adminPortal() {
    displayRankings(); // Display teacher rankings
}

// Function for student portal
void studentPortal(Student &student) {
    int choice;
    do {
        clearScreen(); // Clear the console screen

        // Display options for the student
        cout << "\n--- Student Portal ---\n";
        cout << "1. Rate a Teacher\n";
        cout << "2. Return to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                rateTeacher(student); // Allow the student to rate a teacher
                break;
            case 2:
                return; // Exit the student portal
            default:
                cout << "Invalid choice. Try again.";
        }
    } while (true);
}

// Function for the main menu
void mainMenu() {
    string username, password;
    while (true) {
        clearScreen(); // Clear the console screen

        // Prompt for user credentials
        cout << "\n--- Teacher Rating System ---\n";
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
            adminPortal(); // Redirect to admin portal
        } else {
            bool loggedIn = false;

            // Check student credentials
            for (int i = 0; i < 14; i++) {
                if (username == students[i].username && password == students[i].password) {
                    studentPortal(students[i]); // Redirect to student portal
                    loggedIn = true;
                    break;
                }
            }
            if (!loggedIn) {
                cout << "Invalid credentials. Press Enter to try again...";
                cin.ignore();
                cin.get();
            }
        }
    }
}

// Entry point of the application
int main() {
    mainMenu(); // Start the application with the main menu
    return 0;
}
