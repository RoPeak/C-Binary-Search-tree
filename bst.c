#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int value;
    struct node *left;
    struct node *right;
} node_t;

node_t * search(node_t * node, int toFind); // DONE
node_t * createTree(int firstElem);         // DONE
void destroyTree(node_t * node);            // DONE
void insert(node_t * node, int elem);       // DONE
void delete(node_t * node, int elem);       // DONE

node_t * getParentNode(node_t * root, node_t * child); // Outwith exercise specification

node_t * search(node_t * node, int toFind) {
    // Base cases - node is NULL or value is found
    if (node == NULL || node->value == toFind) {
        return node;
    }

    // If value is less than current node's value, search left subtree
    if (toFind < node->value) {
        return search(node->left, toFind);
    }
    // Otherwise, search right subtree
    return search(node->right, toFind);
}

node_t * createTree(int firstElem) {
    node_t *root = malloc(sizeof(node_t)); // Allocate space on heap
    if (!root) { return NULL; } // No space to allocate

    // Create tree from element provided
    root->value = firstElem;
    root->left = NULL;
    root->right = NULL;
    return root;
}

void destroyTree(node_t * node) {
    // Base case  
    if (node == NULL) {
        return;
    }

    // Recursively traverse each tree branch, freeing the nodes
    destroyTree(node->left);
    destroyTree(node->right);
    free(node);
    node = NULL;
}

void insert(node_t * node, int elem) {
    // Only carry out the operation if elem is *not* already in the bst
    // (Prevents duplicates)
    if (search(node, elem) == NULL) {    
        node_t *nodeToAdd = malloc(sizeof(node_t)); // Allocate space on heap
        if (!nodeToAdd) { return; } // No space to allocate --- abandon operation
        nodeToAdd->value = elem; // Initialise node with given value

        // Find the correct place to insert the node
        while (1) {
            if (elem < node->value) {
                // Move to left subtree and insert node ONLY if there is space
                // Otherwise, loop again
                if (node->left == NULL) {
                    node->left = nodeToAdd;
                    break;
                } else {
                    node = node->left;
                }
            } else {
                // Move to right subtree and insert node ONLY if there is space
                // Otherwise, loop again
                if (node->right == NULL) {
                    node->right = nodeToAdd;
                    break;
                } else {
                    node = node->right;
                }
            }
        }
    } else {
        return; // elem is already in bst
    }
}

void delete(node_t * node, int elem) {
    // Find the node to delete
    node_t *nodeToDelete = search(node, elem);
    if (nodeToDelete == NULL) {
        return; // Node not found, nothing to delete
    }

    // Find the parent node of the node to delete
    node_t *parentNode = getParentNode(node, nodeToDelete);

    // Case 1: Node to delete is a leaf node
    if (nodeToDelete->left == NULL && nodeToDelete->right == NULL) {
        if (parentNode == NULL) {
            node = NULL; // Deleting the root node
        } else {
            // Update the parent node's child pointer
            if (parentNode->left == nodeToDelete) {
                parentNode->left = NULL;
            } else {
                parentNode->right = NULL;
            }
        }
        free(nodeToDelete);
        nodeToDelete = NULL;
    }
    // Case 2: Node to delete has only one child
    else if (nodeToDelete->left == NULL || nodeToDelete->right == NULL) {
        // Find the child node
        node_t *childNode = (nodeToDelete->left != NULL) ? nodeToDelete->left : nodeToDelete->right;

        // If the node to delete is the root node, update the root node
        if (parentNode == NULL) {
            // Swapping the child node attributes with the root node attributes
            // Then deleting the child (now root) node
            node->value = childNode->value;
            node->left = childNode->left;
            node->right = childNode->right;
            free(childNode);
            childNode = NULL;
        } else {
            // Update the parent node's child pointer
            if (parentNode->left == nodeToDelete) {
                parentNode->left = childNode;
            } else {
                parentNode->right = childNode;
            }
            free(nodeToDelete);
            nodeToDelete = NULL;
        }
    }
    // Case 3: Node to delete has two children
    else {
        // Find the inorder successor (smallest value in the right subtree)
        node_t *successor = nodeToDelete->right;
        node_t *successorParent = nodeToDelete;
        while (successor->left != NULL) {
            successorParent = successor;
            successor = successor->left;
        }

        // Copy the value of the inorder successor to the node to delete
        nodeToDelete->value = successor->value;

        // Delete the inorder successor
        if (successorParent->left == successor) {
            successorParent->left = successor->right;
        } else {
            successorParent->right = successor->right;
        }
        free(successor);
        successor = NULL;
    }
}

node_t * getParentNode(node_t * root, node_t * child) {
    // Base case: if root is NULL or child is NULL, return NULL
    if (root == NULL || child == NULL) {
        return NULL;
    }

    // If child node is the root node, there is no parent
    if (root == child) {
        return NULL;
    }

    // If child node is in the left subtree of the root
    if (child->value < root->value) {
        // If the left child of the root is the child node, return the root
        if (root->left == child) {
            return root;
        }
        // Recursively search the left subtree
        return getParentNode(root->left, child);
    }

    // If child node is in the right subtree of the root
    if (child->value > root->value) {
        // If the right child of the root is the child node, return the root
        if (root->right == child) {
            return root;
        }
        // Recursively search the right subtree
        return getParentNode(root->right, child);
    }
}