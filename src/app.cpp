#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
const int MAX = 500;
const string notFound = "Không tồn tại. Hãy tạo mới\n";
const string invalidChoice = "Lỗi. Hãy thử lại\n";
const string usernameExisted = "\t\t\t Tên đăng nhập này đã tồn tại.";

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
    Task *pTask;
    Node *next;
    Node(Task *t) : pTask(t), next(nullptr) {}
};

struct TaskAList
{
    Task arr[MAX];
    int n;
    TaskAList() : n(0) {}
};

struct TaskStack {
    Node *pTop;
    TaskStack() : pTop(nullptr) {}
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

struct UserTree
{
    TreeNode *root;
    UserTree() : root(nullptr) {}
};

UserTree users;
void displayTasks(TaskAList);
void displayStatus(bool completed);
void push(TaskStack &stack, Node *p);
void home(User *currentUser);

void printAppName()
{
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "\t\t\t____________________________\n";
    cout << "\t\t\t                            \n";
    cout << "\t\t\t       QUẢN LÝ CÔNG VIỆC    \n";
    cout << "\t\t\t____________________________\n\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}

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

void displayTasks(TaskAList tasks)
{
    for (int i = 0; i < tasks.n; i++)
    {
        if (i > 4 && i % 5 == 0)
        {
            char ch = 'n';
            cout << "Hiện thêm? (y/n) ";
            cin >> ch;
            if (ch != 'y' && ch != 'Y')
                break;
        }
        cout << "\t\t\t" << i + 1 << ". Title: " << tasks.arr[i].title << endl;
        cout << "\t\t\t   Description: " << tasks.arr[i].description << endl;
        cout << "\t\t\t   Deadline: ";
        displayDate(tasks.arr[i].pDeadline);
        cout << "\n\t\t\t   ";
        displayStatus(tasks.arr[i].completed);
        cout << "\t\t\t        ------\n";
    }
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

void quickSortTasks(TaskAList &tasks, int L, int R) {
    if (L <= R)
    {
        int i = L;
        int j = R;
        int m = (L+R)/2;
        while (i <= j)
        {
            while (compareDate(tasks.arr[i].pDeadline, tasks.arr[m].pDeadline))
                i++;
            while (compareDate(tasks.arr[m].pDeadline, tasks.arr[j].pDeadline))
                j--;

            if (i <= j) {
                Task tmp = tasks.arr[i];
                tasks.arr[i] = tasks.arr[j];
                tasks.arr[j] = tmp;
                i++;
                j--;
            }
        }

        if (j > L) quickSortTasks(tasks, L, j);
        if (i < R) quickSortTasks(tasks, i, R);
    }
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

void addTask(TaskAList &li, Task task)
{
    li.arr[li.n] = task;
    li.n++;
}

void removeTask(TaskAList &li, int pos)
{
    if (pos < 0 || li.n == 0 || pos >= li.n)
        return;
    for (int i = pos; i < li.n - 1; i++)
    {
        li.arr[i] = li.arr[i + 1];
    }
    li.n--;
}

void editTask(User *currentUser, int pos)
{
    int editOption = 0;
    while (editOption != 5)
    {
        string title, des;
        cout << "\t\t\t|___1. Cập nhật tiêu đề____|\n";
        cout << "\t\t\t|___2. Cập nhật chú thích__|\n";
        cout << "\t\t\t|___3. Đánh dấu hoàn thành_|\n";
        cout << "\t\t\t|___4. Thay đổi deadline___|\n";
        cout << "\t\t\t|___5. Trở lại trang chủ___|\n";
        cin >> editOption;
        switch (editOption)
        {
        case 1:
        {
            cout << "Enter new title: ";
            getline(cin, title);
            currentUser->uncompletedTasks.arr[pos].title = title;
            break;
        }
        case 2:
        {
            cout << "Enter new description: ";
            getline(cin, des);
            currentUser->uncompletedTasks.arr[pos].description = des;
            break;
        }
        case 3:
        {
            currentUser->uncompletedTasks.arr[pos].completed = true;
            Task *task = new Task(currentUser->uncompletedTasks.arr[pos]);
            push(currentUser->completedTasks, new Node(task));
            removeTask(currentUser->uncompletedTasks, pos);
            break;
        }
        case 4:
        {
            cout << "Nhập deadline mới (d m y): ";
            cin >> currentUser->uncompletedTasks.arr[pos].pDeadline->d >> currentUser->uncompletedTasks.arr[pos].pDeadline->m >> currentUser->uncompletedTasks.arr[pos].pDeadline->y;
            break;
        }
        case 5:
        {
            home(currentUser);
        }
        default:
            cout << invalidChoice << endl;
            break;
        }
    }
}

void push(TaskStack &stack, Node *p)
{
    p->next = stack.pTop;
    stack.pTop = p;
}

void pop(TaskStack &stack)
{
    if (stack.pTop == nullptr) return;

    Node *p = stack.pTop;
    stack.pTop = stack.pTop->next;
    delete p;
}

void displayTasks(TaskStack stack)
{
    int i = 0;
    for (Node *p = stack.pTop; p != nullptr; p = p->next)
    {
        if (i > 4 && i % 5 == 0)
        {
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

TreeNode *insertUser(TreeNode *root, User req)
{
    if (root == nullptr)
        return new TreeNode(req);

    if (root->user.username == req.username)
    {
        cout << usernameExisted << endl;
        return root;
    }
    if (root->user.username > req.username)
        root->left = insertUser(root->left, req);
    else
        root->right = insertUser(root->right, req);

    return root;    
}

TreeNode *searchOnTree(TreeNode *root, string username)
{
    if (root)
    {
        if (root->user.username == username)
            return root;
        if (root->user.username > username)
            return searchOnTree(root->left, username);
        else
            return searchOnTree(root->right, username);
    }
    return nullptr;
}

User *login(User *currentUser)
{
    string requestUsername;
    string pw;
    cout << "\t\t\tTên đăng nhập: ";
    cin >> requestUsername;
    cout << "\t\t\tMật khẩu: ";
    cin >> pw;
    TreeNode *node = searchOnTree(users.root, requestUsername);
    if (node != nullptr && node->user.password == pw)
    {
        currentUser = &(node->user);
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "\n\t\t\t\t Xin chào " << currentUser->username << endl << endl;
    }
    else
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "\t\t\t Sai thông tin đăng nhập! \n";
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
    return currentUser;
}

void signUp()
{
    string requestUsername;
    string pw;
    cout << "Tên đăng nhập: ";
    cin >> requestUsername;
    cout << "Mật khẩu: ";
    cin >> pw;
    insertUser(users.root, User(requestUsername, pw));
}

User *welcome()
{
    User *currentUser = nullptr;
    int option = -1;

    while (currentUser == nullptr)
    {
        cout << "\t 1. Đăng nhập \t\t 2. Đăng ký \t\t 3.Thoát\n";
        cin >> option;
        switch (option)
        {
        case 1:
            currentUser = login(currentUser);
            break;
        case 2:
            signUp();
            break;
        case 3:
            exit(0);
        default:
            break;
        }
    }
    return currentUser;
}

void editRemoveTask(User *currentUser)
{
    int index = 0;
    int choice = 0;
    while (index <= 0 || index > currentUser->uncompletedTasks.n)
        index = chooseTaskIndex();
    cout << "\t\t\t1. Cập nhật\t\t 2. Xóa\n";
    cin >> choice;
    switch (choice)
    {
    case 1:
        editTask(currentUser, index - 1);
        break;
    case 2:
        removeTask(currentUser->uncompletedTasks, index - 1);
        break;
    default:
        break;
    }
}

void home(User *currentUser)
{
    if (currentUser == nullptr)
        currentUser = welcome();

    while (currentUser)
    {
        // system("cls");
        printAppName();
        cout << "\t\t\t";
        // displayDate(activeDate);
        cout << "\t  " << currentUser->username << endl;
        cout << "\t\t\tCông việc chưa hoàn thành: \n";
        displayTasks(currentUser->uncompletedTasks);
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
            int d, m, y;
            cout << "Tiêu đề: ";
            cin.ignore();
            getline(cin, task.title);
            cout << "Chú thích: ";
            getline(cin, task.description);
            cout << "Deadline (d m y): ";
            cin >> d >> m >> y;
            task.completed = false;
            task.pDeadline = new Date(d, m, y);
            addTask(currentUser->uncompletedTasks, task);
            quickSortTasks(currentUser->uncompletedTasks, 0, currentUser->uncompletedTasks.n-1);
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
            displayTasks(currentUser->completedTasks);
            //41 chon cong viec 42 quay lai
            //411 edit 412 xoa
            break;
        case 5:
        {
            // system("cls");
            printAppName();
            currentUser = welcome();
            break;
        }
        default:
            cout << invalidChoice;
            break;
        }
    }
}

int main()
{
    users.root = insertUser(users.root, User("user1", "111111"));
    users.root = insertUser(users.root, User("user2", "222222"));
    users.root = insertUser(users.root, User("user3", "333333"));

    printAppName();
    User *currentUser = welcome();

    home(currentUser);

    return 0;
}