#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <windows.h>
#include <conio.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
const int MAX = 100;

class Constants
{
public:
    const string notFound = "Không tồn tại. Hãy tạo mới\n";
    const string invalidChoice = "Lỗi. Hãy thử lại\n";
    const string usernameExisted = "Tên đăng nhập này đã tồn tại. Vui lòng chọn tên khác\n";
    const string invalidLogin = "Sai thông tin đăng nhập!\n";
    const string usernameLabel = "Tên đăng nhập";
    const string pwLabel = "Mật khẩu";
    const string deleteSuccess = "Xóa thành công";
};
Constants constants = Constants();

void printAppName()
{
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "\t\t\t____________________________\n";
    cout << "\t\t\t                            \n";
    cout << "\t\t\t       QUẢN LÝ CÔNG VIỆC    \n";
    cout << "\t\t\t____________________________\n\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}

void wait()
{
    cout << "Nhấn phím bất kỳ để tiếp tục... ";
    _getch();
}

void cleanScreen()
{
    cout << "\x1B[2J\x1B[H";
}

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

struct Date
{
    int d;
    int m;
    int y;
    Date(int dd, int mm, int yyyy) : d(dd), m(mm), y(yyyy) {}
};

struct Task
{
    string title;
    string description;
    Date *pDeadline;
    bool completed;
};

struct Node
{
    Task task;
    Node *next;
};
Node *createNode(Task task)
{
    Node *p = new Node;
    if (p == NULL)
    {
        cout << "Khong du bo nho.\n";
        return NULL;
    }
    p->task = task;
    p->next = NULL;
    return p;
}

struct TaskLList
{
    Node *pHead;
    Node *pTail;
    TaskLList() : pHead(nullptr), pTail(nullptr) {}
};

struct User
{
    string username;
    string password;
    TaskLList tasks;
    User(string u, string pw) : username(u), password(pw) {}
};

struct TNode
{
    User user;
    TNode *left;
    TNode *right;
    int height;
    TNode(User u) : user(u), left(nullptr), right(nullptr), height(1) {}
};
typedef TNode *Tree;
Tree users;

void displayTasks(TaskLList li);
void displayStatus(bool completed);
void addLast(TaskLList &stack, Task task);
void home(User *currentUser);

void initTree(Tree &root)
{
    root = nullptr;
}

int maxNum(int a, int b)
{
    return (a > b) ? a : b;
}

int getHeight(TNode *root)
{
    if (root == nullptr)
        return 0;
    else
        return root->height;
}

TNode *rotateRight(TNode *root)
{
    TNode *x = root->left;
    TNode *xR = x->right;

    x->right = root;
    root->left = xR;

    x->height = maxNum(getHeight(x->left), getHeight(x->right)) + 1;

    if (xR != nullptr)
        xR->height = maxNum(getHeight(xR->left), getHeight(xR->right)) + 1;

    return x;
}

TNode *rotateLeft(TNode *root)
{
    TNode *x = root->right;
    TNode *xL = x->left;

    x->left = root;
    root->right = xL;

    x->height = maxNum(getHeight(x->left), getHeight(x->right)) + 1;
    if (xL != nullptr)
        xL->height = maxNum(getHeight(xL->left), getHeight(xL->right)) + 1;

    return x;
}

int getBalance(TNode *root)
{
    if (root == nullptr)
        return 0;
    return getHeight(root->left) - getHeight(root->right);
}

void displayDate(Date *date)
{
    if (date)
        cout << date->d << "/" << date->m << "/" << date->y;
    else
        cout << constants.notFound;
}

void displayTask(Task task)
{
    cout << "\t\t\t Title: " << task.title << endl;
    cout << "\t\t\t Deadline: ";
    displayDate(task.pDeadline);
    cout << "\n\t\t\t Description: " << task.description << endl;
    cout << "\t\t\t ";
    displayStatus(task.completed);
    cout << "\t\t\t      ------\n";
}

bool compareDate(Date *pDate1, Date *pDate2)
{
    if (pDate1->y != pDate2->y)
        return pDate1->y < pDate2->y;
    if (pDate1->m != pDate2->m)
        return pDate1->m < pDate2->m;
    else
        return pDate1->d < pDate2->d;
}

void quickSortTasks(TaskLList &li);

void addLast(TaskLList &li, Task task)
{
    Node *p = createNode(task);
    if (li.pHead == NULL)
    {
        li.pHead = p;
        li.pTail = p;
    }
    else
    {
        li.pTail->next = p;
        li.pTail = p;
    }
}

void editTask(User *currentUser, Task &task)
{
    int editOption = 0;
    while (editOption != 5)
    {
        string title, des;
        cout << "\t\t\t|___1. Đánh dấu hoàn thành__|\n";
        cout << "\t\t\t|___2. Chỉnh sửa tiêu đề____|\n";
        cout << "\t\t\t|___3. Chỉnh sửa chú thích__|\n";
        cout << "\t\t\t|___4. Chỉnh sửa deadline___|\n";
        cout << "\t\t\t|___5. Trở lại trang chủ____|\n";
        cin >> editOption;
        switch (editOption)
        {
        case 1:
        {
            task.completed = true;
            home(currentUser);
            break;
        }
        case 2:
        {
            cout << "Enter new title: ";
            cin.ignore();
            getline(cin, title);
            task.title = title;
            break;
        }
        case 3:
        {
            cout << "Enter new description: ";
            cin.ignore();
            getline(cin, des);
            task.description = des;
            break;
        }
        case 4:
        {
            cout << "Nhập deadline mới (d m y): ";
            cin >> task.pDeadline->d >> task.pDeadline->m >> task.pDeadline->y;
            break;
        }
        case 5:
        {
            cleanScreen();
            home(currentUser);
            break;
        }
        default:
            cout << constants.invalidChoice << endl;
            break;
        }
    }
}