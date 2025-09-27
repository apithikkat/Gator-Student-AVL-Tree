#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
using namespace std;


class TreeNode {
public:
    string student_name;
    string id;
    int height;
    int balanceFactor;
    TreeNode* left;
    TreeNode* right;

    TreeNode(string x, string name)
            : id(x), student_name(name), height(1), balanceFactor(0), left(nullptr), right(nullptr) {}
};

// Simple getheight function from notes
int getHeight(TreeNode* node) {
    return (node == nullptr) ? 0 : node->height;
}

// Calculates balance factor
int getBalanceFactor(TreeNode* node) {
    return (node == nullptr) ? 0 : getHeight(node->left) - getHeight(node->right);
}

// Right Rotation (LL Case)
TreeNode* rightRotate(TreeNode* root) {
    TreeNode* newroot = root->left;
    TreeNode* newchild = newroot->right;

    newroot->right = root;
    root->left = newchild;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    newroot->height = 1 + max(getHeight(newroot->left), getHeight(newroot->right));

    root->balanceFactor = getBalanceFactor(root);
    newroot->balanceFactor = getBalanceFactor(newroot);

    return newroot;
}

// Left Rotation (RR Case)
TreeNode* leftRotate(TreeNode* root) {
    TreeNode* newroot = root->right;
    TreeNode* newchild = newroot->left;

    newroot->left = root;
    root->right = newchild;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    newroot->height = 1 + max(getHeight(newroot->left), getHeight(newroot->right));

    root->balanceFactor = getBalanceFactor(root);
    newroot->balanceFactor = getBalanceFactor(newroot);

    return newroot;
}

// Insert node with checking balance factor and rebalancing
// From class notes
TreeNode* insert(TreeNode* root, string key, string name) {
    if (root == nullptr)
        return new TreeNode(key, name);

    if (key < root->id)
        root->left = insert(root->left, key, name);
    else if (key > root->id)
        root->right = insert(root->right, key, name);
    else
        return root; // No duplicates allowed

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    root->balanceFactor = getBalanceFactor(root);

    if (root->balanceFactor > 1 && key < root->left->id)
        return rightRotate(root);

    if (root->balanceFactor < -1 && key > root->right->id)
        return leftRotate(root);

    if (root->balanceFactor > 1 && key > root->left->id) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (root->balanceFactor < -1 && key < root->right->id) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

//Used for finding inorder successor
TreeNode* getMinNode(TreeNode* root) {
    while (root->left != nullptr)
        root = root->left;
    return root;
}

// Delete a node from the AVL Tree and replace with inorder successor
TreeNode* deleteNode(TreeNode* root, string key) {
    if (root == nullptr)
        return root;

    if (key < root->id)
        root->left = deleteNode(root->left, key);
    else if (key > root->id)
        root->right = deleteNode(root->right, key);
    else {
        if ((root->left == nullptr) || (root->right == nullptr)) {
            TreeNode* temp = root->left ? root->left : root->right;
            if (temp == nullptr) {
                delete root;
                return nullptr;
            } else {
                *root = *temp;
                delete temp;
            }
        }
            //Replacing deleted node with inorder successor
        else {
            TreeNode* temp = getMinNode(root->right);
            root->id = temp->id;
            root->student_name = temp->student_name;
            root->right = deleteNode(root->right, temp->id);
        }
    }

    if (root == nullptr)
        return root;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    root->balanceFactor = getBalanceFactor(root);

    if (root->balanceFactor > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);
    if (root->balanceFactor > 1 && getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (root->balanceFactor < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);
    if (root->balanceFactor < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}


// Check for duplicate id string before inserting
bool checkDuplicate(TreeNode* root, string id) {
    if (root == nullptr)
        return false;
    if (root->id == id)
        return true;
    if (id < root->id)
        return checkDuplicate(root->left, id);
    return checkDuplicate(root->right, id);
}

// Search ID
TreeNode* searchByID(TreeNode* root, string id) {
    if (root == nullptr || root->id == id)
        return root;
    if (id < root->id)
        return searchByID(root->left, id);
    return searchByID(root->right, id);
}

// Search NAME
void searchByName(TreeNode* root, const string& name, string& output) {
    if (root == nullptr)
        return;

    if (root->student_name == name) {
        output += root->id + "\n";
    }

    //Done in preorder traversal manner so that output will also be in relative preorder
    searchByName(root->left, name, output);

    searchByName(root->right, name, output);
}

//Inorder traverses and outputs, with each element seperated by comma
void storeInOrder(TreeNode* root, string& result, bool& nocomma) {
    if (root == nullptr) return;

    storeInOrder(root->left, result, nocomma);

    if (nocomma == false) result += ", ";
    result += root->student_name;
    nocomma = false;

    storeInOrder(root->right, result, nocomma);
}

// Preorder traverses and outputs, with each element seperated by comma
void storePreOrder(TreeNode* root, string& result, bool& nocomma) {
    if (root == nullptr) return;

    if (nocomma == false) result += ", ";
    result += root->student_name;
    nocomma = false;

    storePreOrder(root->left, result, nocomma);
    storePreOrder(root->right, result, nocomma);
}

// Postorder traverses and outputs, with each element seperated by comma
void storePostOrder(TreeNode* root, string& result, bool& nocomma) {
    if (root == nullptr) return;

    storePostOrder(root->left, result, nocomma);
    storePostOrder(root->right, result, nocomma);

    if (nocomma == false) result += ", ";
    result += root->student_name;
    nocomma = false;
}


//Removes N'th node in inorder traversal
TreeNode* removeInorder(TreeNode* root, int N, string& output, int& count) {
    //Base case
    if (root == nullptr) {
        return root;
    }

    //Starts with left subtree
    root->left = removeInorder(root->left, N, output, count);

    // Current node
    if (count == N) {  // Found the N-th node
        string idToRemove = root->id;
        root = deleteNode(root, idToRemove);
        output += "successful\n";
        count = -1;
        return root;
    }

    if (count >= 0)
        count++;

    //Traverse right subtree
    if (count >= 0)
        root->right = removeInorder(root->right, N, output, count);

    return root;
}


//Checks to makes sure name contains only letters, spaces, and hyphens (Ex. Jeane-Claude is true)
bool isValidName(const string& name) {
    for (char c : name) {
        if (!isalpha(c) && c != ' ' && c != '-')
            return false;
    }
    return true;
}

// Function to check if ID is exactly 8 digits long
bool isValidID(const string& id) {
    if (id.length() == 8 && all_of(id.begin(), id.end(), ::isdigit))
        return true;
    else
        return false;
}


//Processes the 9 commands
void menu(TreeNode* root){
    int numCommands;
    string command;
    string finalOutput = "";
    cin >> numCommands;
    cin.ignore();

    for (int i = 0; i < numCommands; i++) {
        getline(cin, command);


        // Insert Command
        if (command.substr(0, 7) == "insert ") {
            stringstream ss(command);
            string action, name, id;

            ss >> action;
            ss.ignore();
            if (getline(ss, name, '"') && getline(ss, name, '"') && ss >> id) {
                // Chacks to make sure valid name entry. (No special characters)
                if (!isValidName(name)) {
                    finalOutput += "unsuccessful\n";
                    continue;
                }

                //Check if id string only contains numbers and is 8 digits
                if (isValidID(id) == false) {
                    finalOutput += "unsuccessful\n";
                    continue;
                }


                // Checks for duplicate before inserting
                if (checkDuplicate(root, id)) {
                    finalOutput += "unsuccessful\n";
                    continue;
                }

                // Valid entry
                root = insert(root, id, name);
                finalOutput += "successful\n";

            } else
                finalOutput += "unsuccessful\n";
        }


            // Search Name or Search ID
        else if (command.substr(0, 7) == "search ") {
            string query = command.substr(7);

            //Search by ID
            if (all_of(query.begin(), query.end(), ::isdigit)) {
                TreeNode* found = searchByID(root, query);
                if (found)
                    finalOutput += found->student_name + "\n";
                else
                    finalOutput += "unsuccessful\n";
            }
                // Search by Name
            else if (query.front() == '"' && query.back() == '"') {
                string name = query.substr(1, query.length() - 2);  // Remove quotes
                string output = "";
                searchByName(root, name, output);
                if (output.empty())
                    finalOutput += "unsuccessful\n";
                else
                    finalOutput += output;
            }
            else
            {
                finalOutput += "unsuccessful\n";
            }
        }

            //Print Inorder
        else if (command == "printInorder") {
            string result = "";
            bool first = true;
            storeInOrder(root, result, first);
            finalOutput += result + "\n";
        }

            //Print Preorder
        else if (command == "printPreorder") {
            string result = "";
            bool first = true;
            storePreOrder(root, result, first);
            finalOutput += result + "\n";
        }

            //Print Postorder
        else if (command == "printPostorder") {
            string result = "";
            bool first = true;
            storePostOrder(root, result, first);
            finalOutput += result + "\n";
        }

            //Print Level Count (Height)
        else if (command == "printLevelCount") {
            finalOutput += to_string(getHeight(root)) + "\n";
        }

            //Remove Command
        else if (command.substr(0, 7) == "remove ") {
            string id = command.substr(7);
            if (!checkDuplicate(root, id)) {
                finalOutput += "unsuccessful\n";
                continue;
            }

            root = deleteNode(root, id);
            finalOutput += "successful\n";
        }

            //Remove Inorder
        else if (command.substr(0, 13) == "removeInorder") {
            stringstream ss(command);
            string action;
            int N;
            ss >> action >> N;

            if (ss.fail()) {
                finalOutput += "unsuccessful\n";
                continue;
            }

            int count = 0;
            root = removeInorder(root, N, finalOutput, count);
        }

            // Invalid Command
        else {
            finalOutput += "unsuccessful\n";
        }
    }

    cout << finalOutput;
}




int main() {
    TreeNode* root = nullptr;

    menu(root);


    return 0;
}