#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <windows.h>
#include <conio.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
const int MAX = 500;
class Constants
{
public:
    const string notFound = "Không tồn tại. Hãy tạo mới\n";
    const string invalidChoice = "Lỗi. Hãy thử lại\n";
    const string usernameExisted = "Tên đăng nhập này đã tồn tại. Vui lòng chọn tên khác\n";
    const string usernameLabel = "Tên đăng nhập";
    const string pwLabel = "Mật khẩu";
};
Constants constants = Constants();

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

struct TaskStack
{
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

struct TNode
{
    User user;
    TNode *left;
    TNode *right;
    TNode(User u) : user(u), left(nullptr), right(nullptr) {}
};
typedef TNode *Tree;
Tree users;

void displayTasks(TaskAList);
void displayStatus(bool completed);
void push(TaskStack &stack, Node *p);
void home(User *currentUser);

void initTree(Tree &root)
{
    root = nullptr;
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

void wait()
{
    cout << "Nhấn phím bất kỳ để tiếp tục... ";
    _getch();
}

void displayDate(Date *date)
{
    if (date)
        cout << date->d << "/" << date->m << "/" << date->y;
    else
        cout << constants.notFound;
}

int chooseTaskIndex()
{
    int index;
    cout << "Enter task index: ";
    cin >> index;
    return index;
}

bool displayMore(int i)
{
    if (i > 4 && i % 5 == 0)
    {
        char ch;
        cout << "Hiển thị thêm? (y/n)\n";
        cin >> ch;
        if (ch != 'y' && ch != 'Y')
            return false;
        else
            return true;
    }
    return true;
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

void displayTasks(TaskAList tasks)
{
    if (tasks.n <= 0)
        return;

    cout << "\tCông việc chưa hoàn thành: \n";
    for (int i = 0; i < tasks.n; i++)
    {
        if (!displayMore(i))
            return;
        cout << "\t  " << i + 1 << ". ";
        displayDate(tasks.arr[i].pDeadline);
        cout << "  |  " << tasks.arr[i].title << endl;
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

void quickSortTasks(TaskAList &tasks, int L, int R)
{
    if (L <= R)
    {
        int i = L;
        int j = R;
        int m = (L + R) / 2;
        while (i <= j)
        {
            while (compareDate(tasks.arr[i].pDeadline, tasks.arr[m].pDeadline))
                i++;
            while (compareDate(tasks.arr[m].pDeadline, tasks.arr[j].pDeadline))
                j--;

            if (i <= j)
            {
                Task tmp = tasks.arr[i];
                tasks.arr[i] = tasks.arr[j];
                tasks.arr[j] = tmp;
                i++;
                j--;
            }
        }

        if (j > L)
            quickSortTasks(tasks, L, j);
        if (i < R)
            quickSortTasks(tasks, i, R);
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
            currentUser->uncompletedTasks.arr[pos].completed = true;
            Task *task = new Task(currentUser->uncompletedTasks.arr[pos]);
            push(currentUser->completedTasks, new Node(task));
            removeTask(currentUser->uncompletedTasks, pos);
            home(currentUser);
            break;
        }
        case 2:
        {
            cout << "Enter new title: ";
            cin.ignore();
            getline(cin, title);
            currentUser->uncompletedTasks.arr[pos].title = title;
            break;
        }
        case 3:
        {
            cout << "Enter new description: ";
            cin.ignore();
            getline(cin, des);
            currentUser->uncompletedTasks.arr[pos].description = des;
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
            cout << "\x1B[2J\x1B[H";
            home(currentUser);
            break;
        }
        default:
            cout << constants.invalidChoice << endl;
            break;
        }
    }
}

// Task binarySearch(TaskAList li, string t)
// {
//     //sort trên deadline nhung kiem tren title??
//     int L = 0;
//     int R = li.n - 1;
//     string title;
//     int M;
//     transform(t.begin(), t.end(), t.begin(), [](unsigned char c) { return std::tolower(c); });

//     while (L <= R)
//     {
//         M = (L + R) / 2;
//         title = li.arr[M].title;
//         transform(title.begin(), title.end(), title.begin(), [](unsigned char c) { return std::tolower(c); });
//         if (title.find(t) != string::npos)
//             return li.arr[M];
//         else if (title.compare(t) <)
//     }
// }

void linearSearch(User *user, string searchStr)
{
    Task *foundTask = nullptr;
    transform(searchStr.begin(), searchStr.end(), searchStr.begin(), [](unsigned char c)
              { return std::tolower(c); });
    string title;
    for (int i = 0; i < user->uncompletedTasks.n; i++)
    {
        title = user->uncompletedTasks.arr[i].title;
        transform(title.begin(), title.end(), title.begin(), [](unsigned char c)
                  { return std::tolower(c); });

        if (title.find(searchStr) != string::npos)
            displayTask(user->uncompletedTasks.arr[i]);
    }

    for (Node *p = user->completedTasks.pTop; p != nullptr; p = p->next)
    {
        title = p->pTask->title;
        transform(title.begin(), title.end(), title.begin(), [](unsigned char c)
                  { return std::tolower(c); });

        if (title.find(searchStr) != string::npos)
            displayTask(*p->pTask);
    }
}

void push(TaskStack &stack, Node *p)
{
    p->next = stack.pTop;
    stack.pTop = p;
}

void pop(TaskStack &stack)
{
    if (stack.pTop == nullptr)
        return;

    Node *p = stack.pTop;
    stack.pTop = stack.pTop->next;
    delete p;
}

void displayTasks(TaskStack stack)
{
    int i = 0;
    for (Node *p = stack.pTop; p != nullptr; p = p->next)
    {
        if (!displayMore(i))
            return;
        cout << "Tiêu đề: " << p->pTask->title << endl;
        cout << "Chú thích: " << p->pTask->description << endl;
        i++;
    }
}
void replaceWithMostLeft(Tree &root, Tree &p)
{
    if (p->left != nullptr)
        replaceWithMostLeft(root, p->left);
    else
    {
        TNode *temp = root;
        root->user = p->user;
        p = p->right;
        delete temp;
    }
}
void deleteUser(Tree &root, User user)
{
    if (root == nullptr)
        return;
    if (root->user.username > user.username)
        deleteUser(root->left, user);
    else if (root->user.username < user.username)
        deleteUser(root->right, user);
    else
    {
        if (root->left != nullptr && root->right != nullptr)
            replaceWithMostLeft(root, root->right);
        else
        {
            TNode *temp = root;
            if (root->left)
                root = root->left;
            else if (root->right)
                root = root->right;
            delete temp;
        }
    }
}

void insertTNode(Tree &root, User req)
{
    if (root == nullptr)
    {
        root = new TNode(req);
        return;
    }

    if (root->user.username == req.username)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "\t\t\t" << constants.usernameExisted << endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
        return;
    }
    else if (root->user.username > req.username)
        insertTNode(root->left, req);
    else
        insertTNode(root->right, req);
}

TNode *searchOnTree(Tree root, string username)
{
    if (root)
    {
        if (root->user.username == username)
        {
            cout << root->user.username << endl;
            return root;
        }
        if (root->user.username > username)
        {
            cout << root->user.username << endl;
            return searchOnTree(root->left, username);
        }
        else
        {
            cout << root->user.username << endl;
            return searchOnTree(root->right, username);
        }
    }
    return nullptr;
}

User *login(User *currentUser)
{
    string requestUsername;
    string pw;
    cout << "\t\t\t" << constants.usernameLabel << ": ";
    cin >> requestUsername;
    cout << "\t\t\t" << constants.pwLabel << ": ";
    cin >> pw;
    TNode *node = searchOnTree(users, requestUsername);
    if (node != nullptr && node->user.password == pw)
    {
        currentUser = &(node->user);
    }
    else
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        cout << "\t\t\t Sai thông tin đăng nhập! \n";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
    }
    return currentUser;
}

void signUp()
{
    string requestUsername;
    string pw;
    cout << constants.usernameLabel << ": ";
    cin >> requestUsername;
    cout << constants.pwLabel << ": ";
    cin >> pw;
    insertTNode(users, User(requestUsername, pw));
}

User *welcome()
{
    User *currentUser = nullptr;
    int option = -1;

    printAppName();
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
    cout << "\x1B[2J\x1B[H";
    return currentUser;
}

void editRemoveTask(User *currentUser)
{
    int index = 0;
    int choice = 0;
    while (index <= 0 || index > currentUser->uncompletedTasks.n)
        index = chooseTaskIndex();

    cout << "\x1B[2J\x1B[H";
    printAppName();
    displayTask(currentUser->uncompletedTasks.arr[index - 1]);
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
    cout << "\x1B[2J\x1B[H";
}

void home(User *currentUser)
{
    if (currentUser == nullptr)
        currentUser = welcome();

    while (currentUser)
    {
        printAppName();
        displayTasks(currentUser->uncompletedTasks);
        cout << "\n\t\t\tMenu: \n";
        cout << "\t\t\t ___________________________\n";
        cout << "\t\t\t|                           |\n";
        cout << "\t\t\t|   1. Thêm công việc       |\n";
        cout << "\t\t\t|   2. Chọn công việc       |\n";
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
            quickSortTasks(currentUser->uncompletedTasks, 0, currentUser->uncompletedTasks.n - 1);
            cout << "\x1B[2J\x1B[H";
            break;
        }
        case 2:
        {
            editRemoveTask(currentUser);
            break;
        }
        case 3:
        {
            string search, title;
            cout << "Tìm kiếm bằng tiêu đề: ";
            cin.ignore();
            getline(cin, search);
            linearSearch(currentUser, search);
            wait();
            cout << "\x1B[2J\x1B[H";
            break;
        }
        case 4:
        {
            displayTasks(currentUser->completedTasks);
            // 41 chon cong viec 42 quay lai
            // 411 edit 412 xoa
            wait();
            cout << "\x1B[2J\x1B[H";
            break;
        }
        case 5:
        {
            cout << "\x1B[2J\x1B[H";
            currentUser = welcome();
            break;
        }
        default:
        {
            cout << "\x1B[2J\x1B[H";
            cout << constants.invalidChoice;
            break;
        }
        }
    }
}

int main()
{
    initTree(users);
    insertTNode(users, User("user1", "111111"));
    insertTNode(users, User("user2", "222222"));
    insertTNode(users, User("user3", "333333"));
    TNode *user1 = searchOnTree(users, "user1");

    deleteUser(users, user1->user);
    TNode *user = searchOnTree(users, "user1");
    cout << (user == nullptr ? "deleted user1" : user->user.username) << endl;
    wait();
    User *currentUser = welcome();

    home(currentUser);

    return 0;
}
