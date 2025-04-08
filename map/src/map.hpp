/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <cstddef>
#include <functional>

#include "exceptions.hpp"
#include "utility.hpp"

namespace sjtu {

template <class Key, class T, class Compare = std::less<Key> >
class map {
   public:
    /**
     * the internal type of data.
     * it should have a default constructor, a copy constructor.
     * You can use sjtu::map as value_type by typedef.
     */
    typedef pair<const Key, T> value_type;
    /**
     * see BidirectionalIterator at CppReference for help.
     *
     * if there is anything wrong throw invalid_iterator.
     *     like it = map.begin(); --it;
     *       or it = map.end(); ++end();
     */

   private:
    struct Node {
        value_type data;
        size_t height;
        Node *left;
        Node *right;

        Node(const value_type &data, Node *left = nullptr,
             Node *right = nullptr, int height = 1)
            : data(data), height(height), left(left), right(right) {
        }

        Node(const Node &other)
            : data(other.data),
              height(other.height),
              left(other.left),
              right(other.right) {
        }

        Node &operator=(const Node &other) {
            if (this != &other) {
                data = other.data;
                height = other.height;
                left = other.left;
                right = other.right;
            }
            return *this;
        }

        ~Node() {
            delete left;
            delete right;
        }

       private:
        Node *balance() {
            int factor = get_balance();
            if (factor > 1) {
                if (left->get_balance() > 0) {
                    return LL(this);
                } else {
                    return LR(this);
                }
            } else if (factor < -1) {
                if (right->get_balance() < 0) {
                    return RR(this);
                } else {
                    return RL(this);
                }
            }
            if (left) {
                left->height = 1 + std::max(left->left ? left->left->height : 0,
                                            left->right ? left->right->height : 0);
            }
            if (right) {
                right->height = 1 + std::max(right->left ? right->left->height : 0,
                                             right->right ? right->right->height : 0);
            }
            height = 1 + std::max(left ? left->height : 0,
                                  right ? right->height : 0);
            return this;
        }

        int get_balance() {
            return (left ? left->height : 0) - (right ? right->height : 0);
        }

        Node *RR(Node *&t) {
        }

        Node *LL(Node *&t) {
        }

        Node *LR(Node *&t) {
        }

        Node *RL(Node *&t) {
        }
    };

    Node *root;
    size_t node_count;

    Node *insert(const value_type &value, Node *&node) {
        if (Compare()(value.first, node->data.first)) {
            if (node->left == nullptr) {
                node->left = new Node(value);
            } else {
                node->left = node->left->insert(value);
            }
        } else if (Compare()(node->data.first, value.first)) {
            if (node->right == nullptr) {
                node->right = new Node(value);
            } else {
                node->right = node->right->insert(value);
            }
        } else {
            return nullptr;  // already exists
        }

        node->height = 1 + std::max(node->left ? node->left->height : 0,
                                    node->right ? node->right->height : 0);
        return node->balance();
    }

    Node *find_node(Node *node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node *erase(const Key &key, Node *&node) {
        if (node == nullptr) {
            return nullptr;
        }
        if (Compare()(key, node->data.first)) {
            node->left = erase(key, node->left);
        } else if (Compare()(node->data.first, key)) {
            node->right = erase(key, node->right);
        } else {
            if (node->left == nullptr) {
                Node *temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node *temp = node->left;
                delete node;
                return temp;
            } else {
                // two children
                // TODO
            }
        }
        return node->balance();
    }

    void deep_copy(Node *&this_node, Node *other_node) {
        if (other_node == nullptr) {
            this_node = nullptr;
            return;
        }
        this_node = new Node(other_node->data);
        deep_copy(this_node->left, other_node->left);
        deep_copy(this_node->right, other_node->right);
    }

   public:
    class const_iterator;
    class iterator {
       private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
       public:
        iterator() {
            // TODO
        }

        iterator(const iterator &other) {
            // TODO
        }

        /**
         * TODO iter++
         */
        iterator operator++(int) {
        }

        /**
         * TODO ++iter
         */
        iterator &operator++() {
        }

        /**
         * TODO iter--
         */
        iterator operator--(int) {
        }

        /**
         * TODO --iter
         */
        iterator &operator--() {
        }

        /**
         * a operator to check whether two iterators are same (pointing to the
         * same memory).
         */
        value_type &operator*() const {
        }

        bool operator==(const iterator &rhs) const {
        }

        bool operator==(const const_iterator &rhs) const {
        }

        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
        }

        bool operator!=(const const_iterator &rhs) const {
        }

        /**
         * for the support of it->first.
         * See
         * <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/>
         * for help.
         */
        value_type *operator->() const noexcept {
        }
    };
    class const_iterator {
        // it should has similar member method as iterator.
        //  and it should be able to construct from an iterator.
       private:
        // data members.
       public:
        const_iterator() {
            // TODO
        }

        const_iterator(const const_iterator &other) {
            // TODO
        }

        const_iterator(const iterator &other) {
            // TODO
        }
        // And other methods in iterator.
        // And other methods in iterator.
        // And other methods in iterator.
    };

    /**
     * TODO two constructors
     */
    map() {
        root = nullptr;
        node_count = 0;
    }

    map(const map &other) {
        root = nullptr;
        node_count = 0;
        deep_copy(root, other.root);
        node_count = other.node_count;
    }

    /**
     * TODO assignment operator
     */
    map &operator=(const map &other) {
        if (this != &other) {
            delete root;
            root = nullptr;
            node_count = 0;
            deep_copy(root, other.root);
            node_count = other.node_count;
        }
        return *this;
    }

    /**
     * TODO Destructors
     */
    ~map() {
        delete root;
    }

    /**
     * TODO
     * access specified element with bounds checking
     * Returns a reference to the mapped value of the element with key
     * equivalent to key. If no such element exists, an exception of type
     * `index_out_of_bound'
     */
    T &at(const Key &key) {
    }

    const T &at(const Key &key) const {
    }

    /**
     * TODO
     * access specified element
     * Returns a reference to the value that is mapped to a key equivalent to
     * key, performing an insertion if such key does not already exist.
     */
    T &operator[](const Key &key) {
    }

    /**
     * behave like at() throw index_out_of_bound if such key does not exist.
     */
    const T &operator[](const Key &key) const {
    }

    /**
     * return a iterator to the beginning
     */
    iterator begin() {
    }

    const_iterator cbegin() const {
    }

    /**
     * return a iterator to the end
     * in fact, it returns past-the-end.
     */
    iterator end() {
    }

    const_iterator cend() const {
    }

    /**
     * checks whether the container is empty
     * return true if empty, otherwise false.
     */
    bool empty() const {
        return node_count == 0;
    }

    /**
     * returns the number of elements.
     */
    size_t size() const {
        return node_count;
    }

    /**
     * clears the contents
     */
    void clear() {
        delete root;
        root = nullptr;
        node_count = 0;
    }

    /**
     * insert an element.
     * return a pair, the first of the pair is
     *   the iterator to the new element (or the element that prevented the
     * insertion), the second one is true if insert successfully, or false.
     */
    pair<iterator, bool> insert(const value_type &value) {
    }

    /**
     * erase the element at pos.
     *
     * throw if pos pointed to a bad element (pos == this->end() || pos points
     * an element out of this)
     */
    void erase(iterator pos) {
    }

    /**
     * Returns the number of elements with key
     *   that compares equivalent to the specified argument,
     *   which is either 1 or 0
     *     since this container does not allow duplicates.
     * The default method of check the equivalence is !(a < b || b > a)
     */
    size_t count(const Key &key) const {
    }

    /**
     * Finds an element with key equivalent to key.
     * key value of the element to search for.
     * Iterator to an element with key equivalent to key.
     *   If no such element is found, past-the-end (see end()) iterator is
     * returned.
     */
    iterator find(const Key &key) {
    }

    const_iterator find(const Key &key) const {
    }
};

}  // namespace sjtu

#endif
