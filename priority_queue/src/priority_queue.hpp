#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>

#include "exceptions.hpp"

namespace sjtu {
/**
 * @brief a container like std::priority_queue which is a heap internal.
 * **Exception Safety**: The `Compare` operation might throw exceptions for
 * certain data. In such cases, any ongoing operation should be terminated, and
 * the priority queue should be restored to its original state before the
 * operation began.
 */
template <typename T, class Compare = std::less<T>>
class priority_queue {
   private:
    struct Node {
        T data;
        Node* left;
        Node* right;

        Node(const T& data) : data(data), left(nullptr), right(nullptr) {
        }
    };

    Node* root;
    size_t _size;

    void clear(Node*& node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
            node = nullptr;
        }
    }

    void copy(Node*& a, Node* b) {
        if (!b) {
            a = nullptr;
            return;
        }
        a = new Node(b->data);
        copy(a->left, b->left);
        copy(a->right, b->right);
    }

    Node* merge(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;
        if (Compare()(a->data, b->data)) std::swap(a, b);
        a->right = merge(a->right, b);
        std::swap(a->left, a->right);
        return a;
    }

   public:
    /**
     * @brief default constructor
     */
    priority_queue() : root(nullptr), _size(0) {
    }

    /**
     * @brief copy constructor
     * @param other the priority_queue to be copied
     */
    priority_queue(const priority_queue& other) {
        root = nullptr;
        _size = other._size;
        copy(root, other.root);
    }

    /**
     * @brief deconstructor
     */
    ~priority_queue() {
        clear(root);
    }

    /**
     * @brief Assignment operator
     * @param other the priority_queue to be assigned from
     * @return a reference to this priority_queue after assignment
     */
    priority_queue& operator=(const priority_queue& other) {
        if (this == &other) return *this;
        clear(root);
        _size = other._size;
        copy(root, other.root);
        return *this;
    }

    /**
     * @brief get the top element of the priority queue.
     * @return a reference of the top element.
     * @throws container_is_empty if empty() returns true
     */
    const T& top() const {
        if (empty()) throw container_is_empty();
        return root->data;
    }

    /**
     * @brief push new element to the priority queue.
     * @param e the element to be pushed
     */
    void push(const T& e) {
        root = merge(root, new Node(e));
        _size++;
    }

    /**
     * @brief delete the top element from the priority queue.
     * @throws container_is_empty if empty() returns true
     */
    void pop() {
        if (empty()) throw container_is_empty();
        Node* tmp = root;
        root = merge(root->left, root->right);
        delete tmp;
        _size--;
    }

    /**
     * @brief return the number of elements in the priority queue.
     * @return the number of elements.
     */
    size_t size() const {
        return _size;
    }

    /**
     * @brief check if the container is empty.
     * @return true if it is empty, false otherwise.
     */
    bool empty() const {
        return _size == 0;
    }

    /**
     * @brief merge another priority_queue into this one.
     * The other priority_queue will be cleared after merging.
     * The complexity is at most O(logn).
     * @param other the priority_queue to be merged.
     */
    void merge(priority_queue& other) {
        root = merge(root, other.root);
        _size += other._size;
        other.root = nullptr;
        other._size = 0;
    }
};

}  // namespace sjtu

#endif