#include <iostream>
#include <string>
#include <ctime>
#include <windows.h>
#include <chrono>

using namespace std;

const int MAX = 500;
CONSOLE_FONT_INFOEX originalFontInfo;

void setTextBold() {
    originalFontInfo.cbSize = sizeof(originalFontInfo);
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(fontInfo);
    GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &fontInfo);

    fontInfo.FontWeight = FW_BOLD; // Set to bold
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &fontInfo);
}

struct Task
{
    string title;
    string description;
    bool completed;
};
class TaskList {
    public:
        Task arr[MAX];
        int n;
        TaskList(): n(0) {}

        void displayTasks()
        {
            for (int i = 0; i < n; i++)
            {
                    cout << i + 1 << ". Title: " << arr[i].title << endl;
                    cout << "   Description: " << arr[i].description << endl;
                    cout << "   Completed: " << arr[i].completed << endl;
                    cout << "        ------\n";
            }
        }

        void addTask(Task task) {
            arr[n] = task;
            n++;
        }

        void removeTask(int pos)
        {
            if (pos < 0 || n == 0 || pos >= n)
                    return;
            for (int i = pos; i < n - 1; i++)
            {
                    arr[i] = arr[i + 1];
            }
            n--;
        }
};
int chooseTaskIndex()
{
        int index;
        cout << "Enter task index: ";
        cin >> index;
        return index;
}
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
class DateList
{
    private:
    
    public:
        Date *head;
        Date *tail;
        DateList(): head(nullptr), tail(nullptr) {}

        bool isEmpty() {
            return head == nullptr;
        }

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


        // Date *getTomorrow(Date *givenDate)
        // {
        //     if (givenDate->next == nullptr) {
        //         givenDate->next = getDate( + chrono::hours(24));
        //     }
        //     return (givenDate && givenDate->next) ? givenDate->next : nullptr;
        // }

        // Date *getYesterday(Date *givenDate)
        // {
        //     return (givenDate && givenDate->prev) ? givenDate->prev : nullptr;
        // }
};
Date *getDate(time_t time)
{
    tm *localTime = localtime(&time);
    Date *ptoday = new Date(localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900);
    return ptoday;
}

struct User
{
    string username;
    string password;
    DateList dates;
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

        void insert(User user)
        {
            root = insertHelper(root, user);
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
        TreeNode *insertHelper(TreeNode *node, User user)
        {
            if (node == nullptr)
                return new TreeNode(user);

            if (user.username < node->user.username)
                node->left = insertHelper(node->left, user);
            else if (user.username > node->user.username)
                node->right = insertHelper(node->right, user);
            else
            {
                cout << "username đã tồn tại.\n";
                return node;
            }
            return node;
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
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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
                system("cls");
                cout << "Tên đăng nhập: ";
                cin.ignore();
                getline(cin, requestUser);
                cout << "Mật khẩu: ";
                getline(cin, password);
                User *user = users.login(requestUser, password);
                if (user != nullptr)
                {
                    currentUser = user;
                    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    cout << "Đăng nhập thành công\n\n";
                }
                else
                {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                    cout << "Sai thông tin đăng nhập!\n";
                }
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                break;
            }
            case 2:
            {
                system("cls");
                cout << "Tên đăng nhập: ";
                cin.ignore();
                getline(cin, requestUser);
                cout << "Mật khẩu: ";
                getline(cin, password);
                users.insert(User(requestUser, password));
                break;
            }
            default:
            {
                system("cls");
            }
        }
    }

    Date *activeDate = currentUser->dates.tail;
    if (activeDate == nullptr) {
        activeDate = getDate(time(nullptr));
        currentUser->dates.addDate(activeDate);
    }

    while (true)
    {
        cout << "To-Do Manager" << endl;
        cout << "Date: ";
        displayDate(activeDate);
        cout << "Tasks: \n";
        activeDate->tasks.displayTasks();
        cout << "Options: \n";
        cout << "1. Thay đổi ngày\n";
        cout << "2. Thêm công việc\n";
        cout << "3. Cập nhật công việc\n";
        cout << "4. Xóa công việc\n";
        cout << "5. Ngày tiếp theo\n";
        cout << "6. Ngày phía trước\n";
        cout << "7. Thoát\n\n";

        int choice;
        cin >> choice;

        switch (choice)
        {
            case 1:
            {
                int d, m, y;
                cout << "Enter day month year (d m y): ";
                cin >> d >> m >> y;
                Date *date = currentUser->dates.findDate(d, m, y);
                if (date)
                {
                    activeDate = date;
                }
                else
                {
                    Date *newDate = new Date(d, m, y);
                    currentUser->dates.addDate(newDate);
                    activeDate = newDate;
                }
                break;
            }
            case 2:
            {
                Task task;
                cout << "Enter task's title: ";
                cin.ignore();
                getline(cin, task.title);
                cout << "Enter task's description: ";
                getline(cin, task.description);
                task.completed = false;
                activeDate->tasks.addTask(task);
                break;
            }
            case 4:
            {
                int index = 0;
                while (index <= 0 || index > activeDate->tasks.n)
                    index = chooseTaskIndex();
                Task task = activeDate->tasks.arr[index - 1];
                activeDate->tasks.removeTask(index - 1);
                break;
            }
            // case 5:
            //     if (activeDate->next)
            //         activeDate = activeDate->next;
            //     else
            //         cout << "Not found\n";
            //     break;
            // case 6:
            //     if (activeDate->prev)
            //         activeDate = activeDate->prev;
            //     else
            //         cout << "Not found\n";
            //     break;
            case 7:
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }
    while (currentUser->dates.head)
    {
        Date *temp = currentUser->dates.head;
        currentUser->dates.head = currentUser->dates.head->next;
        delete temp;
    }

    return 0;
}