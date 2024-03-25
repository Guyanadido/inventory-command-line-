#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct Product
{
    int productID;
    string name;
    float price;
    int quantity;
    string category;
    Product *left;
    Product *right;
};

Product *createProduct(int productID, string name, float price, int quantity, string category);
Product *insertProduct(Product *root, int productID, string name, float price, int quantity, string category);
Product *readFromFile(Product *root, string filename);
void writeToFile(Product *root, string filename);
void *findMin(Product *node);
void *findMax(Product *node);
Product *deleteProduct(Product *root, int productID);
Product *searchProduct(Product *root, int productID);
void inorderTraversal(Product *root);
void preorderTraversal(Product *root);
void postorderTraversal(Product *root);
int countProducts(Product *root);

// rotations
Product *singleLeftRotation(Product *root);
Product *singleRightRotation(Product *root);
Product *doubleLeftRotation(Product *root);
Product *doubleRightRotation(Product *root);

int main()
{
    Product *root = nullptr;

    int choice;
    do
    {
        cout << "\nInventory Management System Menu\n";
        cout << "1. Insert product\n";
        cout << "2. Read from file\n";
        cout << "3. Write to file\n";
        cout << "4. Display minimum product\n";
        cout << "5. Display maximum product\n";
        cout << "6. Delete product\n";
        cout << "7. Search product\n";
        cout << "8. Count products\n";
        cout << "9. Inorder display\n";
        cout << "10. preorder display\n";
        cout << "11. postorder display\n";
        cout << "12. perform single right rotation\n";
        cout << "13. perform single left rotation\n";
        cout << "14. perform double right totation\n";
        cout << "15. perform double left rotation\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int productID;
            string name, category;
            float price;
            int quantity;

            cout << "Enter Product ID: ";
            cin >> productID;
            cout << "Enter Product Name: ";
            cin >> name;
            cout << "Enter Price: ";
            cin >> price;
            cout << "Enter Quantity: ";
            cin >> quantity;
            cout << "Enter Category: ";
            cin >> category;

            root = insertProduct(root, productID, name, price, quantity, category);
            break;
        }
        case 2:
            root = readFromFile(root, "inventory.txt");
            break;
        case 3:
            writeToFile(root, "inventory.txt");
            break;
        case 4:
            findMin(root);
            break;
        case 5:
            findMax(root);
            break;
        case 6:
        {
            cout << "enter the product ID to be deleted";
            int n;
            cin >> n;
            deleteProduct(root, n);
            break;
        }
        case 7:
            int productID;
            cout << "Enter the productID";
            cin >> productID;
            searchProduct(root, productID);
            break;
        case 8:
        {
            int numberOfProducts = countProducts(root);
            cout << "Total number of products recorded are " << numberOfProducts;
            break;
        }
        case 9:
            inorderTraversal(root);
            break;
        case 10:
            preorderTraversal(root);
            break;
        case 11:
            postorderTraversal(root);
            break;
        case 12:
            root = singleRightRotation(root);
            break;
        case 13:
            root = singleLeftRotation(root);
            break;
        case 14:
            root = doubleRightRotation(root);
            break;
        case 15:
            root = doubleLeftRotation(root);
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << choice;
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);
    return 0;
}

Product *createProduct(int productID, string name, float price, int quantity, string category)
{
    Product *newProduct = new Product;
    newProduct->productID = productID;
    newProduct->name = name;
    newProduct->price = price;
    newProduct->quantity = quantity;
    newProduct->category = category;
    newProduct->left = nullptr;
    newProduct->right = nullptr;
    return newProduct;
}

Product *insertProduct(Product *root, int productID, string name, float price, int quantity, string category)
{
    Product *newNode = createProduct(productID, name, price, quantity, category);

    if (root == nullptr)
    {
        root = newNode;
        return root;
    }

    Product *current = root;
    Product *parent = nullptr;

    while (current != nullptr)
    {
        parent = current;
        if (productID < current->productID)
        {
            current = current->left;
        }
        else if (productID > current->productID)
        {
            current = current->right;
        }
        else
        {
            cout << "Error: Product with ID " << productID << " already exists.\n";
            delete newNode;
            return root;
        }
    }

    if (productID < parent->productID)
    {
        parent->left = newNode;
    }
    else
    {
        parent->right = newNode;
    }

    return root;
}

Product *readFromFile(Product *root, string filename)
{
    ifstream inFile(filename);
    if (!inFile.is_open())
    {
        cout << "Error: Unable to open file " << filename << " for reading.\n";
        return root;
    }

    int productID;
    string name, category, line;
    float price;
    int quantity;

    while (getline(inFile, line)) // Read line by line
    {
        if (line.find("Product ID:") != string::npos) // Check if the line contains "Product ID:"
        {

            stringstream ss(line.substr(line.find(":") + 1));
            ss >> productID;
        }
        else if (line.find("Name:") != string::npos)
        {
            name = line.substr(line.find(":") + 1);
        }
        else if (line.find("Price:") != string::npos)
        {

            stringstream ss(line.substr(line.find(":") + 1));
            ss >> price;
        }
        else if (line.find("Quantity:") != string::npos)
        {

            stringstream ss(line.substr(line.find(":") + 1));
            ss >> quantity;
        }
        else if (line.find("Category:") != string::npos)
        {

            category = line.substr(line.find(":") + 1);

            root = insertProduct(root, productID, name, price, quantity, category);
        }
    }

    inFile.close();
    return root;
}

void inorderTraversalToFile(Product *node, ofstream &outFile)
{
    if (node == nullptr)
    {
        return;
    }

    inorderTraversalToFile(node->left, outFile);

    outFile << "Product ID:" << node->productID << endl;
    outFile << "Name: " << node->name << endl;
    outFile << "Price: " << node->price << endl;
    outFile << "Quantity: " << node->quantity << endl;
    outFile << "Category: " << node->category << endl
            << endl;

    inorderTraversalToFile(node->right, outFile);
}

void writeToFile(Product *root, string filename)
{
    ofstream outFile(filename);
    if (!outFile.is_open())
    {
        cout << "Error: Unable to open file " << filename << " for writing.\n";
        return;
    }

    inorderTraversalToFile(root, outFile);

    outFile.close();
}

void *findMin(Product *node)
{
    if (node == nullptr)
    {
        cout << "The tree is empty." << endl;
        return nullptr;
    }

    while (node->left != nullptr)
    {
        node = node->left;
    }

    cout << "Product ID:" << node->productID << endl;
    cout << "Name: " << node->name << endl;
    cout << "Price: " << node->price << endl;
    cout << "Quantity: " << node->quantity << endl;
    cout << "Category: " << node->category << endl
         << endl;
}

void *findMax(Product *node)
{
    if (node == nullptr)
    {
        cout << "The tree is empty." << endl;
        return nullptr;
    }

    while (node->right != nullptr)
    {
        node = node->right;
    }

    cout << "Product ID:" << node->productID << endl;
    cout << "Name: " << node->name << endl;
    cout << "Price: " << node->price << endl;
    cout << "Quantity: " << node->quantity << endl;
    cout << "Category: " << node->category << endl
         << endl;
}

Product *deleteProduct(Product *root, int productIDToDelete)
{
    if (root == nullptr)
    {
        cout << "The tree is empty. No deletion possible." << endl;
        return nullptr;
    }

    Product *current = root;
    Product *parent = nullptr;
    bool found = false;
    while (current != nullptr)
    {
        if (productIDToDelete < current->productID)
        {
            parent = current;
            current = current->left;
        }
        else if (productIDToDelete > current->productID)
        {
            parent = current;
            current = current->right;
        }
        else
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "Product with ID " << productIDToDelete << " not found. Deletion failed." << endl;
        return root;
    }

    if (current->left == nullptr && current->right == nullptr)
    {
        if (current == root)
        {
            delete current;
            return nullptr;
        }
        else
        {
            if (parent->left == current)
            {
                parent->left = nullptr;
            }
            else
            {
                parent->right = nullptr;
            }
            delete current;
            return root;
        }
    }

    if (current->left == nullptr || current->right == nullptr)
    {
        Product *child = (current->left != nullptr) ? current->left : current->right;
        if (current == root)
        {
            delete current;
            return child;
        }
        else
        {
            if (parent->left == current)
            {
                parent->left = child;
            }
            else
            {
                parent->right = child;
            }
            delete current;
            return root;
        }
    }

    Product *successor = current->right;
    while (successor->left != nullptr)
    {
        successor = successor->left;
    }

    current->productID = successor->productID;
    current->name = successor->name;
    current->price = successor->price;
    current->quantity = successor->quantity;
    current->category = successor->category;

    root = deleteProduct(root, successor->productID);

    return root;
}

Product *searchProduct(Product *root, int productID)
{
    if (root == nullptr)
    {
        cout << "The tree is empty." << endl;
        return nullptr;
    }

    Product *current = root;
    while (current != nullptr)
    {
        if (productID == current->productID)
        {
            cout << "Product Found:" << endl;
            cout << "Product ID: " << current->productID << endl;
            cout << "Name: " << current->name << endl;
            cout << "Price: " << current->price << endl;
            cout << "Quantity: " << current->quantity << endl;
            cout << "Category: " << current->category << endl;
            return current;
        }
        else if (productID < current->productID)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    cout << "Product with ID " << productID << " not found." << endl;
    return nullptr;
}

void inorderTraversal(Product *root)
{
    if (root == nullptr)
        return;

    inorderTraversal(root->left);
    cout << "Product ID: " << root->productID << ", Name: " << root->name << ", Price: " << root->price << ", Quantity: " << root->quantity << ", Category: " << root->category << endl;
    inorderTraversal(root->right);
}

void preorderTraversal(Product *root)
{
    if (root == nullptr)
        return;

    cout << "Product ID: " << root->productID << ", Name: " << root->name << ", Price: " << root->price << ", Quantity: " << root->quantity << ", Category: " << root->category << endl;
    preorderTraversal(root->left);
    preorderTraversal(root->right);
}

void postorderTraversal(Product *root)
{
    if (root == nullptr)
        return;

    postorderTraversal(root->left);
    postorderTraversal(root->right);
    cout << "Product ID: " << root->productID << ", Name: " << root->name << ", Price: " << root->price << ", Quantity: " << root->quantity << ", Category: " << root->category << endl;
}

int countProducts(Product *root)
{
    if (root == nullptr)
    {
        return 0;
    }

    int countLeft = countProducts(root->left);
    int countRight = countProducts(root->right);

    cout << 1;
    return 1 + countLeft + countRight;
}

Product *singleLeftRotation(Product *root)
{
    if (root == nullptr || root->right == nullptr)
    {
        return root;
    }

    Product *newRoot = root->right;
    root->right = newRoot->left;
    newRoot->left = root;
    return newRoot;
}

Product *singleRightRotation(Product *root)
{
    if (root == nullptr || root->left == nullptr)
    {
        return root;
    }

    Product *newRoot = root->left;
    root->left = newRoot->right;
    newRoot->right = root;
    return newRoot;
}

Product *doubleLeftRotation(Product *root)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    root->right = singleRightRotation(root->right);
    return singleLeftRotation(root);
}

Product *doubleRightRotation(Product *root)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    root->left = singleLeftRotation(root->left);
    return singleRightRotation(root);
}
