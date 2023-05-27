#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
char blue[]={0x1b,'[','0',';','3','4','m',0};
char normal[]={0x1b,'[','0',';','3','9','m',0};
char yellow[]={0x1b,'[','0',';','3', '3', 'm',0};
char red[]={0x1b,'[','0',';','3','1','m',0};
using namespace std;

enum RelationshipStatus
{
    Single,
    InRelationship,
    Married,
    Divorced
};

class UserInformation
{
public:
    string name;
    string birthdate;
    string occupation;
    RelationshipStatus relationshipStatus;

    UserInformation() = default;

    UserInformation(const string &name, const string &birthdate, const string &occupation, RelationshipStatus relationshipStatus)
        : name(name), birthdate(birthdate), occupation(occupation), relationshipStatus(relationshipStatus) {}
};

template <typename T>
class AccountSystem
{
private:
    struct Account
    {
        string username;
        string password;
        UserInformation userInfo;
        string friends;
    };

    Account loggedInAccount;

public:
    bool isValidAccount(const T &Username)
    {
        ifstream file("accounts.txt");
        if (file.is_open())
        {
            string username, password;
            while (file >> username >> password)
            {
                if (username == Username)
                {
                    return true;
                }
            }
            file.close();
        }
        return false;
    }
    void createAccount()
    {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        while (isValidAccount(username))
        {
            cout<<"Sorry this username is already taken. Try another.\n"<<endl;
            cout << "Enter username: ";
            cin>>username;
        }
        
        cout << "Enter password: ";
        cin >> password;

        // Create an account object
        Account account;
        account.username = username;
        account.password = password;

        cout << "Enter name: ";
        cin.ignore();
        getline(cin, account.userInfo.name);
        cout << "Enter birthdate (DD/MM/YYYY): ";
        cin >> account.userInfo.birthdate;
        cout << "Enter occupation: ";
        cin.ignore();
        getline(cin, account.userInfo.occupation);

        int relationshipChoice;
        cout << "Select relationship status:\n";
        cout << "1. Single\n";
        cout << "2. In a relationship\n";
        cout << "3. Married\n";
        cout << "4. Divorced\n";
        cout << "Enter choice: ";
        cin >> relationshipChoice;

        switch (relationshipChoice)
        {
        case 1:
            account.userInfo.relationshipStatus = RelationshipStatus::Single;
            break;
        case 2:
            account.userInfo.relationshipStatus = RelationshipStatus::InRelationship;
            break;
        case 3:
            account.userInfo.relationshipStatus = RelationshipStatus::Married;
            break;
        case 4:
            account.userInfo.relationshipStatus = RelationshipStatus::Divorced;
            break;
        default:
            cout << "Invalid choice. Setting relationship status to Single.\n";
            account.userInfo.relationshipStatus = RelationshipStatus::Single;
        }

        // Open the file in append mode and write the account details
        ofstream file("accounts.txt", ios::app);
        if (file.is_open())
        {
            file << account.username << " " << account.password << endl;
            file << account.userInfo.name << endl;
            file << account.userInfo.birthdate << endl;
            file << account.userInfo.occupation << endl;
            file << static_cast<int>(account.userInfo.relationshipStatus) << endl;
            file.close();
            cout << "Account created successfully!" << endl;
        }
        else
        {
            cout << "Failed to create account. Please try again later." << endl;
        }
    }
    bool isfriend(const T &friendUsername)
    {
        fstream file(loggedInAccount.username + "friends" + ".txt", ios::in);
        if (file.is_open())
        {
            string friendid;
            while (getline(file, friendid))
            {
                if (friendid == friendUsername)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool isblocked(const T &loggedin, const T &friendUsername)
    {
        fstream file(loggedin + "blocked" + ".txt", ios::in);
        if (file.is_open())
        {
            string blockedid;
            while (getline(file, blockedid))
            {
                if (blockedid == friendUsername)
                {
                    return true;
                }
            }
        }
        return false;
    }

    void addFriend(const T &friendUsername)
    {
        if (!isValidAccount(friendUsername))
        {
            cout<<"Sorry! Entered username is invalid. Try another username."<<endl;
            return;
        }
        else if(friendUsername==loggedInAccount.username)
        {
            cout<<red<<"You can't add yourself as friend!\n"<<normal;
            return;
        }
        // Check if the friend is already added
        if (isfriend(friendUsername))
        {
            cout << "You have already added " << friendUsername << endl;
            return;
        }
        else if (isblocked(loggedInAccount.username, friendUsername))
        {
            cout <<red<< "Sorry you have blocked this person." <<normal<< endl;
        }
        else if (isblocked(friendUsername, loggedInAccount.username))
        {
            cout << "Sorry can't add " << friendUsername << " ! (This person may have blocked you)" << endl;
            return;
        }
        else
        {
            fstream file(loggedInAccount.username + "friends" + ".txt", ios::app);
            file << friendUsername << endl;
            cout <<blue<< "Friend added successfully!" <<blue<< endl;
            file.close();
            ofstream file2(friendUsername + "friends" + ".txt", ios::app);
            if (file2.is_open())
            {
                file2 << loggedInAccount.username << endl;
            }
            file2.close();
        }
    }

    void seeFriends()
    {
        cout <<blue<< "\nYour friends:" <<normal<< endl;
        ifstream file(loggedInAccount.username + "friends" + ".txt", ios::app);
        if (file.is_open())
        {
            string username;
            int c = 1;
            while (getline(file, username))
            {
                cout << c << ". " << username << endl;
                c++;
            }
            file.close();
        }
        cout << endl;

        cout << "Do you want to view detailed information about a friend? (Y/N): ";
        char choice;
        cin >> choice;

        if (toupper(choice) == 'Y')
        {
            cout << "Enter friend's username: ";
            string friendUsername;
            cin >> friendUsername;
            displayFriendInformation(friendUsername);
        }
    }

    void displayFriendInformation(const T &friendUsername)
    {
        if (isblocked(loggedInAccount.username, friendUsername))
        {
            cout << "Sorry you've blocked " << friendUsername << " !" << endl;
            return;
        }
        else if (isblocked(friendUsername, loggedInAccount.username))
        {
            cout << "Sorry can't show any information of " << friendUsername << " ! (This person may have blocked you)" << endl;
            return;
        }

        else if (isfriend(friendUsername))
        // Code to retrieve and display friend's information from the file
        {
            ifstream file("accounts.txt");
            if (file.is_open())
            {
                string username, password;
                while (file >> username >> password)
                {
                    if (username == friendUsername)
                    {
                        UserInformation userInfo;
                        string name, birthdate, occupation;
                        int relationshipStatus;
                        file.ignore();

                        getline(file, name);
                        getline(file, birthdate);
                        getline(file, occupation);
                        file >> relationshipStatus;

                        userInfo.name = name;
                        userInfo.birthdate = birthdate;
                        userInfo.occupation = occupation;
                        userInfo.relationshipStatus = static_cast<RelationshipStatus>(relationshipStatus);

                        cout <<blue<<"\nAbout "<<friendUsername<<"\n"<<normal;
                        cout << "\tUsername: " << friendUsername << endl;
                        cout << "\tName: " << userInfo.name << endl;
                        cout << "\tBirthdate: " << userInfo.birthdate << endl;
                        cout << "\tOccupation: " << userInfo.occupation << endl;
                        cout << "\tRelationship Status: ";

                        switch (userInfo.relationshipStatus)
                        {
                        case RelationshipStatus::Single:
                            cout << "Single" << endl;
                            break;
                        case RelationshipStatus::InRelationship:
                            cout << "In a relationship" << endl;
                            break;
                        case RelationshipStatus::Married:
                            cout << "Married" << endl;
                            break;
                        case RelationshipStatus::Divorced:
                            cout << "Divorced" << endl;
                            break;
                        }

                        file.close();
                        return;
                    }
                }
                file.close();
            }
        }
        else
            cout << "Sorry this person is not your friend. Add " << friendUsername << " to see personal information" << endl;
    }

    void removefriend(const T &loggedin,const T &removeuser)
    {
        fstream file;
        file.open(loggedin + "friends" + ".txt");
        vector<string> lines;
        string line;
        while (getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();
        ofstream file2;
        file2.open(loggedin + "friends" + ".txt");
        for (int i = 0; i < lines.size(); i++)
        {
            if (lines[i] != removeuser)
                file2 << lines[i] << endl;
        }
        file2.close();
    }
    void removeblock(const T &removeuser)
    {
        fstream file;
        file.open(loggedInAccount.username + "blocked" + ".txt");
        vector<string> lines;
        string line;
        while (getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();
        ofstream file2;
        file2.open(loggedInAccount.username + "blocked" + ".txt");
        for (int i = 0; i < lines.size(); i++)
        {
            if (lines[i] != removeuser)
                file2 << lines[i] << endl;
        }
        file2.close();

        cout <<yellow<< removeuser << " is removed from blocklist." <<normal<< endl;
    }
    void blockUser(const T &usernameToBlock)
    {
        if (isblocked(loggedInAccount.username, usernameToBlock))
        {
            cout <<yellow<< "You have already blocked " << usernameToBlock <<normal<< endl;
            return;
        }
        else
        {
            fstream file(loggedInAccount.username + "blocked" + ".txt", ios::out);
            file << usernameToBlock << endl;
            cout <<red<< "You have blocked " << usernameToBlock << "!" <<normal<< endl;
            removefriend(loggedInAccount.username,usernameToBlock);
            removefriend(usernameToBlock,loggedInAccount.username);
            file.close();
        }
    }

    void login()
    {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        // Open the file to read the account details
        ifstream file("accounts.txt");
        if (file.is_open())
        {
            string storedUsername, storedPassword;
            while (file >> storedUsername >> storedPassword)
            {
                if (username == storedUsername && password == storedPassword)
                {
                    file.close();
                    loggedInAccount.username = username;
                    loggedInAccount.password = password;

                    cout <<blue<< "\n\tLogin successful. Welcome, " << loggedInAccount.username << "!" <<normal<< endl;

                    bool loggedOut = false;
                    while (!loggedOut)
                    {
                        cout<<red<<"\nMenu"<<normal;
                        cout << "\n1. Add Friend\n2. See Friends\n3. Unfriend\n4. Block User\n5. Unblock\n6. Log Out\nEnter your choice: ";
                        int choice;
                        cin >> choice;

                        switch (choice)
                        {
                        case 1:
                        {
                            T friendUsername;
                            cout << "Enter username: ";
                            cin >> friendUsername;
                            addFriend(friendUsername);
                            break;
                        }
                        case 2:
                            seeFriends();
                            break;
                        case 3:
                        {
                            T usernametoUnfriend;
                            cout << "Enter username to unfriend: ";
                            cin >> usernametoUnfriend;
                            removefriend(loggedInAccount.username,usernametoUnfriend);
                            removefriend(usernametoUnfriend,loggedInAccount.username);
                            cout<<"\nYou've unfriended "<<usernametoUnfriend<<endl;
                            break;
                        }
                        case 4:
                        {
                            T usernameToBlock;
                            cout << "Enter username to block: ";
                            cin >> usernameToBlock;
                            blockUser(usernameToBlock);
                            break;
                        }
                        case 5:
                        {
                            T usernameToUnblock;
                            cout << "Enter username to unblock: ";
                            cin >> usernameToUnblock;
                            removeblock(usernameToUnblock);
                            break;
                        }
                        case 6:
                            cout <<blue<< "Logging out. Goodbye, " << loggedInAccount.username << "!"<<normal << endl;
                            loggedOut = true;
                            break;
                        default:
                            cout << "Invalid choice. Please try again." << endl;
                        }
                    }

                    return;
                }
            }

            file.close();
        }

        cout << "Invalid username or password. Please try again." << endl;
    }
};

int main()
{
    int choice;

    while (true)
    {
        cout<<blue<<"\t\t\t\t\t _________________________\n\t\t\t\t\t| Welcome to Facelessbook |\n\t\t\t\t\t _________________________\n\n"<<yellow<<endl;
        cout << "1. Create Account\n2. Login\n3. Exit\n\nEnter your choice: ";
        cin >> choice;
        cout<<"\n"<<normal;
        switch (choice)
        {
        case 1:
        {
            AccountSystem<string> accountSystem;
            accountSystem.createAccount();
            break;
        }
        case 2:
        {
            AccountSystem<string> accountSystem;
            accountSystem.login();
            break;
        }
        case 3:
            cout<<blue << "Exiting program. Goodbye!" <<blue<< endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}
