#ifndef BINARY_SEARCH_TREE_HPP_INCLUDED
#define BINARY_SEARCH_TREE_HPP_INCLUDED

#include <iostream>

namespace BST
{
template<typename _Tp>
class binary_search_tree
{
 public:
    class node;
    using value_type = _Tp;
    using node_pointer = node*;

    class node
    {
     private:
        value_type* key;
        node_pointer left;
        node_pointer right;
        node_pointer parent;

     public:
        constexpr node() : key(nullptr), left(nullptr), right(nullptr), parent(nullptr) {}

        explicit constexpr node(value_type key) : node() { this->key = new value_type(key); }

        virtual ~node() { this->clear(); }

        constexpr node(const node& n) : key(nullptr), left(n.left), right(n.right), parent(n.parent)
            { if(n.key != nullptr) key = new value_type(*n.key); }

        constexpr node(node&& n) noexcept : key(n.key), left(n.left), right(n.right), parent(n.parent)
            { n.key = nullptr; n.left = nullptr; n.right = nullptr; n.parent = nullptr; }

        constexpr node& operator=(const node& n)
        {
            if(this == &n)
                return *this;

            clear();

            key = (n.key != nullptr) ? new value_type(*n.key) : nullptr;
            left = n.left;
            right = n.right;
            parent = n.parent;
            return *this;
        }

        constexpr node& operator=(node&& n) noexcept
        {
            clear();

            key = n.key;
            left = n.left;
            right = n.right;
            parent = n.parent;

            n.key = nullptr;
            n.left = nullptr;
            n.right = nullptr;
            n.parent = nullptr;
            return *this;
        }

        constexpr void clear()
        {
            if(key != nullptr)
            {
                delete key;
                key = nullptr;
            }

            if(left != nullptr)
                left->parent = nullptr;

            if(right != nullptr)
                right->parent = nullptr;

            if(parent != nullptr)
            {
                if(parent->left == this)
                    parent->left = nullptr;
                else
                    parent->right = nullptr;
            }
        }

        constexpr void create_left(value_type key)
        {
            if(left != nullptr)
                return;

            left = new node(key);
            left->parent = this;
        }

        constexpr void create_right(value_type key)
        {
            if(right != nullptr)
                return;

            right = new node(key);
            right->parent = this;
        }

        constexpr void create_parent_left(value_type key)
        {
            if(parent != nullptr)
                return;

            parent = new node(key);
            parent->left = this;
        }

        constexpr void create_parent_right(value_type key)
        {
            if(parent != nullptr)
                return;

            parent = new node(key);
            parent->right = this;
        }

        constexpr void set_left(node_pointer new_left)
        {
            if(left != nullptr)
                left->parent = nullptr;

            left = new_left;

            if(new_left != nullptr)
                new_left->parent = this;
        }

        constexpr node_pointer get_left() const
        {
            return left;
        }

        constexpr void set_right(node_pointer new_right)
        {
            if(right != nullptr)
                right->parent = nullptr;

            right = new_right;

            if(new_right != nullptr)
                new_right->parent = this;
        }

        constexpr node_pointer get_right() const
        {
            return right;
        }

        constexpr void set_parent_left(node_pointer new_parent)
        {
            if(parent != nullptr)
                (parent->get_left() == this) ? parent->set_left(nullptr) : parent->set_right(nullptr);

            parent = new_parent;

            if(new_parent != nullptr)
                new_parent->set_left(this);
        }

        constexpr void set_parent_right(node_pointer new_parent)
        {
            if(parent != nullptr)
                (parent->get_left() == this) ? parent->set_left(nullptr) : parent->set_right(nullptr);

            parent = new_parent;

            if(new_parent != nullptr)
                new_parent->set_right(this);
        }

        constexpr node_pointer get_parent() const
        {
            return parent;
        }

        constexpr void set_key(value_type key)
        {
            if(this->key != nullptr)
                *this->key = key;
            else
                this->key = new value_type(key);
        }

        constexpr value_type get_key() const
        {
            return *key;
        }
    };

    static constexpr node_pointer make_node(value_type key)
    {
        return new node(key);
    }

 protected:
    node_pointer root;

 private:
    static constexpr void preorder_traversal_inserter(binary_search_tree& dst_tree, node_pointer starting_node)
    {
        if(!starting_node)
            return;

        dst_tree.insert(starting_node->get_key());
        preorder_traversal_inserter(dst_tree, starting_node->get_left());
        preorder_traversal_inserter(dst_tree, starting_node->get_right());
    }

    static constexpr void tree_copy(binary_search_tree& dst_tree, const binary_search_tree& src_tree)
    {
        dst_tree.clear();
        preorder_traversal_inserter(dst_tree, src_tree.get_root());
    }

 public:
    constexpr binary_search_tree() : root(nullptr) {}

    explicit constexpr binary_search_tree(value_type root_key) : root(make_node(root_key)) {}

    constexpr binary_search_tree(std::initializer_list<value_type> initList) : root(nullptr)
    {
        for(auto iter = initList.begin(), iter_end = initList.end(); iter != iter_end; iter++)
            insert(*iter);
    }

    virtual ~binary_search_tree()
    {
        clear();
    }

    constexpr binary_search_tree(const binary_search_tree& bst) : root(nullptr) { tree_copy(*this, bst); }

    constexpr binary_search_tree(binary_search_tree&& bst) noexcept : root(root = bst.root) { bst.root = nullptr; }

    constexpr binary_search_tree& operator=(const binary_search_tree& bst)
    {
        if(this == &bst)
            return *this;

        tree_copy(*this, bst);
        return *this;
    }

    constexpr binary_search_tree& operator=(binary_search_tree&& bst) noexcept
    {
        clear();
        root = bst.root;
        bst.root = nullptr;
        return *this;
    }

    constexpr void clear()
    {
        if(root != nullptr)
        {
            delete_tree(root);
            root = nullptr;
        }
    }

    constexpr void set_root(value_type key)
    {
        if(root != nullptr)
            clear();

        root = make_node(key);
    }

    constexpr node_pointer get_root() const
    {
        return root;
    }

    constexpr unsigned count(node_pointer starting_node, value_type key) const
    {
        return (!starting_node) ? 0 :
               (starting_node->get_key() == key) ? 1 + count(starting_node->get_left(), key) + count(starting_node->get_right(), key) :
                                                   0 + count(starting_node->get_left(), key) + count(starting_node->get_right(), key);
    }

    constexpr unsigned count(value_type key) const
    {
        return count(root, key);
    }

    constexpr unsigned count(node_pointer starting_node) const
    {
        return (!starting_node) ? 0 : 1 + count(starting_node->get_left()) + count(starting_node->get_right());
    }

    constexpr unsigned count() const
    {
        return count(root);
    }

    template<typename Predicate> constexpr unsigned count_if(node_pointer starting_node, Predicate pred) const
    {
        return (!starting_node) ? 0 :
               (pred(std::forward<value_type>(starting_node->get_key()))) ?
                                                    1 + count_if(starting_node->get_left(), pred) + count_if(starting_node->get_right(), pred) :
                                                    0 + count_if(starting_node->get_left(), pred) + count_if(starting_node->get_right(), pred);
    }

    template<typename Predicate> constexpr unsigned count_if(Predicate pred) const
    {
        return count_if(root, pred);
    }

    constexpr void inorder_traversal(node_pointer pivot_node) const
    {
        if(!pivot_node)
            return;

        inorder_traversal(pivot_node->get_left());
        std::cout << pivot_node->get_key() << "  ";
        inorder_traversal(pivot_node->get_right());
    }

    constexpr void inorder_traversal() const
    {
        inorder_traversal(root);
    }

    constexpr void preorder_traversal(node_pointer pivot_node) const
    {
        if(!pivot_node)
            return;

        std::cout << pivot_node->get_key() << "  ";
        preorder_traversal(pivot_node->get_left());
        preorder_traversal(pivot_node->get_right());
    }

    constexpr void preorder_traversal() const
    {
        preorder_traversal(root);
    }

    constexpr void postorder_traversal(node_pointer pivot_node) const
    {
        if(!pivot_node)
            return;

        postorder_traversal(pivot_node->get_left());
        postorder_traversal(pivot_node->get_right());
        std::cout << pivot_node->get_key() << "  ";
    }

    constexpr void postorder_traversal() const
    {
        postorder_traversal(root);
    }

    constexpr node_pointer search(node_pointer starting_node, value_type key) const
    {
        if(!starting_node || key == starting_node->get_key())
            return starting_node;

        if(key < starting_node->get_key())
        {
            return search(starting_node->get_left(), key);
        }
        else
        {
            return search(starting_node->get_right(), key);
        }
    }

    constexpr node_pointer search(value_type key) const
    {
        return search(root, key);
    }

    constexpr node_pointer minimum(node_pointer starting_node) const
    {
        if(!starting_node)
            return nullptr;

        while(starting_node->get_left() != nullptr)
        {
            starting_node = starting_node->get_left();
        }

        return starting_node;
    }

    constexpr node_pointer minimum() const
    {
        return minimum(root);
    }

    constexpr node_pointer maximum(node_pointer starting_node) const
    {
        if(!starting_node)
            return nullptr;

        while(starting_node->get_right() != nullptr)
        {
            starting_node = starting_node->get_right();
        }

        return starting_node;
    }

    constexpr node_pointer maximum() const
    {
        return maximum(root);
    }

    constexpr node_pointer successor(node_pointer starting_node) const
    {
        if(!starting_node)
        {
            return nullptr;
        }
        else if(starting_node->get_right() != nullptr)
        {
            return minimum(starting_node->get_right());
        }
        else
        {
            node_pointer temp_node = starting_node->get_parent();
            while(temp_node != nullptr && starting_node == temp_node->get_right())
            {
                starting_node = temp_node;
                temp_node = temp_node->get_parent();
            }

            return temp_node;
        }
    }

    constexpr node_pointer successor() const
    {
        return successor(root);
    }

    constexpr void insert(node inserted_node)
    {
        node_pointer new_node = make_node(inserted_node.get_key());
        node_pointer pivot_node = nullptr;
        node_pointer checking_node = root;
        while(checking_node != nullptr)
        {
            pivot_node = checking_node;
            if(new_node->get_key() < checking_node->get_key())
            {
                checking_node = checking_node->get_left();
            }
            else
            {
                checking_node = checking_node->get_right();
            }
        }

        if(!pivot_node) /* Tree was empty */
        {
            set_root(new_node);
        }
        else if(new_node->get_key() < pivot_node->get_key())
        {
            pivot_node->set_left(new_node);
        }
        else
        {
            pivot_node->set_right(new_node);
        }
    }

    constexpr void insert(value_type key)
    {
        node_pointer pivot_node = nullptr;
        node_pointer checking_node = root;
        while(checking_node != nullptr)
        {
            pivot_node = checking_node;
            if(key < checking_node->get_key())
            {
                checking_node = checking_node->get_left();
            }
            else
            {
                checking_node = checking_node->get_right();
            }
        }

        if(!pivot_node) /* Tree was empty */
        {
            set_root(key);
        }
        else if(key < pivot_node->get_key())
        {
            pivot_node->create_left(key);
        }
        else
        {
            pivot_node->create_right(key);
        }
    }

    constexpr node_pointer delete_node(node_pointer starting_node, value_type key)
    {
        // Base case
        if(!starting_node)
            return starting_node;

        // Recursive calls for ancestors of node to be deleted
        if(key < starting_node->get_key())
        {
            starting_node->set_left(delete_node(starting_node->get_left(), key));
            return starting_node;
        }
        else if(key > starting_node->get_key())
        {
            starting_node->set_right(delete_node(starting_node->get_right(), key));
            return starting_node;
        }

        // We reach here when starting_node is the node to be deleted

        // If both children are empty
        if(!starting_node->get_left() && !starting_node->get_right())
        {
            // disconnect the current node with its parent
            node_pointer current_parent = starting_node->get_parent();
            if(current_parent != nullptr)
                (starting_node == current_parent->get_left()) ? current_parent->set_left(nullptr) : current_parent->set_right(nullptr);

            // Special case where the node to be deleted is the root, this mean tree only has one node
            if(starting_node == root)
                root = current_parent;

            delete starting_node;

            return nullptr;
        }
        // If only one of the children is empty
        else if(!starting_node->get_left())
        {
            node_pointer temp = starting_node->get_right();

            // Set new parent for temp
            node_pointer new_parent = starting_node->get_parent();
            if(new_parent != nullptr)
                (starting_node == new_parent->get_left()) ? temp->set_parent_left(new_parent) : temp->set_parent_right(new_parent);

            // Special case where the node to be deleted is the root, reset the root
            if(starting_node == root)
                root = temp;

            delete starting_node;

            return temp;
        }
        else if(!starting_node->get_right())
        {
            node_pointer temp = starting_node->get_left();

            // Set new parent for temp
            node_pointer new_parent = starting_node->get_parent();
            if(new_parent != nullptr)
                (starting_node == new_parent->get_left()) ? temp->set_parent_left(new_parent) : temp->set_parent_right(new_parent);

            // Special case where the node to be deleted is the root, reset the root
            if(starting_node == root)
                root = temp;

            delete starting_node;

            return temp;
        }
        // If both children exist
        else
        {
            node_pointer succ_parent = starting_node;

            // Find successor
            node_pointer succ = starting_node->get_right();
            while(succ->get_left() != nullptr)
            {
                succ_parent = succ;
                succ = succ->get_left();
            }

            // Delete successor. Since successor is always left child of its parent it is safe to make successor's right child as left of its parent.
            // If there is no succ, then assign succ->right to succ_parent->right
            if(succ_parent != starting_node)
            {
                succ_parent->set_left(succ->get_right());
            }
            else
            {
                succ_parent->set_right(succ->get_right());
            }

            // Copy Successor Data to starting_node before deleting it
            starting_node->set_key(succ->get_key());

            delete succ;

            return starting_node;
        }
    }

    constexpr node_pointer delete_node(value_type key)
    {
        return delete_node(root, key);
    }

    constexpr node_pointer delete_all_node(node_pointer starting_node, value_type key)
    {
        for(unsigned counter = count(starting_node, key); counter != 0; counter--)
        {
            starting_node = delete_node(starting_node, key);
        }
        return starting_node;
    }

    constexpr node_pointer delete_all_node(value_type key)
    {
        return delete_all_node(root, key);
    }

    constexpr void delete_tree(node_pointer root)
    {
        if(!root)
            return;

        /* first delete both subtrees */
        delete_tree(root->get_left());
        delete_tree(root->get_right());

        /* then delete the node */

        if(root->get_parent() != nullptr)
        {
            node_pointer root_parent = root->get_parent();
            (root == root_parent->get_left()) ? root_parent->set_left(nullptr) : root_parent->set_right(nullptr);
        }
        else if(root == this->root)
        {
            this->root = nullptr;
        }

        delete root;
    }
};

template<typename T> constexpr void bst_sort(T* arr, size_t length)
{
    binary_search_tree<T> tree;
    for(size_t i = 0; i < length; i++)
        tree.insert(arr[i]);
    tree.inorder_traversal(tree.get_root());
}

template<typename T, size_t length> constexpr void bst_sort(T (&arr)[length])
{
    bst_sort(arr, length);
}
}

#endif // BINARY_SEARCH_TREE_HPP_INCLUDED

