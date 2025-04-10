// acmoj: 2671
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
        Node *parent;

        Node(const value_type &data, Node *left = nullptr,
             Node *right = nullptr, size_t height = 1)
            : data(data), height(height), left(left), right(right) {
        }

        Node(const Node &other) = delete;
        Node &operator=(const Node &other) = delete;

        ~Node() {
            // DO NOTHING!
        }

        Node *balance() {
            update_height();
            int factor = get_balance();
            if (factor > 1) {
                if (left->get_balance() >= 0) {
                    return LL();
                } else {
                    return LR();
                }
            } else if (factor < -1) {
                if (right->get_balance() <= 0) {
                    return RR();
                } else {
                    return RL();
                }
            }
            return this;
        }

        void clear() {
            if (left) {
                left->clear();
                delete left;
                left = nullptr;
            }
            if (right) {
                right->clear();
                delete right;
                right = nullptr;
            }
        }

       private:
        int get_balance() {
            return (left ? left->height : 0) - (right ? right->height : 0);
        }

        void update_height() {
            height = 1 + std::max(left ? left->height : 0,
                                  right ? right->height : 0);
        }

        Node *RR() {
            Node *new_root = right;
            right = new_root->left;
            new_root->left = this;

            update_height();
            new_root->update_height();

            return new_root;
        }

        Node *LL() {
            Node *new_root = left;
            left = new_root->right;
            new_root->right = this;

            update_height();
            new_root->update_height();

            return new_root;
        }

        Node *LR() {
            left = left->RR();
            return LL();
        }

        Node *RL() {
            right = right->LL();
            return RR();
        }
    };

    Node *root;
    size_t node_count;

    Node *insert(const value_type &value, Node *&node) {
        if (node == nullptr) {
            node = new Node(value);
            ++node_count;
            return node;
        }

        if (Compare()(value.first, node->data.first)) {
            node->left = insert(value, node->left);
        } else if (Compare()(node->data.first, value.first)) {
            node->right = insert(value, node->right);
        } else {
            return node;
        }

        return node->balance();
    }

    Node *find_parent(Node *node, const Key &key) {
        Node *parent = nullptr;
        Node *current = node;
        while (current != nullptr) {
            if (Compare()(key, current->data.first)) {
                parent = current;
                current = current->left;
            } else if (Compare()(current->data.first, key)) {
                parent = current;
                current = current->right;
            } else {
                return parent;
            }
        }
        return nullptr;
    }

    Node *find_parent(Node *node, const Key &key) const {
        Node *parent = nullptr;
        Node *current = node;
        while (current != nullptr) {
            if (Compare()(key, current->data.first)) {
                parent = current;
                current = current->left;
            } else if (Compare()(current->data.first, key)) {
                parent = current;
                current = current->right;
            } else {
                return parent;
            }
        }
        return nullptr;
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
                Node *s = find_after(node);
                Node *parent =
                    find_parent(node->right, s->data.first);  // parent of s

                // disconnect s
                if (parent == nullptr) {
                    node->right = s->right;
                } else {
                    parent->left = s->right;
                }

                // replace node with s
                s->left = node->left;
                s->right = node->right;
                delete node;
                node = s;
            }
        }
        return node->balance();
    }

    T &find(const Key &key, Node *node) {
        if (node == nullptr) {
            throw index_out_of_bound();
        }
        if (Compare()(key, node->data.first)) {
            return find(key, node->left);
        } else if (Compare()(node->data.first, key)) {
            return find(key, node->right);
        } else {
            return node->data.second;
        }
    }

    const T &find(const Key &key, Node *node) const {
        if (node == nullptr) {
            throw index_out_of_bound();
        }
        if (Compare()(key, node->data.first)) {
            return find(key, node->left);
        } else if (Compare()(node->data.first, key)) {
            return find(key, node->right);
        } else {
            return node->data.second;
        }
    }

    Node *find_node(const Key &key, Node *node) {
        if (node == nullptr) {
            return nullptr;
        }
        if (Compare()(key, node->data.first)) {
            return find_node(key, node->left);
        } else if (Compare()(node->data.first, key)) {
            return find_node(key, node->right);
        } else {
            return node;
        }
    }

    Node *find_node(const Key &key, Node *node) const {
        if (node == nullptr) {
            return nullptr;
        }
        if (Compare()(key, node->data.first)) {
            return find_node(key, node->left);
        } else if (Compare()(node->data.first, key)) {
            return find_node(key, node->right);
        } else {
            return node;
        }
    }

    Node *find_after(Node *node) const {
        if (node->right != nullptr) {
            Node *temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            return temp;
        } else {
            if (!root || !node) return nullptr;
            Node *successor_candidate = nullptr;
            Node *current = root;
            while (current != nullptr) {
                if (Compare()(node->data.first, current->data.first)) {
                    successor_candidate = current;
                    current = current->left;
                } else if (Compare()(current->data.first, node->data.first)) {
                    current = current->right;
                } else {
                    break;
                }
            }
            return successor_candidate;
        }
    }

    Node *find_before(Node *node) const {
        if (node->left != nullptr) {
            if (!node || !node->left) return nullptr;
            Node *current = node->left;
            while (current->right != nullptr) {
                current = current->right;
            }
            return current;
        } else {
            if (!root || !node) return nullptr;
            Node *predecessor_candidate = nullptr;
            Node *current = root;
            while (current != nullptr) {
                if (Compare()(node->data.first, current->data.first)) {
                    current = current->left;
                } else if (Compare()(current->data.first, node->data.first)) {
                    predecessor_candidate = current;
                    current = current->right;
                } else {
                    break;
                }
            }
            return predecessor_candidate;
        }
    }

    Node *find_max_node(Node *node) const {
        // for end()--;
        if (!node) return nullptr;
        while (node->right != nullptr) {
            node = node->right;
        }
        return node;
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
    T *traverse_all() {
        T *result = new T[node_count];
        int index = 0;
        Node *node = root;
        while (node != nullptr) {
            result[index++] = node->data.second;
            if (node->left != nullptr) {
                node = node->left;
            } else if (node->right != nullptr) {
                node = node->right;
            } else {
                break;
            }
        }
        return result;
    }

    class const_iterator;
    class iterator {
       private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        Node *node;
        map *map_ptr;
        friend class map;

       public:
        iterator() : node(nullptr), map_ptr(nullptr) {
        }

        iterator(Node *node, map *map_ptr) : node(node), map_ptr(map_ptr) {
        }

        iterator(const iterator &other)
            : node(other.node), map_ptr(other.map_ptr) {
        }

        /**
         * TODO iter++
         */
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        /**
         * TODO ++iter
         */
        iterator &operator++() {
            if (node == nullptr) {
                throw invalid_iterator();
            }
            Node *p = node;

            node = map_ptr->find_after(p);
            return *this;
        }

        /**
         * TODO iter--
         */
        iterator operator--(int) {
            iterator temp = *this;
            --(*this);
            return temp;
        }

        /**
         * TODO --iter
         */
        iterator &operator--() {
            if (node == nullptr) {
                if (map_ptr == nullptr || map_ptr->empty()) {
                    throw invalid_iterator();
                }
                node = map_ptr->find_max_node(map_ptr->root);
                if (node == nullptr) {
                    throw invalid_iterator();
                }
            } else {
                Node *p = node;
                if (p == map_ptr->begin().node) {
                    throw invalid_iterator();
                }

                node = map_ptr->find_before(p);
            }
            return *this;
        }

        /**
         * a operator to check whether two iterators are same (pointing to the
         * same memory).
         */
        value_type &operator*() const {
            /*if (node == nullptr) {
                throw invalid_iterator();
            }*/ // throw? I don't know.
            return node->data;
        }

        bool operator==(const iterator &rhs) const {
            return (node == rhs.node) && (map_ptr == rhs.map_ptr);
        }

        bool operator==(const const_iterator &rhs) const {
            return (node == rhs.node) && (map_ptr == rhs.map_ptr);
        }

        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return (node != rhs.node) || (map_ptr != rhs.map_ptr);
        }

        bool operator!=(const const_iterator &rhs) const {
            return (node != rhs.node) || (map_ptr != rhs.map_ptr);
        }

        /**
         * for the support of it->first.
         * See
         * <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/>
         * for help.
         */
        value_type *operator->() const noexcept {
            return &node->data;
        }
    };

    class const_iterator {
        // it should has similar member method as iterator.
        //  and it should be able to construct from an iterator.
       private:
        // data members.
        Node *node;
        const map *map_ptr;
        friend class map;

       public:
        const_iterator() : node(nullptr), map_ptr(nullptr) {
        }
        const_iterator(Node *node, const map *map_ptr)
            : node(node), map_ptr(map_ptr) {
        }

        const_iterator(const const_iterator &other)
            : node(other.node), map_ptr(other.map_ptr) {
        }

        const_iterator(const iterator &other)
            : node(other.node), map_ptr(other.map_ptr) {
        }
        const_iterator &operator=(const const_iterator &other) {
            if (this != &other) {
                node = other.node;
                map_ptr = other.map_ptr;
            }
            return *this;
        }
        const_iterator &operator=(const iterator &other) {
            node = other.node;
            map_ptr = other.map_ptr;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }
        const_iterator &operator++() {
            if (node == nullptr) {
                throw invalid_iterator();
            }
            Node *p = node;

            node = map_ptr->find_after(p);
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator temp = *this;
            --(*this);
            return temp;
        }
        const_iterator &operator--() {
            if (node == nullptr) {
                if (map_ptr == nullptr || map_ptr->empty()) {
                    throw invalid_iterator();
                }
                node = map_ptr->find_max_node(map_ptr->root);
                if (node == nullptr) {
                    throw invalid_iterator();
                }
            } else {
                Node *p = node;
                if (p == map_ptr->cbegin().node) {
                    throw invalid_iterator();
                }

                node = map_ptr->find_before(p);
            }
            return *this;
        }
        const value_type &operator*() const {
            if (node == nullptr) {
                throw invalid_iterator();
            }
            return node->data;
        }
        const value_type *operator->() const noexcept {
            if (node == nullptr) {
                throw invalid_iterator();
            }
            return &node->data;
        }
        bool operator==(const const_iterator &rhs) const {
            return (node == rhs.node) && (map_ptr == rhs.map_ptr);
        }
        bool operator==(const iterator &rhs) const {
            return (node == rhs.node) && (map_ptr == rhs.map_ptr);
        }
        bool operator!=(const const_iterator &rhs) const {
            return (node != rhs.node) || (map_ptr != rhs.map_ptr);
        }
        bool operator!=(const iterator &rhs) const {
            return (node != rhs.node) || (map_ptr != rhs.map_ptr);
        }
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
            clear();
            deep_copy(root, other.root);
            node_count = other.node_count;
        }
        return *this;
    }

    /**
     * TODO Destructors
     */
    ~map() {
        if (root != nullptr) {
            root->clear();
        }
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
        if (root == nullptr) {
            throw index_out_of_bound();
        }
        return find(key, root);
    }

    const T &at(const Key &key) const {
        if (root == nullptr) {
            throw index_out_of_bound();
        }
        return find(key, root);
    }

    /**
     * TODO
     * access specified element
     * Returns a reference to the value that is mapped to a key equivalent to
     * key, performing an insertion if such key does not already exist.
     */
    T &operator[](const Key &key) {
        try {
            return at(key);
        } catch (index_out_of_bound &) {
            // insert
            value_type value(key, T());
            pair<iterator, bool> result = insert(value);
            return result.first->second;
        }
    }

    /**
     * behave like at() throw index_out_of_bound if such key does not exist.
     */
    const T &operator[](const Key &key) const {
        if (root == nullptr) {
            throw index_out_of_bound();
        }
        return find(key, root);
    }

    /**
     * return a iterator to the beginning
     */
    iterator begin() {
        if (root == nullptr) {
            return iterator(nullptr, this);
        }
        Node *node = root;
        while (node->left != nullptr) {
            node = node->left;
        }
        return iterator(node, this);
    }

    const_iterator cbegin() const {
        if (root == nullptr) {
            return const_iterator(nullptr, this);
        }
        Node *node = root;
        while (node->left != nullptr) {
            node = node->left;
        }
        return const_iterator(node, this);
    }

    /**
     * return a iterator to the end
     * in fact, it returns past-the-end.
     */
    iterator end() {
        return iterator(nullptr, this);
    }

    const_iterator cend() const {
        return const_iterator(nullptr, this);
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
        if (root != nullptr) {
            root->clear();
            delete root;
            root = nullptr;
        }
        node_count = 0;
    }

    /**
     * insert an element.
     * return a pair, the first of the pair is
     *   the iterator to the new element (or the element that prevented the
     * insertion), the second one is true if insert successfully, or false.
     */
    pair<iterator, bool> insert(const value_type &value) {
        Node *found_node = find_node(value.first, root);
        if (found_node != nullptr) {
            // Key already exists
            return pair<iterator, bool>(iterator(found_node, this), false);
        } else {
            size_t old_size = node_count;
            root = insert(value, root);
            Node *new_node = find_node(value.first, root);
            return pair<iterator, bool>(iterator(new_node, this), true);
        }
    }

    /**
     * erase the element at pos.
     *
     * throw if pos pointed to a bad element (pos == this->end() || pos points
     * an element out of this)
     */
    void erase(iterator pos) {
        if (pos.node == nullptr || pos.map_ptr != this) {
            throw invalid_iterator();
        }
        root = erase(pos.node->data.first, root);
        --node_count;
    }

    /**
     * Returns the number of elements with key
     *   that compares equivalent to the specified argument,
     *   which is either 1 or 0
     *     since this container does not allow duplicates.
     * The default method of check the equivalence is !(a < b || b > a)
     */
    size_t count(const Key &key) const {
        if (root == nullptr) {
            return 0;
        }
        Node *node = find_node(key, root);
        if (node == nullptr) {
            return 0;
        }
        return 1;
    }

    /**
     * Finds an element with key equivalent to key.
     * key value of the element to search for.
     * Iterator to an element with key equivalent to key.
     *   If no such element is found, past-the-end (see end()) iterator is
     * returned.
     */
    iterator find(const Key &key) {
        if (root == nullptr) {
            return end();
        }
        Node *node = find_node(key, root);
        if (node == nullptr) {
            return end();
        }
        return iterator(node, this);
    }

    const_iterator find(const Key &key) const {
        if (root == nullptr) {
            return cend();
        }
        Node *node = find_node(key, root);
        if (node == nullptr) {
            return cend();
        }
        return const_iterator(node, this);
    }
};

}  // namespace sjtu

#endif