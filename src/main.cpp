#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
const int MAX = 500;
const string notFound = "Không tồn tại. Hãy tạo mới\n";
const string invalidChoice = "Lỗi. Hãy thử lại\n";
struct User;
class UserTree;
class TaskStack;
class TaskAList;
User *loginUser();
void home(User *currentUser);
void editRemoveTask(User *currentUser);

struct Date
{
    int d;
    int m;
    int y;
    Date(int dd, int mm, int yyyy) : d(dd), m(mm), y(yyyy) {}
};
void displayDate(Date *date);

struct Task
{
    string title;
    string description;
    Date *pDeadline;
    bool completed;
};

struct Node {
    Task *pTask;
    Node *next;
    Node(Task *t): pTask(t), next(nullptr) {}
};

class TaskAList
{
public:
    Task arr[MAX];
    int n;
    TaskAList() : n(0) {}

    void displayTasks()
    {
        for (int i = 0; i < n; i++)
        {
            if (i > 4 && i % 5 == 0)
            {
                char ch = 'n';
                cout << "Hiện thêm? (y/n) ";
                cin >> ch;
                if (ch != 'y' && ch != 'Y')
                    break;
            }
            cout << "\t\t\t" << i + 1 << ". Title: " << arr[i].title << endl;
            cout << "\t\t\t   Description: " << arr[i].description << endl;
            cout << "\t\t\t   Deadline: ";
            displayDate(arr[i].pDeadline);
            cout << "\t\t\t   ";
            displayStatus(arr[i].completed);
            cout << "\t\t\t        ------\n";
        }
    }

    void addTask(Task task)
    {
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

    void editTask(User *currentUser, int pos)
    {
        char editOption;
        while (editOption != 'e')
        {
            string title, des;
            cout << "a. Cập nhật tiêu đề\n";
            cout << "b. Cập nhật chú thích\n";
            cout << "c. Đánh dấu hoàn thành\n";
            cout << "d. Thay đổi deadline\n";
            cout << "e. Trở lại trang chủ\n";
            cin >> editOption;
            cin.ignore();
            switch (editOption)
            {
            case 'a':
            {
                cout << "Enter new title: ";
                getline(cin, title);
                arr[pos].title = title;
                break;
            }
            case 'b':
            {
                cout << "Enter new description: ";
                getline(cin, des);
                arr[pos].description = des;
                break;
            }
            case 'c':
            {
                arr[pos].completed = true;
                Task *task = new Task(arr[pos]);
                currentUser->completedTasks.push(new Node(task));
                currentUser->uncompletedTasks.removeTask(pos);
                break;
            }
            case 'd':
            {
                cout << "Nhập deadline mới (d m y): ";
                cin >> arr[pos].pDeadline->d >> arr[pos].pDeadline->m >> arr[pos].pDeadline->y;
                break;
            }
            case 'e':
            {
                home(currentUser);
            }
            default:
                cout << invalidChoice << endl;
                break;
            }
        }
    }

private:
    void displayStatus(bool completed)
    {
        if (completed)
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            cout << "Đã hoàn thành\n";
        }
        else
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "Chưa hoàn thành\n";
        }
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
    }
};

class TaskStack {
    private:
        Node *pTop;
    
    public:
        TaskStack(): pTop(nullptr) {}

        void push(Node *p) {
            p->next = pTop;
            pTop = p;
        }

        Node *top() {
            return pTop;
        }

        bool isEmpty() {
            return pTop == nullptr;
        }

        void displayTasks() {
            int i = 0;
            for(Node *p = pTop; p->next != nullptr; p = p->next) {
                if (i > 4 && i % 5 == 0) {
                    char ch;
                    cout << "Hiển thị thêm? (y/n)\n";
                    cin >> ch;
                    if (ch != 'y' && ch != 'Y')
                        break;
                }
                cout << "Tiêu đề: " << p->pTask->title << endl;
                cout << "Chú thích: " << p->pTask->description << endl;
                i++;
            }
        }
};

struct User
{
        string username;
        string password;
        TaskStack completedTasks;
        TaskAList uncompletedTasks;
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
    TreeNode *root;

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
            cout << "\t\t\t Tên đăng nhập này đã tồn tại.\n";
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
        cout << notFound;
}

int chooseTaskIndex()
{
    int index;
    cout << "Enter task index: ";
    cin >> index;
    return index;
}

void printAppName()
{
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "\t\t\t____________________________\n";
    cout << "\t\t\t                            \n";
    cout << "\t\t\t       QUẢN LÝ CÔNG VIỆC    \n";
    cout << "\t\t\t____________________________\n\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}

UserTree users;
int main()
{
    users.insert(User("user1", "111111"));
    users.insert(User("user2", "222222"));
    users.insert(User("user3", "333333"));

    printAppName();
    User *currentUser = loginUser();

    home(currentUser);

    return 0;
}

User *loginUser()
{
    User *currentUser = nullptr;
    string requestUser = "";
    string password = "";
    int option = -1;
    while (currentUser == nullptr)
    {
        cout << "\t 1. Đăng nhập \t\t 2. Đăng ký \t\t 3.Thoát\n";
        cin >> option;
        switch (option)
        {
        case 1:
        {
            cout << "\t\t\tTên đăng nhập: ";
            cin.ignore();
            getline(cin, requestUser);
            cout << "\t\t\tMật khẩu: ";
            getline(cin, password);
            User *user = users.login(requestUser, password);
            if (user != nullptr)
            {
                currentUser = user;
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                cout << "\n\t\t\t\t Xin chào " << currentUser->username << endl
                     << endl;
            }
            else
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                cout << "\t\t\t Sai thông tin đăng nhập! \n";
            }
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
            break;
        }
        case 2:
        {
            cout << "Tên đăng nhập: ";
            cin.ignore();
            getline(cin, requestUser);
            cout << "Mật khẩu: ";
            getline(cin, password);
            users.insert(User(requestUser, password));
            break;
        }
        case 3:
            exit(0);
        default:
            break;
        }
    }
    return currentUser;
}

void home(User *currentUser)
{
    if (currentUser == nullptr)
        currentUser = loginUser();

    while (currentUser)
    {
        // system("cls");
        printAppName();
        cout << "\t\t\t";
        //displayDate(activeDate);
        cout << "\t  " << currentUser->username << endl;
        cout << "\t\t\tCông việc: \n";
        currentUser->uncompletedTasks.displayTasks();
        cout << "\t\t\tMenu: \n";
        cout << "\t\t\t ___________________________\n";
        cout << "\t\t\t|                           |\n";
        cout << "\t\t\t|   1. Thêm công việc       |\n";
        cout << "\t\t\t|   2. Chọn công việc       |\n"; // chỉnh sửa or xóa
        cout << "\t\t\t|   3. Tìm công việc        |\n";
        cout << "\t\t\t|   4. Xem thành tựu        |\n";
        cout << "\t\t\t|   5. Đăng xuất            |\n";
        cout << "\t\t\t|___________________________|\n";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            Task task;
            cout << "Tiêu đề: ";
            cin.ignore();
            getline(cin, task.title);
            cout << "Chú thích: ";
            getline(cin, task.description);
            cout << "Deadline (d m y): ";
            cin >> task.pDeadline->d >> task.pDeadline->m >> task.pDeadline->y;
            task.completed = false;
            currentUser->uncompletedTasks.addTask(task);
            break;
        }
        case 2:
        {
            editRemoveTask(currentUser);
            break;
        }
        // case 3:
        // {
        //     int index = 0;
        //     while (index <= 0 || index > currentUser->uncompletedTasks.n)
        //         index = chooseTaskIndex();
        //     Task task = currentUser->uncompletedTasks.arr[index - 1];
        //     break;
        // }
        case 4:
            currentUser->completedTasks.displayTasks();
            break;
        case 5:
        {
            // system("cls");
            printAppName();
            currentUser = loginUser();
            break;
        }
        default:
            cout << invalidChoice;
            break;
        }
    }
}

void editRemoveTask(User *currentUser)
{
    int index = 0;
    int choice = 0;
    while (index == 0 || index > currentUser->uncompletedTasks.n)
        index = chooseTaskIndex();
    cout << "31. Cập nhật\t\t 32. Xóa\n";
    cin >> choice;
    switch (choice)
    {
    case 31:
        currentUser->uncompletedTasks.editTask(currentUser, index - 1);
        break;
    case 32:
        currentUser->uncompletedTasks.removeTask(index - 1);
        break;
    default:
        break;
    }
}

