#include <iostream>
#include <string>
#include <ctime>
#include <windows.h>
#include <chrono>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
const int MAX = 500;
const string notFound = "Không tồn tại. Hãy tạo mới\n";
const string invalidChoice = "Lỗi. Hãy thử lại\n";
string backUrl = "";
CONSOLE_FONT_INFOEX originalFontInfo;

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

struct Node {
    string url;
    Node *next;
    Node(string u): url(u), next(nullptr) {}
};

class LinkedStack {
    private:
        Node *pTop;
    
    public:
        LinkedStack(): pTop(nullptr) {}

        void push(Node *p) {
            p->next = pTop;
            pTop = p;
        }
        void pop() {
            Node *tp = pTop;
            pTop = pTop->next;
            delete tp;
        }
        string top() {
            return pTop->url;
        }
};

struct Task
{
    string title;
    string description;
    int priorityLevel;
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
                    if (i > 4) {
                        char ch = 'n'; 
                        cout << "Hiện thêm? (y/n) ";
                        cin >> ch;
                        if (ch != 'y' && ch != 'Y')
                            break;
                    }
                    cout << "\t\t\t" << i + 1 << ". Title: " << arr[i].title << endl;
                    cout << "\t\t\t   Description: " << arr[i].description << endl;
                    cout << "\t\t\t   Mức độ ưu tiên: " << (arr[i].priorityLevel == 1 ? "Cao" : arr[i].priorityLevel == 2 ? "Trung bình" : "Thấp") << endl;
                    cout << "\t\t\t   ";
                    displayStatus(arr[i].completed);
                    cout << "\t\t\t        ------\n";
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

        void editTask(User *currentUser, Date *activeDate, int pos)
        {
            char editOption;
            while (editOption != 'e')
            {
                    string title, des;
                    cout << "a. Update title\n";
                    cout << "b. Update description\n";
                    cout << "c. Change status\n";
                    cout << "d. Cập nhật thứ tự ưu tiên\n";
                    cout << "e. Trở lại\n";
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
                        if (arr[pos].completed)
                            arr[pos].completed = false;
                        else
                            arr[pos].completed = true;
                        break;
                    }
                    case 'd':
                    {
                        int level = 0;
                        cout << "Mức độ ưu tiên (1:cao/ 2:trung bình/ 3:thấp): ";
                        cin >> level;
                        if (level > 0 && level < 4)
                            arr[pos].priorityLevel = level;
                        break;
                    }
                    case 'e':
                    {
                        backUrl = undo.top();
                        undo.pop();
                        redo.push(new Node(backUrl));
                        back(currentUser, activeDate);
                    }
                    default:
                        cout << invalidChoice << endl;
                        break;
                    }
            }
        }

    private:
        void displayStatus(bool completed) {
            if (completed) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                cout << "Đã hoàn thành\n";
            } else {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                cout << "Chưa hoàn thành\n";
            }
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
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
UserTree users;
LinkedStack undo;
LinkedStack redo;

void home(User *currentUser, Date *activeDate);

void setTextBold()
{
        originalFontInfo.cbSize = sizeof(originalFontInfo);
        CONSOLE_FONT_INFOEX fontInfo;
        fontInfo.cbSize = sizeof(fontInfo);
        GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &fontInfo);

        fontInfo.FontWeight = FW_BOLD; // Set to bold
        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), false, &fontInfo);
}

Date *getDate(time_t time)
{
    tm *localTime = localtime(&time);
    Date *ptoday = new Date(localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900);
    return ptoday;
}


void displayDate(Date *date)
{
    if (date)
        cout << date->d << "/" << date->m << "/" << date->y;
    else
        cout << notFound;
}

void printAppName() {
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    cout << "\t\t\t____________________________\n";
    cout << "\t\t\t                            \n";
    cout << "\t\t\t       QUẢN LÝ CÔNG VIỆC    \n";
    cout << "\t\t\t____________________________\n\n";
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}

void back(User *currentUser, Date *activeDate) {
    backUrl = undo.top();
    undo.pop();
    redo.push(new Node(backUrl));

    if (backUrl == "home")
        home(currentUser, activeDate);
    else if (backUrl == "login")
        loginApp();
    else if (backUrl == "editremovetask")
        editRemoveTask(currentUser, activeDate);
}

User *loginApp() {
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

void home(User * currentUser, Date *activeDate)
{
    Node *pHome = new Node("home");
    undo.push(pHome);
    if (currentUser == nullptr)
        currentUser = loginApp();

    while (currentUser)
    {
        // system("cls");
        printAppName();
        cout << "\t\t\t";
        displayDate(activeDate);
        cout << "\t  " << currentUser->username << endl;
        cout << "\t\t\tCông việc: \n";
        activeDate->tasks.displayTasks();
        cout << "\t\t\tMenu: \n";
        cout << "\t\t\t ___________________________\n";
        cout << "\t\t\t|                           |\n";
        cout << "\t\t\t|   1. Xem ngày khác        |\n";
        cout << "\t\t\t|   2. Thêm công việc       |\n";
        cout << "\t\t\t|   3. Chọn công việc       |\n"; // chỉnh sửa or xóa
        cout << "\t\t\t|   4. Tìm công việc        |\n";
        cout << "\t\t\t|   4. Sắp xếp              |\n"; // 41 theo thứ tự ưu tiên ; 42 theo trình trạng; 43 theo thứ tự ưu tiên và theo tình trạng
        cout << "\t\t\t|   5. Ngày tiếp theo       |\n";
        cout << "\t\t\t|   6. Ngày phía trước      |\n";
        cout << "\t\t\t|   7. Đăng xuất            |\n";
        cout << "\t\t\t|___________________________|\n";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int d, m, y;
            cout << "Nhập ngày tháng năm (d m y): ";
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
            cout << "Tiêu đề: ";
            cin.ignore();
            getline(cin, task.title);
            cout << "Chú thích: ";
            getline(cin, task.description);
            cout << "Mức độ ưu tiên (1:cao/ 2:trung bình/ 3:thấp): ";
            cin >> task.priorityLevel;
            task.completed = false;
            activeDate->tasks.addTask(task);
            break;
        }
        case 3:
        {
            undo.push(new Node("editremovetask"));
            editRemoveTask(currentUser, activeDate);
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
        case 5:
            if (activeDate->next)
                    activeDate = activeDate->next;
            else
                    cout << notFound;
            break;
        case 6:
            if (activeDate->prev)
                    activeDate = activeDate->prev;
            else
                    cout << notFound;
            break;
        case 7:
        {
            while (currentUser->dates.head)
            {
                    Date *temp = currentUser->dates.head;
                    currentUser->dates.head = currentUser->dates.head->next;
                    delete temp;
            }
            // system("cls");
            printAppName();
            currentUser = loginApp();
            break;
        }
        default:
            cout << invalidChoice;
            break;
        }
    }
}

void editRemoveTask(User *currentUser, Date *activeDate) {
    int index = 0;
    int choice = 0;
    while (index == 0 || index > activeDate->tasks.n)
        index = chooseTaskIndex();
    cout << "31. Cập nhật\t\t 32. Xóa\n";
    cin >> choice;
    switch (choice)
    {
    case 31:
        activeDate->tasks.editTask(currentUser, activeDate, index - 1);
        break;
    case 32:
        activeDate->tasks.removeTask(index - 1);
    default:
        break;
    }
}

int main()
{
    users.insert(User("user1", "111111"));
    users.insert(User("user2", "222222"));
    users.insert(User("user3", "333333"));

    printAppName();
    User *currentUser = loginApp();

    Date *activeDate = currentUser->dates.tail;
    if (activeDate == nullptr)
    {
        activeDate = getDate(time(nullptr));
        currentUser->dates.addDate(activeDate);
    }

    home(currentUser, activeDate);

    return 0;
}