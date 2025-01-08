#include <iostream>
#include <string>
#include <cctype> // For tolower function
#include <algorithm> // For sort function

using namespace std;

// Constants for user roles (does not include student roles)
const string ADMIN_USERNAME = "admin";
const string ADMIN_PASSWORD = "admin123";

// Structure for Teacher (does not track subjects or individual student ratings)
struct Teacher {
    int id;
    string name;
    double totalRating;
    int ratingCount;
    int goodComments;
    int badComments;

    double getAverageRating() const {
        return ratingCount > 0 ? totalRating / ratingCount : 0;
    }
};

// Structure for Student (does not include advanced features like dynamic teacher lists)
struct Student {
    int id;
    string username;
    string password;
    int allowedTeachers[2]; // List of teachers assigned to the student (fixed size for simplicity)
    int ratedTeachers[2];   // To track which teachers a student has rated
    int ratedCount;         // Count of rated teachers
};

Teacher teachers[5] = {
    {1, "Mr. Smith", 0, 0, 0, 0},
    {2, "Mrs. Johnson", 0, 0, 0, 0},
    {3, "Ms. Brown", 0, 0, 0, 0},
    {4, "Mr. Lee", 0, 0, 0, 0},
    {5, "Mrs. Garcia", 0, 0, 0, 0}
};

Student students[4] = {
    {1, "student1", "pass1", {1, 2}, {}, 0},
    {2, "student2", "pass2", {2, 3}, {}, 0},
    {3, "student3", "pass3", {1, 3}, {}, 0},
    {4, "student4", "pass4", {4, 5}, {}, 0}
};

// Helper function to convert a string to lowercase (does not handle non-ASCII characters)
string toLowerCase(const string &str) {
    string result;
    for (char ch : str) {
        result += tolower(ch);
    }
    return result;
}

// Function to analyze comments (enhanced with negation handling)
void analyzeComment(const string &comment, int teacherId) {
    string goodKeywords[] = {"good", "excellent", "amazing", "great", "awesome"};
    string badKeywords[] = {"bad", "poor", "terrible", "awful", "horrible"};
    string negations[] = {"not", "never", "no"};

    string lowerComment = toLowerCase(comment);

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
void displayRankings() {
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

// Allows a student to rate a teacher (does not allow feedback for improvements or suggestions)
void rateTeacher(Student &student) {
    cout << "\n--- Available Teachers to Rate ---\n";
    for (int i = 0; i < 2; i++) {
        int teacherId = student.allowedTeachers[i];
        cout << teacherId << ". " << teachers[teacherId - 1].name << "\n";
    }

    int teacherId;
    cout << "Enter the Teacher ID to rate: ";
    cin >> teacherId;

    // Check if teacher is allowed for this student (does not check for invalid inputs explicitly)
    bool isAllowed = false;
    for (int i = 0; i < 2; i++) {
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

// Admin portal (does not allow adding or removing teachers)
void adminPortal() {
    displayRankings();
}

// Provides options for students (does not provide teacher feedback or overall ratings)
void studentPortal(Student &student) {
    int choice;
    do {
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

// Main menu (does not include a forgot password feature or logout option)
void mainMenu() {
    string username, password;
    while (true) {
        cout << "\n--- Teacher Rating System ---\n";
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
            adminPortal();
        } else {
            bool loggedIn = false;
            for (int i = 0; i < 4; i++) {
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

int main() {
    mainMenu();
    return 0;
}
