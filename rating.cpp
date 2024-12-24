#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib> // For system("clear")

using namespace std;

// Constants for user roles
const string ADMIN_USERNAME = "admin";
const string ADMIN_PASSWORD = "admin123";

struct Teacher {
    int id;
    string name;
    double totalRating;   // Sum of all ratings
    int ratingCount;      // Number of ratings
    int goodComments;
    int badComments;
};

struct Student {
    int id;
    string username;
    string password;
    vector<int> allowedTeachers; // List of teachers assigned to the student
    vector<int> ratedTeachers;  // To track which teachers a student has rated
};

// Initial state of Teachers
Teacher teachers[5] = {
    {1, "Mr. Smith", 0, 0, 0, 0},
    {2, "Mrs. Johnson", 0, 0, 0, 0},
    {3, "Ms. Brown", 0, 0, 0, 0},
    {4, "Mr. Lee", 0, 0, 0, 0},
    {5, "Mrs. Garcia", 0, 0, 0, 0}
};

// Initial state of Students with assigned teachers
Student students[4] = {
    {1, "student1", "pass1", {1, 2}, {}},  // student1 can rate Mr. Smith and Mrs. Johnson
    {2, "student2", "pass2", {2, 3}, {}},  // student2 can rate Mrs. Johnson and Ms. Brown
    {3, "student3", "pass3", {1, 3}, {}},  // student3 can rate Mr. Smith and Ms. Brown
    {4, "student4", "pass4", {4, 5}, {}}    // student4 can rate Mr. Lee and Mrs. Garcia
};

// Helper function to classify comments
string analyzeComment(const string &comment) {
    string goodKeywords[] = {"good", "excellent", "amazing", "great", "awesome"};
    string badKeywords[] = {"bad", "poor", "terrible", "awful", "horrible"};

    for (const auto &word : goodKeywords) {
        if (comment.find(word) != string::npos) {
            return "good";
        }
    }
    for (const auto &word : badKeywords) {
        if (comment.find(word) != string::npos) {
            return "bad";
        }
    }
    return "neutral";
}

// Function to display teacher rankings
void displayRankings() {
    system("clear");
    cout << "\n--- Teacher Rankings ---\n";
    cout << left << setw(15) << "Teacher Name" << setw(10) << "Rating" << setw(10) << "Good" << "Bad" << endl;
    for (int i = 0; i < 5; i++) {
        double averageRating = teachers[i].ratingCount > 0 ? teachers[i].totalRating / teachers[i].ratingCount : 0;
        cout << left << setw(15) << teachers[i].name
             << setw(10) << fixed << setprecision(2) << averageRating
             << setw(10) << teachers[i].goodComments
             << teachers[i].badComments << endl;
    }
    cout << "\nPress Enter to return to the previous menu...";
    cin.ignore();
    cin.get();
}

// Function for student to rate a teacher
void rateTeacher(Student &student) {
    system("clear");
    cout << "\n--- Available Teachers to Rate ---\n";

    // Display only the teachers assigned to this student
    for (int teacherId : student.allowedTeachers) {
        cout << teachers[teacherId - 1].id << ". " << teachers[teacherId - 1].name << endl;
    }

    int teacherId;
    cout << "Enter the Teacher ID to rate: ";
    cin >> teacherId;

    // Validate Teacher ID
    if (teacherId < 1 || teacherId > 5) {
        cout << "Invalid Teacher ID. Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    // Check if student has already rated this teacher
    for (int ratedTeacher : student.ratedTeachers) {
        if (ratedTeacher == teacherId) {
            cout << "You have already rated this teacher. Press Enter to continue...";
            cin.ignore();
            cin.get();
            return;
        }
    }

    // Rating and Comment section
    double rating;
    cout << "Enter rating (1-5): ";
    cin >> rating;

    if (rating < 1 || rating > 5) {
        cout << "Invalid rating. Please enter a value between 1 and 5. Press Enter to continue...";
        cin.ignore();
        cin.get();
        return;
    }

    cin.ignore();
    string comment;
    cout << "Enter comment (10-200 characters): ";
    getline(cin, comment);

    if (comment.length() < 10 || comment.length() > 200) {
        cout << "Invalid comment length. Press Enter to continue...";
        cin.get();
        return;
    }

    // Update Teacher's rating and comments
    teachers[teacherId - 1].totalRating += rating;
    teachers[teacherId - 1].ratingCount++;
    string sentiment = analyzeComment(comment);
    if (sentiment == "good") {
        teachers[teacherId - 1].goodComments++;
    } else if (sentiment == "bad") {
        teachers[teacherId - 1].badComments++;
    }

    student.ratedTeachers.push_back(teacherId);
    cout << "Rating submitted successfully! Press Enter to continue...";
    cin.get();
}

// Admin login function
void adminPortal() {
    system("clear");
    displayRankings();
}

// Student login function
void studentPortal(Student &student) {
    int choice;
    do {
        system("clear");
        cout << "\n--- Student Portal ---\n";
        cout << "1. Rate a Teacher\n";
        cout << "2. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                rateTeacher(student);
                break;
            case 2:
                cout << "Logging out... Press Enter to continue...";
                cin.ignore();
                cin.get();
                return; // Return to main menu
            default:
                cout << "Invalid choice. Try again. Press Enter to continue...";
                cin.ignore();
                cin.get();
        }
    } while (true);
}

// Function to reset the teacher and student data
void resetData() {
    // Reset Teachers' ratings and comments
    for (int i = 0; i < 5; i++) {
        teachers[i].totalRating = 0;
        teachers[i].ratingCount = 0;
        teachers[i].goodComments = 0;
        teachers[i].badComments = 0;
    }

    // Reset Students' ratedTeachers
    for (int i = 0; i < 4; i++) {
        students[i].ratedTeachers.clear();
    }
}

// Main function for authentication
int main() {
    resetData();  // Reset the data when the program starts

    while (true) {
        system("clear");
        string username, password;
        cout << "\n--- Teacher Rating System ---\n";
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
            adminPortal();  // Admin portal keeps the session active
        } else {
            bool loggedIn = false;
            for (Student &student : students) {
                if (username == student.username && password == student.password) {
                    studentPortal(student);  // Student portal keeps the session active
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
    return 0;
}
