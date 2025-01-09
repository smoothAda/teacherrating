#include <iostream>
#include <string>
#include <cctype> // For tolower function
#include <algorithm> // For sort function
#include <cstdlib> // For system function

using namespace std;

// Constants for user roles (does not include student roles)
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

// Structure for Teacher (does not track subjects or individual student ratings)
struct Teacher {
    int id;
    string name;
    double totalRating;
    int ratingCount;
    int goodComments;
    int badComments;

    // Calculates and returns the average rating of the teacher
    double getAverageRating() const {
        return ratingCount > 0 ? totalRating / ratingCount : 0;
    }
};

// Structure for Student (increased allowedTeachers size to 5)
struct Student {
    int id;
    string username;
    string password;
    int allowedTeachers[5]; // List of teachers assigned to the student
    int ratedTeachers[5];   // To track which teachers a student has rated
    int ratedCount;         // Count of rated teachers
};

Teacher teachers[5] = {
    {1, "Mr. Smith", 0, 0, 0, 0},
    {2, "Mrs. Johnson", 0, 0, 0, 0},
    {3, "Ms. Brown", 0, 0, 0, 0},
    {4, "Mr. Lee", 0, 0, 0, 0},
    {5, "Mrs. Garcia", 0, 0, 0, 0}
};

Student students[14] = {
    {1, "student1", "pass1", {1, 2, 3, 4, 5}, {}, 0},
    {2, "student2", "pass2", {1, 2, 3, 4, 5}, {}, 0},
    {3, "student3", "pass3", {1, 2, 3, 4, 5}, {}, 0},
    {4, "student4", "pass4", {1, 2, 3, 4, 5}, {}, 0},
    {5, "student5", "pass5", {1, 2, 3, 4, 5}, {}, 0},
    {6, "student6", "pass6", {1, 2, 3, 4, 5}, {}, 0},
    {7, "student7", "pass7", {1, 2, 3, 4, 5}, {}, 0},
    {8, "student8", "pass8", {1, 2, 3, 4, 5}, {}, 0},
    {9, "student9", "pass9", {1, 2, 3, 4, 5}, {}, 0},
    {10, "student10", "pass10", {1, 2, 3, 4, 5}, {}, 0},
    {11, "student11", "pass11", {1, 2, 3, 4, 5}, {}, 0},
    {12, "student12", "pass12", {1, 2, 3, 4, 5}, {}, 0},
    {13, "student13", "pass13", {1, 2, 3, 4, 5}, {}, 0},
    {14, "student14", "pass14", {1, 2, 3, 4, 5}, {}, 0}
};

// Helper function to convert a string to lowercase (does not handle non-ASCII characters)
// Converts all characters in the given string to lowercase and returns the result.
string toLowerCase(const string &str) {
    string result;
    for (char ch : str) {
        result += tolower(ch);
    }
    return result;
}

// Function to analyze comments (enhanced with negation handling)
// Processes the comment to determine the number of good and bad comments for a teacher.
void analyzeComment(const string &comment, int teacherId) {
    string goodKeywords[] = {"good", "excellent", "amazing", "great", "awesome"};
    string badKeywords[] = {"bad", "poor", "terrible", "awful", "horrible"};
    string negations[] = {"not", "never", "no"};

    string lowerComment = toLowerCase(comment);

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

// Displays teacher rankings sorted by average rating
// Sorts the teachers by average rating in descending order and displays their rankings.
void displayRankings() {
    clearScreen();
    // Sort teachers by average rating in descending order
    sort(begin(teachers), end(teachers), [](const Teacher &a, const Teacher &b) {
        return a.getAverageRating() > b.getAverageRating();
    });

    cout << "\n--- Teacher Rankings ---\n";
    cout << "Rank | ID | Name           | Avg. Rating | Good | Bad\n";
    cout << "-----+----+---------------+-------------+------+-----\n";
    for (int i = 0; i < 5; i++) {
        double averageRating = teachers[i].getAverageRating();
        cout << (i + 1) << "    | " << teachers[i].id << "  | " << teachers[i].name;
        cout << string(15 - teachers[i].name.length(), ' ') << " | " << averageRating;
        cout << "         | " << teachers[i].goodComments << "    | " << teachers[i].badComments << "\n";
    }
    cout << "\nPress Enter to return to the main menu...";
    cin.ignore();
    cin.get();
}

// Allows a student to rate a teacher
// Facilitates the process of students rating a teacher and optionally leaving a comment.
void rateTeacher(Student &student) {
    clearScreen();
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

    if (alreadyRated || teacherId < 1 || teacherId > 5) {
        cout << "Invalid or already rated. Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    double rating;
    cout << "Enter rating (1-5): ";
    cin >> rating;
    if (rating < 1 || rating > 5) {
        cout << "Invalid rating. Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    cin.ignore();
    string comment;
    cout << "Enter your comment (10-100 characters): ";
    getline(cin, comment);

    // Validate comment length
    if (comment.length() < 10 || comment.length() > 100) {
        cout << "Comment not submitted successfully. Rating has been recorded. Press Enter to continue...";
        teachers[teacherId - 1].totalRating += rating;
        teachers[teacherId - 1].ratingCount++;
        student.ratedTeachers[student.ratedCount++] = teacherId;
        cin.get();
        return;
    }

    analyzeComment(comment, teacherId);

    teachers[teacherId - 1].totalRating += rating;
    teachers[teacherId - 1].ratingCount++;
    student.ratedTeachers[student.ratedCount++] = teacherId;

    cout << "Rating and comment submitted successfully! Press Enter to continue...";
    cin.get();
}

// Admin portal
// Allows the admin to view the rankings of all teachers.
void adminPortal() {
    displayRankings();
}

// Provides options for students
// Allows students to either rate a teacher or return to the main menu.
void studentPortal(Student &student) {
    int choice;
    do {
        clearScreen();
        cout << "\n--- Student Portal ---\n";
        cout << "1. Rate a Teacher\n";
        cout << "2. Return to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                rateTeacher(student);
                break;
            case 2:
                return;
            default:
                cout << "Invalid choice. Try again.";
        }
    } while (true);
}

// Main menu
// Handles user authentication and provides access to either the admin or student portal.
void mainMenu() {
    string username, password;
    while (true) {
        clearScreen();
        cout << "\n--- Teacher Rating System ---\n";
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
            adminPortal();
        } else {
            bool loggedIn = false;
            for (int i = 0; i < 14; i++) {
                if (username == students[i].username && password == students[i].password) {
                    studentPortal(students[i]);
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
// Starts the main menu loop.
int main() {
    mainMenu();
    return 0;
}
