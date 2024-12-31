#include <iostream>
#include <string>
#include <cctype> // For tolower function

using namespace std;

// Constants for user roles
const string ADMIN_USERNAME = "admin";
const string ADMIN_PASSWORD = "admin123";

struct Teacher {
    int id;
    string name;
    double totalRating;
    int ratingCount;
    int goodComments;
    int badComments;
    double finalScore; // New field for final score
};

struct Student {
    int id;
    string username;
    string password;
    int allowedTeachers[2]; // List of teachers assigned to the student (fixed size for simplicity)
    int ratedTeachers[2];   // To track which teachers a student has rated
    int ratedCount;         // Count of rated teachers
};

Teacher teachers[5] = {
    {1, "Mr. Smith", 0, 0, 0, 0, 0},
    {2, "Mrs. Johnson", 0, 0, 0, 0, 0},
    {3, "Ms. Brown", 0, 0, 0, 0, 0},
    {4, "Mr. Lee", 0, 0, 0, 0, 0},
    {5, "Mrs. Garcia", 0, 0, 0, 0, 0}
};

Student students[4] = {
    {1, "student1", "pass1", {1, 2}, {}, 0},
    {2, "student2", "pass2", {2, 3}, {}, 0},
    {3, "student3", "pass3", {1, 3}, {}, 0},
    {4, "student4", "pass4", {4, 5}, {}, 0}
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
    string goodKeywords[] = {"good", "excellent", "amazing", "great", "awesome"};
    string badKeywords[] = {"bad", "poor", "terrible", "awful", "horrible"};

    string lowerComment = toLowerCase(comment);

    for (const string &keyword : goodKeywords) {
        size_t pos = 0;
        while ((pos = lowerComment.find(keyword, pos)) != string::npos) {
            teachers[teacherId - 1].goodComments++;
            pos += keyword.length();
        }
    }
    for (const string &keyword : badKeywords) {
        size_t pos = 0;
        while ((pos = lowerComment.find(keyword, pos)) != string::npos) {
            teachers[teacherId - 1].badComments++;
            pos += keyword.length();
        }
    }
}

void calculateFinalScores() {
    for (int i = 0; i < 5; i++) {
        double avgRating = teachers[i].ratingCount > 0 ? teachers[i].totalRating / teachers[i].ratingCount : 0;
        teachers[i].finalScore = (0.8 * avgRating) + (0.2 * (teachers[i].goodComments - teachers[i].badComments));
    }
}

void sortTeachersByScore() {
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (teachers[i].finalScore < teachers[j].finalScore) {
                Teacher temp = teachers[i];
                teachers[i] = teachers[j];
                teachers[j] = temp;
            }
        }
    }
}

void displayRankings() {
    calculateFinalScores();
    sortTeachersByScore();

    cout << "\n--- Teacher Rankings ---\n";
    cout << "Rank | ID | Name           | Avg. Rating | Good | Bad | Final Score\n";
    cout << "-----+----+---------------+-------------+------+-----+-------------\n";

    for (int i = 0; i < 5; i++) {
        double avgRating = teachers[i].ratingCount > 0 ? teachers[i].totalRating / teachers[i].ratingCount : 0;
        cout << i + 1 << "    | " << teachers[i].id << "  | " << teachers[i].name;
        cout << string(15 - teachers[i].name.length(), ' ') << " | " << avgRating;
        cout << "         | " << teachers[i].goodComments << "    | " << teachers[i].badComments;
        cout << "   | " << teachers[i].finalScore << "\n";
    }

    cout << "\nPress Enter to return to the main menu...";
    cin.ignore();
    cin.get();
}

void rateTeacher(Student &student) {
    cout << "\n--- Available Teachers to Rate ---\n";
    for (int i = 0; i < 2; i++) {
        int teacherId = student.allowedTeachers[i];
        cout << teacherId << ". " << teachers[teacherId - 1].name << "\n";
    }

    int teacherId;
    cout << "Enter the Teacher ID to rate: ";
    cin >> teacherId;

    // Check if teacher is allowed for this student
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
    cout << "Enter your comment: ";
    getline(cin, comment);

    analyzeComment(comment, teacherId);

    teachers[teacherId - 1].totalRating += rating;
    teachers[teacherId - 1].ratingCount++;
    student.ratedTeachers[student.ratedCount++] = teacherId;

    cout << "Rating and comment submitted successfully! Press Enter to continue...";
    cin.get();
}

void adminPortal() {
    displayRankings();
}

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
