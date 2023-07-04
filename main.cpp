#include <stdlib.h>
#include <iostream>
#include<conio.h>



#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
using namespace std;


const string server = "tcp://127.0.0.1:3306";
const string username = "root";
const string password = "yashgarg";

sql::Driver* driver;
sql::Connection* con;
sql::Statement* stmt;
sql::PreparedStatement* pstmt;
sql::PreparedStatement* pstmt1;
sql::PreparedStatement* pstmt2;
sql::ResultSet* res;
sql::ResultSet* res1;
sql::ResultSet* res2;
string bufferStr;

void showStudents() {
    pstmt = con->prepareStatement("SELECT * FROM student");
    res = pstmt->executeQuery();
    while (res->next()) {
        cout << res->getInt("Stud_ID") << " " << res->getString("Stud_Name") << " " << res->getString("Course") << " " << res->getString("Stud_No") << endl;
    }
    delete pstmt;
}

void showBooks() {
    pstmt = con->prepareStatement("SELECT * FROM books");
    res = pstmt->executeQuery();
    while (res->next()) {
        cout << res->getInt("Book_ID") << " " << res->getString("Book_Name") << " by " << res->getString("Book_Author")  << endl;
    }
    delete pstmt;
}

void showIssues(int studId) {
    if (studId != -1) {
        pstmt = con->prepareStatement("SELECT * FROM issues where issued_to_id = ?;");
        pstmt->setInt(1, studId);
        res = pstmt->executeQuery();
    }
    else {
        pstmt = con->prepareStatement("SELECT * FROM issues");
        res = pstmt->executeQuery();
    }
    while (res->next()) {
        cout << endl<<endl<<res->getInt("Issue_ID") << " on " << res->getString("Issued_On")<<" ";
        pstmt1 = con->prepareStatement("Select * from student where Stud_ID = ?");
        pstmt1->setInt(1, res->getInt("issued_to_id"));
        res1 = pstmt1->executeQuery();
        while (res1->next()) {
            cout << res1->getString("Stud_Name")<<" ";
        }
        delete pstmt1;

        pstmt2 = con->prepareStatement("Select * from books where Book_ID = ?");
        pstmt2->setInt(1, res->getInt("book_issued_id"));
        res2 = pstmt2->executeQuery();
        while (res2->next()) {
            cout << res2->getString("Book_Name") << "\n\n";
        }
        delete pstmt2;

    }
    delete pstmt;
}


int main()
{
    




    //MAKING CONNECTION
    try
    {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }

    con->setSchema("project");


    stmt = con->createStatement();

    bool adminBool = false;
    bool libBool = false;
    bool studBool = false;
    bool isNPresent = false;



    //LOGIN PORTAL  3--ADMIN 4--LIBRARIAN 5--STUDENT

    while (true) {
    cout << "Logged into Library..." << endl << endl;
            int x;
        try {
            
            system("cls");
            cout << "You are ?\n1.Admin\n2.Librarian\n3.Student" << endl;
            cin >> x;
            cin.ignore(1000, '\n');
            while (x == 1 || x == 2) {
                try {
                    cout << "Enter your details:\nID - ";

                    string username, password;
                    getline(cin, username);
                    cout << "Password - ";
                    getline(cin, password);
                    isNPresent = false;

                    //string q = "SELECT * FROM stakeholders WHERE Emp_ID = " + username + " AND Emp_Pass = " ? +password;

                    pstmt = con->prepareStatement("SELECT * FROM stakeholders WHERE Emp_ID = ? AND Emp_Pass = ?");
                    pstmt->setString(1, username);
                    pstmt->setString(2, password);
                    res = pstmt->executeQuery();
                    int ans = 0;
                    while (res->next()) {
                        ans++;
                    }
                    if (ans != 0) {
                        system("cls");
                        cout << "Logged in as " << username << endl;
                        if (x == 1) {
                            x = 4; adminBool = true;
                        }
                        else {
                            x = 5; libBool = true;
                        }
                    }
                    else {
                        cout << "Wrong credentials." << endl;
                        x = 10;
                        system("cls");
                    }
                }
                catch (exception e) {
                    cout << e.what() << endl;
                    cout << "\n\nEnter 0 and press enter to continue\n";
                    int x; cin >> x;
                    cin.ignore(1000, '\n');
                }

            }
            while (x == 3) {
                try {
                    cout << "Enter your details:\nID - ";
                    int id;
                    string name;
                    if (isNPresent) getline(cin, bufferStr);
                    cin >> id;
                    cin.ignore(1000, '\n');


                    cout << "Name - ";
                    getline(cin, name);
                    isNPresent = false;
                    //string q = "SELECT * FROM stakeholders WHERE Emp_ID = " + username + " AND Emp_Pass = " ? +password;

                    pstmt = con->prepareStatement("SELECT * FROM student WHERE Stud_ID = ? AND Stud_Name = ?");
                    pstmt->setInt(1, id);
                    pstmt->setString(2, name);

                    res = pstmt->executeQuery();
                    int ans = 0;
                    while (res->next()) {
                        ans++;
                    }
                    if (ans != 0) {
                        system("cls");
                        cout << "Logged in as " << id << " " << name << endl;
                        x = 6;
                    }
                    else {
                        cout << "Wrong credentials." << endl;
                        x = 10;
                    }
                }
                catch (exception e) {
                    cout << e.what();
                    if (isNPresent) getline(cin, bufferStr);
                    int x;
                    cin >> x;
                    cin.ignore(1000, '\n');
                    isNPresent = false;

                }

            }
            delete stmt;
        }
        catch (exception e) {
            cout << e.what() << endl;
            cout << "Enter 0 and exit";
            int x;
            cin >> x;
            cin.ignore(1000, '\n');

        }


        //LOGIN COMPLETED



        //ADMIN WORKS
        if (x == 4) {
            while (adminBool) {
                system("cls");
                cout << "1.Add Librarian\n2.Remove Librarian\n3.Add Student\n4.Remove Student\n5.Show records of Librarians\n6.Show records of students\n7.Log Out";
                cout << endl;
                try {
                    int y;
                    if (isNPresent) getline(cin, bufferStr);
                    cin >> y;
                    cin.ignore(1000, '\n');
                    isNPresent = false;


                    //ADDING LIBRARIAN
                    if (y == 1) {
                        try {
                            system("cls");
                            cout << "Add Librarian\n\n";
                            string id, name, password;
                            long long phn;
                            if (isNPresent) getline(cin, bufferStr);
                            cout << "Enter ID: " << endl;
                            getline(cin, id);
                            cout << "Enter Name: " << endl;
                            getline(cin, name);
                            cout << "Enter Number: " << endl;
                            
                            cin >> phn;
                            cin.ignore(1000, '\n');
                            cout << "Enter Password: " << endl;
                            
                            getline(cin, password);
                            isNPresent = false;
                            pstmt = con->prepareStatement("INSERT INTO STAKEHOLDERS VALUES (?,?,?,?);");
                            pstmt->setString(1, id);
                            pstmt->setString(2, name);
                            pstmt->setInt64(3, phn);
                            pstmt->setString(4, password);
                            pstmt->executeQuery();
                            y = 10;
                            delete pstmt;
                        }
                        catch (exception e) {
                            cout << "Something went wrong" << endl;
                            cout << e.what();

                            int x;
                            cout << "Enter 0 and enter\n";
                            cin >> x;
                            cin.ignore(1000, '\n');

                        }

                    }
                    //REMOVING LIBRARIAN
                    else if (y == 2) {
                        try {
                            system("cls");
                            cout << "Remove Librarian";
                            string id, name, password;
                            long long phn;
                            cout << "Enter ID: ";
                            getline(cin, id);
                            pstmt = con->prepareStatement("SELECT * FROM stakeholders WHERE Emp_ID = ?");
                            pstmt->setString(1, id);
                            res = pstmt->executeQuery();
                            while (res->next()) {
                                cout << "Are you sure you want to remove (y/n)" << res->getString("Emp_ID") << " " << res->getString("Emp_Name");
                            }
                            bool check = false;
                            char c;
                            cin >> c;
                            cin.ignore(1000, '\n');
                            if (c == 'y' || c == 'Y') check = true;
                            pstmt = con->prepareStatement("DELETE FROM stakeholders WHERE Emp_ID = ?");
                            pstmt->setString(1, id);
                            if (check) {
                                pstmt->executeQuery();
                            }
                            y = 10;
                            delete pstmt;
                        }
                        catch (exception e) {
                            cout << "Something went wrong" << endl;
                            cout << e.what();
                            cout << "Press 0 then enter to continue" << endl;
                            int x;
                            cin >> x;
                            cin.ignore(1000, '\n');


                        }

                    }
                    //ADDING STUDENT
                    if (y == 3) {
                        try {
                            system("cls");
                            cout << "Add Student\n\n" << endl;
                            string course, name;
                            long long phn;
                            cout << "Enter Name: " << endl;
                            getline(cin, name);
                            cout << "Enter Number: " << endl;
                            cin >> phn;
                            cin.ignore(1000, '\n');

                            cout << "Enter Course: " << endl;
                            getline(cin, course);
                            pstmt = con->prepareStatement("INSERT INTO student(Stud_Name,Course,Stud_No) VALUES (?,?,?);");
                            pstmt->setString(1, name);
                            pstmt->setString(2, course);
                            pstmt->setInt64(3, phn);
                            //pstmt->setString(4, password);
                            pstmt->executeQuery();
                            y = 10;
                            delete pstmt;
                        }
                        catch (exception e) {
                            cout << "Something went wrong" << endl;
                            cout << e.what();
                            cout << "\nEnter 0 and press enter to continue";
                            int x;
                            cin >> x;
                            cin.ignore(1000, '\n');

                        }

                    }
                    //REMOVING STUDENT
                    else if (y == 4) {
                        try {
                            system("cls");
                            cout << "Remove Student\n\n" << endl;
                            string id, name, password;
                            long long phn;
                            cout << "Enter ID: ";
                            getline(cin, id);
                            pstmt = con->prepareStatement("SELECT * FROM student WHERE Stud_ID = ?");
                            pstmt->setString(1, id);
                            res = pstmt->executeQuery();
                            while (res->next()) {
                                cout << "Are you sure you want to remove (y/n)" << res->getString("Stud_ID") << " " << res->getString("Stud_Name");
                            }
                            bool check = false;
                            char c;
                            cin >> c;
                            cin.ignore(1000, '\n');
                            if (c == 'y' || c == 'Y') check = true;
                            pstmt = con->prepareStatement("DELETE FROM student WHERE Stud_ID = ?");
                            pstmt->setString(1, id);
                            if (check) {
                                pstmt->executeQuery();
                            }
                            y = 10;
                            delete pstmt;
                        }
                        catch (exception e) {
                            cout << "Something went wrong" << endl;
                            cout << e.what();
                            cout << "Press 0 then enter to continue" << endl;
                            int x;
                            cin >> x;
                            cin.ignore(1000, '\n');


                        }

                    }
                    //SHOWING LIBRARIANS
                    else if (y == 5) {
                        try {
                            cout << "Librarians are:\n" << endl;
                            pstmt = con->prepareStatement("SELECT * FROM stakeholders WHERE Emp_ID like 'LIB%'");
                            //string k = "LIB%";
                            //pstmt->setString(1, k);
                            res = pstmt->executeQuery();
                            while (res->next()) {
                                cout << res->getString("Emp_ID") << " " << res->getString("Emp_Name") << " " << res->getInt64("Emp_No") << endl;
                            }
                            cout << "Enter 0 and enter to exit" << endl;
                            int x;
                            cin >> x;
                            cin.ignore(1000, '\n');


                            delete pstmt;
                            y = 10;
                        }
                        catch (exception e) {
                            cout << e.what() << endl;
                            cout << "Enter 0 and enter to exit" << endl;
                            int x;
                            cin >> x;
                            cin.ignore(1000, '\n');

                        }

                    }
                    //SHOWING STUDENTS
                    else if (y == 6) {
                        try {
                            cout << "Students are:\n" << endl;
                            pstmt = con->prepareStatement("SELECT * FROM student ");
                            //string k = "LIB%";
                            //pstmt->setString(1, k);
                            res = pstmt->executeQuery();
                            while (res->next()) {
                                cout << res->getString("Stud_ID") << " " << res->getString("Stud_Name") << " " << res->getString("Course") << " " << res->getInt64("Stud_No") << endl;
                            }
                            cout << "Enter 0 and enter to exit" << endl;
                            int x;
                            cin >> x;
                            cin.ignore(1000, '\n');


                            delete pstmt;
                            y = 10;
                        }
                        catch (exception e) {
                            cout << e.what() << endl;
                            cout << "Enter 0 and enter to exit" << endl;
                            int x;
                            cin >> x;
                            cin.ignore(1000, '\n');

                        }

                    }
                    //LOGGING OUT
                    else if (y == 7) {
                        cout << "Are you sure you want to log out (y/n)";
                        char c; cin >> c;
                        cin.ignore(1000, '\n');
                        if (c == 'y' || c == 'Y') {
                            adminBool = false;
                        }
                    }
                }
                catch (exception e) {
                    cout << e.what() << endl;
                    cout << "Enter any int and exit";
                    int x;
                    cin >> x;
                    cin.ignore(1000, '\n');

                }
            }
        }



        //LIBRARIAN WORKS
        /*
            Check Books
            Order Books
            Remove Books
            Issue Book to a student
            Return Book
            Log Out
        */
        else if (x == 5)
        {
            while (libBool) {
                system("cls");
                cout << "1.Check Books\n2.Add Books\n3.Remove Books\n4.Issue Book\n5.Return Book\n6.Log Out" << endl;
                int y;
                cin >> y;
                cin.ignore(1000, '\n');

                try {
                    //SHOW BOOKS
                    if (y == 1) {
                        try {
                            cout << "Books are:\n" << endl;
                            pstmt = con->prepareStatement("SELECT * FROM books");
                            //string k = "LIB%";
                            //pstmt->setString(1, k);
                            res = pstmt->executeQuery();
                            int i = 1;
                            while (res->next()) {
                                cout << i << ". " << res->getString("Book_Name") << " by " << res->getString("Book_Author") << endl;
                            }
                            cout << "Enter 0 and enter to exit" << endl;
                            int x;
                            cin >> x;
                            cin.ignore(1000, '\n');


                            delete pstmt;
                            y = 10;
                        }
                        catch (exception e) {
                            cout << e.what() << endl;
                            cout << "Enter 0 and enter to exit" << endl;
                            int x;
                            cin >> x;
                            cin.ignore(1000, '\n');

                        }
                    }
                    //ADD BOOKS
                    else if (y == 2)
                    {
                        try {
                            system("cls");
                            cout << "Add Books\n\n";
                            string  name, author;
                            long long phn;

                            cout << "Enter Name: " << endl;

                            getline(cin, name);



                            cout << "Enter Author: " << endl;
                            getline(cin, author);

                            pstmt = con->prepareStatement("INSERT INTO books (Book_Name, Book_Author) values (?,?);");
                            pstmt->setString(1, name);
                            pstmt->setString(2, author);
                            pstmt->executeQuery();

                            y = 10;
                            delete pstmt;
                        }
                        catch (exception e) {
                            cout << "Something went wrong" << endl;
                            cout << e.what();
                            int x;
                            cout << "Enter 0 and enter\n";
                            cin >> x;
                            cin.ignore(1000, '\n');

                        }
                    }
                    //REMOVE BOOKS
                    else if (y == 3) {
                        try {
                            system("cls");
                            cout << "Remove Books\n\n" << endl;
                            string id, name, password;
                            long long phn;
                            cout << "Enter ID: ";
                            getline(cin, id);
                            pstmt = con->prepareStatement("SELECT * FROM books WHERE Book_ID = ?");
                            pstmt->setString(1, id);
                            res = pstmt->executeQuery();
                            while (res->next()) {
                                cout << "Are you sure you want to remove (y/n)" << res->getString("Book_Name") << " by " << res->getString("Book_Author");
                            }
                            bool check = false;
                            char c;
                            cin >> c;
                            cin.ignore(1000, '\n');

                            if (c == 'y' || c == 'Y') check = true;
                            pstmt = con->prepareStatement("DELETE FROM books WHERE Book_ID= ?");
                            pstmt->setString(1, id);
                            if (check) {
                                pstmt->executeQuery();
                            }
                            y = 10;
                            delete pstmt;
                        }
                        catch (exception e) {
                            cout << "Something went wrong" << endl;
                            cout << e.what();
                            cout << "Press 0 then enter to continue" << endl;
                            int x;
                            cin >> x;
                            cin.ignore(1000, '\n');


                        }

                    }
                    //ISSUE BOOK
                    else if(y==4){
                    showStudents();
                    cout << "\n\nEnter Student ID to issue to :\n\n";
                    int studId;
                    cin >> studId;
                    cin.ignore(1000, '\n');

                    showBooks();
                    cout << "\n\nEnter Book ID to issue to :\n\n";
                    int bID;
                    cin >> bID;
                    cin.ignore(1000, '\n');

                    pstmt = con->prepareStatement("INSERT INTO issues(Issued_On,book_issued_id,issued_to_id) values (sysdate(), ?, ?);");
                    pstmt->setInt(1, bID);
                    pstmt->setInt(2, studId);

                    pstmt->executeQuery();
                    }
                    //RETURN BOOK
                    
                    else if (y == 5) {
                    showIssues(-1);
                    cout << "Select Issue Id: ";
                    int issId;
                    cin >> issId;
                    cin.ignore(1000,'\n');
                    pstmt = con->prepareStatement("DELETE FROM issues WHERE Issue_ID = ?");
                    pstmt->setInt(1, issId);
                    pstmt->executeQuery();
}
                    //LOG OUT
                    else if (y == 6) {
                        cout << "Are you sure you want to log out (y/n)";
                        char c; cin >> c;
                        cin.ignore(1000, '\n');
                        if (c == 'y' || c == 'Y') {
                            libBool = false;
                        }
                    }
                }
                catch (exception e) {
                    cout << e.what() << endl;
                    cout << "Enter 0 and enter to continue\n";
                    int p;
                    cin >> p;
                    cin.ignore(1000, '\n');
                }
            }
        }
        
        //STUDENT WORKS
        /*
            1.Check Books
            2.Check his records
        */

        else if(x==6)
        {
        cout << "1.Check Books\n2.Check your issues\n";
        int y; cin >> y;
        cin.ignore(1000, '\n');
        try {

            if (y == 1) {
                string s;

                cout << "Enter Name: ";
                getline(cin, s);
                try {
                    cout << "Books are:\n" << endl;
                    pstmt = con->prepareStatement("SELECT * FROM books where Book_Name like ?");
                    string k = "%";
                    k += s;
                    k += '%';
                    pstmt->setString(1, k);
                    //pstmt->setString(1, k);
                    res = pstmt->executeQuery();
                    int i = 1;
                    while (res->next()) {
                        cout << i++ << ". " << res->getString("Book_Name") << " by " << res->getString("Book_Author") <<"  "<< (res->getInt("is_issued") ? "Not Available" : "Available") << endl << endl << endl;
                    }
                    cout << "Enter 0 and enter to exit" << endl;
                    int x;
                    cin >> x;
                    cin.ignore(1000, '\n');


                    delete pstmt;
                    y = 10;
                }
                catch (exception e) {
                    cout << e.what() << endl;
                    cout << "Enter 0 and enter to exit" << endl;
                    int x;
                    cin >> x;
                    cin.ignore(1000, '\n');

                }
            }
            else if (y == 2) {
                cout << "enter your id: ";
                int l;
                cin >> l;
                cin.ignore(1000, '\n');

                showIssues(l);
                cout << "Enter 0 and enter to continue";
                int p;
                cin >> p;
                cin.ignore(1000, '\n');
            }
        }
        catch (exception e) {
            cout << e.what() << endl;
            cout << "Enter 0 and enter to exit\n";
            int p;
            cin >> p;
            cin.ignore(100, '\n');
        }


        }
    }
            delete con;
            system("pause");
            return 0;
}