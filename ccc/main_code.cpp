#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define STUD_FILE "students.txt"
#define CRE_FILE  "credentials.txt"

string currentUser;
string currentRole;

bool login() {
    string user, pass, role;
    string inUser, inPass;

    cout << "USERNAME: ";
    cin >> inUser;
    cout << "PASSWORD: ";
    cin >> inPass;

    ifstream fin(CRE_FILE);
    if (!fin) {
        cout << "Credential file missing!\n";
        return false;
    }
    while (fin >> user >> pass >> role) {
        if (inUser == user && inPass == pass) {
            currentUser = user;
            currentRole = role;
            return true;
        }
    }
    return false;
}

void addStudent() {
    int roll;
    string name;
    float mark;

    cout << "Roll: ";
    cin >> roll;
    cout << "Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Mark: ";
    cin >> mark;

    ofstream fout(STUD_FILE, ios::app);
    fout << roll << "|" << name << "|" << mark << "\n";
}

void displayStudents() {
    ifstream fin(STUD_FILE);
    if (!fin) {
        cout << "Student file missing!\n";
        return;
    }

    string line;
    cout << "Roll\tName\tMark\n";
    cout << "----\t----\t----\n";

    while (getline(fin, line)) {
        if (line == "") continue;

        size_t p1 = line.find('|');
        size_t p2 = line.rfind('|');

        int roll = stoi(line.substr(0, p1));
        string name = line.substr(p1 + 1, p2 - p1 - 1);
        float mark = stof(line.substr(p2 + 1));

        cout << roll << "\t" << name << "\t" << mark << "\n";
    }
}

void searchStudent() {
    int findRoll;
    cout << "Enter roll to search: ";
    cin >> findRoll;

    ifstream fin(STUD_FILE);
    string line;

    while (getline(fin, line)) {
        if (line == "") continue;

        size_t p1 = line.find('|');
        size_t p2 = line.rfind('|');

        int roll = stoi(line.substr(0, p1));
        string name = line.substr(p1 + 1, p2 - p1 - 1);
        float mark = stof(line.substr(p2 + 1));

        if (roll == findRoll) {
            cout << "Found: " << roll << " " << name << " " << mark << "\n";
            return;
        }
    }
    cout << "Student not found!\n";
}

void deleteStudent() {
    int delRoll;
    cout << "Enter roll to delete: ";
    cin >> delRoll;

    ifstream fin(STUD_FILE);
    ofstream temp("temp.txt");

    string line;
    bool found = false;

    while (getline(fin, line)) {
        size_t p1 = line.find('|');
        int roll = stoi(line.substr(0, p1));

        if (roll != delRoll)
            temp << line << "\n";
        else
            found = true;
    }

    fin.close();
    temp.close();

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) cout << "Student deleted!\n";
    else cout << "Roll not found!\n";
}

void updateStudent() {
    int updateRoll;
    cout << "Enter roll to update: ";
    cin >> updateRoll;

    ifstream fin(STUD_FILE);
    ofstream temp("temp.txt");

    string line;
    bool found = false;

    while (getline(fin, line)) {
        size_t p1 = line.find('|');
        size_t p2 = line.rfind('|');

        int roll = stoi(line.substr(0, p1));

        if (roll == updateRoll) {
            found = true;
            string newName;
            float newMark;

            cout << "New Name: ";
            cin.ignore();
            getline(cin, newName);
            cout << "New Mark: ";
            cin >> newMark;

            temp << roll << "|" << newName << "|" << newMark << "\n";
        } else {
            temp << line << "\n";
        }
    }

    fin.close();
    temp.close();

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) cout << "Student updated!\n";
    else cout << "Roll not found!\n";
}

void adminMenu() {
    int c;
    while (true) {
        cout << "\nADMIN MENU\n1.Add\n2.Display\n3.Search\n4.Update\n5.Delete\n6.Logout\n";
        cin >> c;
        if (c == 1) addStudent();
        else if (c == 2) displayStudents();
        else if (c == 3) searchStudent();
        else if (c == 4) updateStudent();
        else if (c == 5) deleteStudent();
        else return;
    }
}

void staffMenu() {
    int c;
    while (true) {
        cout << "\nSTAFF MENU\n1.Add\n2.Display\n3.Search\n4.Update\n5.Logout\n";
        cin >> c;
        if (c == 1) addStudent();
        else if (c == 2) displayStudents();
        else if (c == 3) searchStudent();
        else if (c == 4) updateStudent();
        else return;
    }
}

void guestMenu() {
    int c;
    while (true) {
        cout << "\nGUEST MENU\n1.Display\n2.Search\n3.Logout\n";
        cin >> c;
        if (c == 1) displayStudents();
        else if (c == 2) searchStudent();
        else return;
    }
}

int main() {
    if (!login()) {
        cout << "Invalid login!\n";
        return 0;
    }

    cout << "Logged in as: " << currentRole << endl;

    if (currentRole == "admin") adminMenu();
    else if (currentRole == "staff") staffMenu();
    else guestMenu();

    return 0;
}
