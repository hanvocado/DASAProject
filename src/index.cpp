#include <bits/stdc++.h>
using namespace std;

const int MAX = 100;

struct Task {
    string title;
    string description;
    bool completed;
};

struct TaskList {
    Task arr[MAX];
    int n;
};

struct Date {
    int day;
    int month;
    int year;
    Date *next;
    Date *prev;
    TaskList tasks;
};

struct DateList {
    Date *Head;
    Date *Tail;
};

void initDateList(DateList &dates) {
    dates.Head = NULL;
    dates.Tail = NULL;
}

void initDate(Date *pDate, int d, int m, int y) {
    pDate->day = d;
    pDate->month = m;
    pDate->year = y;
    pDate->next = NULL;
    pDate->prev = NULL;
    pDate->tasks.n = 0;
}

Date *findDate(DateList dates,int day, int month, int year) {
    Date *date = dates.Tail;
    while (date) {
        if (date->day == day && date->month == month && date->year == year)
            return date;
        date = date->prev;
    }
    return NULL;
}

void addDate(DateList &li, Date *newDate) {
    if (li.Head == NULL) {
        li.Head = newDate;
        li.Tail = newDate;
        return;
    }

    Date *current = li.Tail;
    while (current) {
        if (current->year > newDate->year || (current->year == newDate->year && current->month > newDate->month) ||
            (current->year == newDate->year && current->month == newDate->month && current->day > newDate->day))
        {
            current = current->prev;
        } else {
            // Insert after current
            newDate->next = current->next;
            newDate->prev = current;
            current->next = newDate;
            if (newDate->next)
                newDate->next->prev = newDate;
            else
                li.Tail = newDate;
            return;
        }
    }

    newDate->next = li.Head;
    li.Head->prev = newDate;
    li.Head = newDate;
}

Date *getTomorrow(Date *givenDate)
{
    return (givenDate && givenDate->next) ? givenDate->next : nullptr;
}

Date *getYesterday(Date *givenDate)
{
    return (givenDate && givenDate->prev) ? givenDate->prev : nullptr;
}

void displayDate(Date *date) {
    if (date)
        cout << date->day << "/" << date->month << "/" << date->year;
    else
        cout << "Not found.";
    cout << endl;
}

void displayTasks(TaskList tasks) {
    for (int i = 0; i < tasks.n; i++) {
        cout << i + 1 << ". Title: " << tasks.arr[i].title << endl;
        cout << "   Description: " << tasks.arr[i].description << endl;
        cout << "   Completed: " << tasks.arr[i].completed << endl;
        cout << "        ------\n";
    }
}

void addTask(Date *date, Task task) {
    date->tasks.arr[date->tasks.n] = task;
    date->tasks.n++;
}

void editTask(Task &task) {
    char editOption;
    int flag = 1;
    while (flag)
    {
        string title, des;
        cout << "a. Update title\n";
        cout << "b. Update description\n";
        cout << "c. Change status\n";
        cout << "d. Back to home\n";
        cin >> editOption;
        cin.ignore();
        switch (editOption)
        {
            case 'a':
            {
                cout << "Enter new title: ";
                getline(cin, title);
                task.title = title;
                break;
            }
            case 'b':
            {
                cout << "Enter new description: ";
                getline(cin, des);
                task.description = des;
                break;
            }
            case 'c':
            {
                if(task.completed)
                    task.completed = false;
                else
                    task.completed = true;
                break;
            }
            case 'd':
            {
                flag = 0;
                break;
            }
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }
}

void removeTask(TaskList &tasks, int pos) {
    if (pos < 0 || tasks.n == 0 || pos >= tasks.n) return;
    for (int i = pos; i < tasks.n - 1; i++) {
        tasks.arr[i] = tasks.arr[i+1];
    }
    tasks.n--;
}

int chooseTaskIndex() {
    int index;
    cout << "Enter task index: ";
    cin >> index;
    return index;
}

//undo redo vs stack

int main()
{
    DateList dates;
    initDateList(dates);
    Date *activeDate = new Date;
    initDate(activeDate, 0, 0, 0);

    while (true)
    {
        cout << "To-Do Manager" << endl;
        cout << "Date: ";
        displayDate(activeDate);
        cout<< "Tasks: \n";
        displayTasks(activeDate->tasks);
        cout << "Options:" << endl;
        cout << "1. Change/Add date" << endl;
        cout << "2. Add task" << endl;
        cout << "3. Edit task" << endl;
        cout << "4. Remove task" << endl;
        cout << "5. Move to Next Day" << endl;
        cout << "6. Move to Previous Day" << endl;
        cout << "7. Exit\n\n" << endl;

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int d, m, y;
            cout << "Enter day month year (16 9 2023): ";
            cin >> d >> m >> y;
            Date *date = findDate(dates, d, m, y);
            if (date) {
                activeDate = date;
            }
            else {
                Date *newDate = new Date;
                initDate(newDate, d, m, y);
                addDate(dates, newDate);
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
            addTask(activeDate, task);
            break;
        }
        case 3:
        {
            int index = 0;
            while (index == 0 || index > activeDate->tasks.n)
                index = chooseTaskIndex();
            editTask(activeDate->tasks.arr[index - 1]);
            break;
        }
        case 4:
        {
            int index = 0;
            while (index == 0 || index > activeDate->tasks.n)
                index = chooseTaskIndex();
            Task task = activeDate->tasks.arr[index - 1];
            removeTask(activeDate->tasks, index-1);
            break;
        }
        case 5:
            if (activeDate->next)
                activeDate = activeDate -> next;
            else
                cout << "Not found\n";
            break;
        case 6:
            if (activeDate->prev)
                activeDate = activeDate->prev;
            else
                cout << "Not found\n";
            break;
        case 7:
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }

    // Clean up - remember to free memory
    while (dates.Head)
    {
        Date *temp = dates.Head;
        dates.Head = dates.Head->next;
        delete temp;
    }

    return 0;
}
