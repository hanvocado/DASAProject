#include <bits/stdc++.h>
using namespace std;

const int MAX = 500;

struct User {
    string username;
    string password;
    User(string u, string pw): username(u), password(pw) {}
};
struct TreeNode {
    User user;
    TreeNode *left;
    TreeNode *right;
    TreeNode(User u): user(u), left(nullptr), right(nullptr) {}
};
class UserTree {
    private:
        TreeNode *root;
    public:
        UserTree() : root(nullptr) {}

        int insert(User user) {
            return insertHelper(root, user);
        }
        User *search(string username) {
            return searchHelper(root, username);
        }
        User *login(string username, string pw) {
            User *user = searchHelper(root, username);
            if (user == nullptr) {
                cout << "search found null\n";
                return nullptr;
            }
            cout << pw.compare(user->password);
            if (pw.compare(user->password) == 0)
                return user;
            return nullptr;
        }
    private:
        int insertHelper(TreeNode *node, User user) {
            if (node != nullptr) {
                if (user.username < node->user.username)
                    return insertHelper(node->left, user);
                else if (user.username > node->user.username)
                    return insertHelper(node->right, user);
                else {
                    cout << "username đã tồn tại.\n";
                    return 0;
                }

            }
            node = new TreeNode(user);
            cout << user.username << user.password;
            if (node == nullptr)
                return 0;
            else
                return 1;
        }

        User *searchHelper(TreeNode *node, string username) {
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

int main() {
    UserTree users;
    users.insert(User("user1", "111111"));
    users.insert(User("user2", "222222"));
    users.insert(User("user3", "333333"));

    User *currentUser = nullptr;
    string requestUser = "";
    string password = "";
    int option = -1;
    int flag = 0;
    while (flag == 0) {
        cout << "1. Đăng nhập\n";
        cout << "2. Đăng ký\n";
        cin >> option;
        switch (option)
        {
        case 1:
        {
            cout << "Nhập tên tài khoản (user1): ";
            cin.ignore();
            getline(cin, requestUser);
            cout << "Nhập mật khẩu (111111): ";
            getline(cin, password);
            User *user = users.login(requestUser, password);
            if (user != nullptr)
            {
                currentUser = user;
                cout << "Đăng nhập thành công";
                flag = 1;
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
}