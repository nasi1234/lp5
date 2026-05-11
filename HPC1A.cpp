#include <iostream>
#include <queue>
#include <omp.h>
using namespace std;

// Define a simple tree node
struct Node {
    int data;
    Node *left, *right;
    Node(int value) : data(value), left(NULL), right(NULL) {}
};

// Function to insert nodes into a binary tree
Node* insert(Node* root, int data) {
    if (!root) return new Node(data);
    
    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* temp = q.front();
        q.pop();

        if (!temp->left) {
            temp->left = new Node(data);
            break;
        } else {
            q.push(temp->left);
        }

        if (!temp->right) {
            temp->right = new Node(data);
            break;
        } else {
            q.push(temp->right);
        }
    }
    return root;
}

// Parallel Breadth First Search
void parallelBFS(Node* root) {
    if (!root) return;

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        int size = q.size();

        #pragma omp parallel for
        for (int i = 0; i < size; ++i) {
            Node* curr;

            #pragma omp critical
            {
                curr = q.front();
                q.pop();
                cout << curr->data << " ";
            }

            #pragma omp critical
            {
                if (curr->left) q.push(curr->left);
                if (curr->right) q.push(curr->right);
            }
        }
    }
}

int main() {
    Node* root = NULL;
    int data;
    char choice;

    do {
        cout << "Enter node data: ";
        cin >> data;
        root = insert(root, data);

        cout << "Do you want to add another node? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    double start = omp_get_wtime();
    parallelBFS(root);
    double end = omp_get_wtime();

    cout << "\nExecution Time: " << (end - start) << " seconds\n";

    return 0;
}
