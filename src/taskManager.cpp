#include <bits/stdc++.h>
using namespace std;

const int MAX = 500;

struct Task
{
    string title;
    string description;
    bool completed;
};
struct TaskList
{
    Task arr[50];
    int n;
    TaskList(): n(0) {}
};
struct Date
{
    int d;
    int m;
    int y;
    Date *next;
    Date *prev;
    TaskList tasks;
    Date(int dd, int mm, int yyyy) : d(dd), m(mm), y(yyyy), next(nullptr), prev(nullptr) {}
};
// struct DateList
// {
//     Date *Head;
//     Date *Tail;
//     DateList(): Head(nullptr), Tail(nullptr) {}
// };

struct Project
{
    int id;
    string title;
    string managerUsername;
    string membersUsername[50];
    Date createdOn;
    Date dueDate;
    TaskList tasks;
};
struct User
{
    string username;
    string password;
    User(string u, string pw) : username(u), password(pw) {}
};
struct TreeNode
{
    User user;
    TreeNode *left;
    TreeNode *right;
    TreeNode(User u) : user(u), left(nullptr), right(nullptr) {}
};

class UserTree
{
    private:
        TreeNode *root;

    public:
        UserTree() : root(nullptr) {}

        int insert(User user)
        {
            return insertHelper(root, user);
        }
        User *search(string username)
        {
            return searchHelper(root, username);
        }
        User *login(string username, string pw)
        {
            User *user = searchHelper(root, username);
            if (user != nullptr && user->password == pw)
                return user;
            return nullptr;
        }

    private:
        int insertHelper(TreeNode *node, User user)
        {
            if (node != nullptr)
            {
                if (user.username < node->user.username)
                    return insertHelper(node->left, user);
                else if (user.username > node->user.username)
                    return insertHelper(node->right, user);
                else
                {
                    cout << "username đã tồn tại.\n";
                    return 0;
                }
            }
            node = new TreeNode(user);
            if (node == nullptr)
                return 0;
            else
                return 1;
        }

        User *searchHelper(TreeNode *node, string username)
        {
            if (node == nullptr)
                return nullptr;
            else if (node->user.username == username)
                return &(node->user);
            else if (node->user.username < node->user.username)
                return searchHelper(node->left, username);
            else
                return searchHelper(node->right, username);
        }
};

class DateList
{
    private:
        Date *head;
        Date *tail;
    
    public:
        DateList(): head(nullptr), tail(nullptr) {}

        Date *findDate(int d, int m, int y) {
            Date *date = tail;
            while (date) {
                if (date->d == d && date->m == m && date->y == y)
                    return date;
                date = date->prev;
            }
            return nullptr;
        }

        void addDate(Date *newDate) {
            if (head == nullptr) {
                head = tail = newDate;
                return;
            }

            Date *current = tail;
            while(current) {
                if (current->y > newDate->y || current->y == newDate->y && current->m > newDate->m
                    || current->y == newDate->y && current->m == newDate->m && current->d > newDate->d)
                        current = current -> prev;
                else {
                    newDate->next = current->next;
                    newDate->prev = current;
                    current->next = newDate;
                    if (newDate->next)
                        newDate->next->prev = newDate;
                    else
                        tail = newDate;
                    return;
                }
            }
            newDate->next = head;
            head->prev = newDate;
            head = newDate;
        }

        Date *getTomorrow(Date *givenDate)
        {
            return (givenDate && givenDate->next) ? givenDate->next : nullptr;
        }

        Date *getYesterday(Date *givenDate)
        {
            return (givenDate && givenDate->prev) ? givenDate->prev : nullptr;
        }

};

void displayDate(Date *date)
{
    if (date)
        cout << date->d << "/" << date->m << "/" << date->y;
    else
        cout << "Not found.";
    cout << endl;
}

int main()
{
    UserTree users;
    users.insert(User("user1", "111111"));
    users.insert(User("user2", "222222"));
    users.insert(User("user3", "333333"));

    User *currentUser = nullptr;
    string requestUser = "";
    string password = "";
    int option = -1;
    while (currentUser == nullptr)
    {
        cout << "1. Đăng nhập\n";
        cout << "2. Đăng ký\n";
        cin >> option;
        switch (option)
        {
            case 1:
            {
                cout << "Tên đăng nhập (user1): ";
                cin.ignore();
                getline(cin, requestUser);
                cout << "Mật khẩu (111111): ";
                getline(cin, password);
                User *user = users.login(requestUser, password);
                if (user != nullptr)
                {
                    currentUser = user;
                    cout << "Đăng nhập thành công";
                }
                else
                    cout << "Sai thông tin đăng nhập!\n";
                break;
            }
            case 2:
            {
                cout << "Username: ";
                cin.ignore();
                getline(cin, requestUser);
                cout << "Password: ";
                getline(cin, password);
                users.insert(User(requestUser, password));
            }
        }
    }

    DateList dates;
    Date *activeDate = new Date(0,0,0);
    while (true)
    {
        cout << "To-Do Manager" << endl;
        cout << "Date: ";
        displayDate(activeDate);
        cout << "Tasks: \n";

        cout << "Options: \n";
        cout << "1. Change/Add date\n";
        cout << "2. Add task\n";
        cout << "3. Edit task\n";
        cout << "4. Remove task\n";
        cout << "5. Move to Next Day\n";
        cout << "6. Move to Previous Day\n";
        cout << "7. Exit\n\n";

        int choice;
        cin >> choice;

        switch (choice)
        {
            case 1:
            {
                int d, m, y;
                cout << "Enter day month year (16 9 2023): ";
                cin >> d >> m >> y;
                Date *date = dates.findDate(d, m, y);
                if (date)
                {
                    activeDate = date;
                }
                else
                {
                    Date *newDate = new Date(d, m, y);
                    dates.addDate(newDate);
                    activeDate = newDate;
                }
                break;
            }
        }
    }
}