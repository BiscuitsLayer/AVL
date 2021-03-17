#pragma once

//  SYSTEM
#include <vector>
#include <stack>
#include <iterator>

namespace AVL {
    template <typename T>
    class Tree final {
        public:
            class Iterator;

            //  CTORS
            Tree ():
                nodes_ ({}),
                head_ (nullptr)
                {}  
            Tree (const std::vector <T>& data):
                nodes_ ({}),
                head_ (nullptr)
                {
                    for (auto&& item : data) {
                        insert (item);
                    }
                }

            //  DTOR
            ~Tree () {
                for (auto* node : nodes_) {
                    delete node;
                }
            }

            //  COPY
            Tree (const Tree& rhs):
                nodes_ (rhs.nodes_.size ()),
                head_ (nullptr)
                {   
                    std::vector <Node*> tempNodes {};
                    tempNodes.reserve (rhs.nodes_.size ());
                    for (int i = 0; i < rhs.nodes_.size (); ++i) {
                        tempNodes.emplace_back (new Node { rhs.nodes_[i]->key_ });
                        tempNodes[i]->id_ = i;
                    }
                    for (auto&& node : tempNodes) {
                        if (rhs.nodes_[node->id_]->parent_) {
                            node->parent_ = nodes_[rhs.nodes_[node->id_]->parent_->id_];
                        }
                        if (rhs.nodes_[node->id_]->left_) {
                            node->left_ = nodes_[rhs.nodes_[node->id_]->left_->id_];
                        }
                        if (rhs.nodes_[node->id_]->right_) {
                            node->right_ = nodes_[rhs.nodes_[node->id_]->right_->id_];
                        }
                    }
                    Node* tempHead = nodes_[rhs.head_->id_];
                    std::swap (tempNodes, nodes_);
                    std::swap (tempHead, head_);
                }
            Tree& operator = (Tree& rhs) {
                if (this != &rhs) {
                    Tree temp { rhs };
                    ShallowSwap (temp);
                }
                return *this;
            }

            //  MOVE
            Tree (Tree&& rhs) noexcept {
                ShallowSwap (rhs);
            }
            Tree& operator = (Tree&& rhs) {
                if (this != &rhs) {
                    ShallowSwap (rhs);
                }
                return *this;
            }

            //  MODIFIERS
            void clear () { 
                head_ = nullptr;
                for (auto&& node : nodes_) {
                    delete node;
                }
                nodes_.clear ();
            }
            Iterator insert (const T& key) {
                Node* result = InsertRecursive (key, head_);
                if (nodes_.size () == 1) {
                    head_ = result;
                }
                return MakeIterator (this, result);
            }
            void extract (const T& key) {
                Node* result = ExtractRecursive (key, head_);
                if (!head_) {
                    head_ = result;
                }
            }
            void extract (const Iterator& it) {
                Node* result = ExtractRecursive (*it, it);
                if (!head_) {
                    head_ = result;
                }
            }

            //  ITERATORS
            Iterator begin () {
                Node* ans = head_;
                if (ans) {
                    while (ans->left_) {
                        ans = ans->left_;
                    }
                }
                return MakeIterator (this, ans);
            }
            Iterator end () {
                return MakeIterator (this, nullptr);
            }
            Iterator lower_bound (const T& key) {
                Node* cur = head_;
                Node* prev = nullptr;
                while (cur) {
                    if (key < cur->key_) {
                        prev = cur;
                        cur = cur->left_;
                    }
                    else if (key > cur->key_) {
                        prev = cur;
                        cur = cur->right_;
                    }
                    else { // key == cur->key_
                        return MakeIterator (this, cur);
                    }
                }
                Iterator ans = MakeIterator (this, prev);
                return (prev->key_ > key ? ans : ++ans);
            }
            Iterator upper_bound (const T& key) {
                Iterator ans = lower_bound (key);
                if (ans && *ans == key) {
                    ++ans;
                }
                return ans;
            }

            //  CAPACITY
            size_t size () const { return nodes_.size (); }
            bool empty () const { return nodes_.empty (); }

            //  LOOKUP
            Iterator find (const T& key) const {
                Node* cur = head_;
                while (cur) {
                    if (key < cur->key_) {
                        cur = cur->left_;
                    }
                    else if (key > cur->key_) {
                        cur = cur->right_;
                    }
                    else { // key == cur->key_
                        break;
                    }
                }
                return MakeIterator (this, cur);
            }

            //  DOT IMAGE
            void dump (std::ofstream* outfile) const {
                *outfile << "digraph G {" << std::endl << "fontsize = 50" << std::endl;
                if (head_) {
                    *outfile << head_->id_ << "[label = \"" << head_->key_ << "\"]" << std::endl;
                    MakeDotRecursive (outfile, head_);
                }
                *outfile << "}";
            }
        private:
            struct Node final {
                public:
                    //  DATA
                    Node* parent_ = nullptr;
                    T key_ {};
                    int height_ = 1;
                    int balanceFactor_ = 0;
                    int id_ = -1;
                    Node* left_ = nullptr;
                    Node* right_ = nullptr;

                    //  CTOR
                    Node (T key):
                        parent_ (),
                        key_ (key),
                        height_ (),
                        balanceFactor_ (),
                        id_ (),
                        left_ (),
                        right_ ()
                        {}

                    //  COPY
                    Node (const Node& rhs) = delete;
                    Node& operator = (Node& rhs) = delete;

                    //  MOVE
                    Node (const Node&& rhs) = delete;
                    Node& operator = (Node&& rhs) = delete;

                    //  SERVICE
                    void Update () {
                        int leftHeight = (left_ ? left_->height_ : 0);
                        int rightHeight = (right_ ? right_->height_ : 0);
                        balanceFactor_ = rightHeight - leftHeight;
                        height_ = std::max (leftHeight, rightHeight) + 1;
                    }
            }; 
            //  DATA
            std::vector <Node*> nodes_ {};
            Node* head_ = nullptr;

            //  ITERATOR
            friend Iterator MakeIterator (const Tree* tree, Node* node);
            
            //  SHALLOW SWAP
            void ShallowSwap (Tree& rhs) {
                std::swap (this->head_, rhs.head_);
                std::swap (this->nodes_, rhs.nodes_);
            }

            //  MODIFIERS
            Node* InsertRecursive (T key, Node* node) {
                if (!node) {
                    node = new Node { key };
                    node->id_ = nodes_.size ();
                    nodes_.push_back (node);
                    return node;
                }
                if (key < node->key_) {
                    node->left_ = InsertRecursive (key, node->left_);
                    node->left_->parent_ = node;
                }
                else
                if (key > node->key_) {
                    node->right_ = InsertRecursive (key, node->right_);
                    node->right_->parent_ = node;
                }
                return Balance (node);
            }
            Node* RemoveMin (Node* node) {
                if (node->left_) {
                    node->left_ = RemoveMin (node->left_);
                    return Balance (node);
                }
                return node->right_;
            }
            Node* ExtractRecursive (T key, Node* node) {
                if (!node) { return nullptr; }
                if (key < node->key_) {
                    node->left_ = ExtractRecursive (key, node->left_);
                }
                else 
                if (key > node->key_) {
                    node->right_ = ExtractRecursive (key, node->right_);
                }
                else { // key == node->key_
                    Node* lhs = node->left_;
		            Node* rhs = node->right_;

                    // moving node ptrs in vector
                    nodes_.back ()->id_ = node->id_;
                    nodes_[node->id_] = nodes_.back ();
                    nodes_.pop_back ();

                    // check if we are deleting the head
                    if (node == head_) {
                        head_ = nullptr;
                    }
		            delete node;

		            if (!rhs) {
                        return lhs;
                    }
		            Node* min = rhs;
                    while (min->left_) {
                        min = min->left_;
                    }
                    min->right_ = RemoveMin (rhs);
		            min->left_ = lhs;
		            return Balance (min);
                }
                return Balance (node);
            }

            //  ROTATIONS
            Node* RotateLeft (Node* node) {
                Node* temp = node->right_;
                temp->parent_ = node->parent_;

                node->right_ = temp->left_;
                if (temp->left_) {
                    temp->left_->parent_ = node;
                }

                temp->left_ = node;
                node->parent_ = temp;

                if (node == head_) {
                    head_ = temp;
                }

                node->Update ();
                temp->Update ();
                return temp;
            }
            Node* RotateRight (Node* node) {
                Node* temp = node->left_;
                temp->parent_ = node->parent_;

                node->left_ = temp->right_;
                if (temp->right_) {
                    temp->right_->parent_ = node;
                }

                temp->right_ = node;
                node->parent_ = temp;

                if (node == head_) {
                    head_ = temp;
                }

                node->Update ();
                temp->Update ();
                return temp;
            }

            //  BALANCING
            Node* Balance (Node* node) {
                node->Update ();
                if (node->balanceFactor_ > 1) {
                    if (node->right_ && node->right_->balanceFactor_ < 0) {
                        node->right_ = RotateRight (node->right_);
                        return RotateLeft (node);
                    }
                }
                if (node->balanceFactor_ < -1) {
                    if (node->left_ && node->left_->balanceFactor_ > 0) {
                        node->left_ = RotateLeft (node->left_);
                        return RotateRight (node);
                    }
                }
                return node;
            }

            //  DOT IMAGE
            void MakeDotRecursive (std::ofstream* outfile, Node* node) const {
                if (!node) { return; }
                if (node->left_) {                    
                    *outfile << node->left_->id_ << "[label = \"" << node->left_->key_ << "\"]" << std::endl;
                    *outfile << node->id_ << " -> " << node->left_->id_ << std::endl;
                    MakeDotRecursive (outfile, node->left_);
                }
                if (node->right_) {
                    *outfile << node->right_->id_ << "[label = \"" << node->right_->key_ << "\"]" << std::endl;
                    *outfile << node->id_ << " -> " << node->right_->id_ << std::endl;
                    MakeDotRecursive (outfile, node->right_);
                }
            }

        public:
            //  ITERATOR
            class Iterator final {
                private:
                    //  DATA
                    Tree* tree_ = nullptr;
                    Node* node_ = nullptr;
                    std::stack <Node*> previousStack_ {};
                    
                    //  SERVICE STUFF
                    Iterator (const Tree* tree, Node* node):
                        tree_ (const_cast <Tree*> (tree)),
                        node_ (node)
                        {
                            std::cerr << "Ctor iter" << std::endl;
                            Node* cur = tree_->head_;
                            if (node) {
                                while (cur) {
                                    if (node->key_ < cur->key_) {
                                        previousStack_.push (cur);
                                        cur = cur->left_;
                                    }
                                    else if (node->key_ > cur->key_) {
                                        cur = cur->right_;
                                    }
                                    else { // node->key_ == cur->key_
                                        cur = cur->right_;
                                        while (cur) {
                                            previousStack_.push (cur);
                                            cur = cur->left_;
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                        
                    explicit Iterator (const Tree* tree, Node* node, std::stack <Node*>&& previousStack):
                        tree_ (const_cast <Tree*> (tree)),
                        node_ (node)
                        {
                            std::cerr << "Move ctor iter" << std::endl;
                            std::swap (previousStack, previousStack_);
                            std::cerr << "Move ctor iter finished" << std::endl;
                        }
                        
                    //  FRIEND
                    friend Iterator MakeIterator (const Tree* tree, Node* node) {
                        return { tree, node };
                    }
                    
                public:
                    //  REQUIRED TYPES
                    using iterator_category = std::forward_iterator_tag;
                    using value_type = T;
                    using difference_type = std::ptrdiff_t;
                    using pointer = Node*;
                    using reference = T&;

                    //  CTOR
                    Iterator ():
                        node_ (nullptr),
                        tree_ (nullptr)
                        {}

                    //  CAST OPERATORS
                    explicit operator bool () { return node_; }

                    //  OPERATORS
                    const T operator * () const { 
                        if (node_) {
                            return node_->key_; 
                        }
                        return static_cast <T> (-666);
                    }
                    const T* operator -> () const { return node_; }
                    Iterator operator ++ () {
                        std::cerr << "operator ++" << std::endl;
                        // now complexity is amortized O (1)
                        if (!previousStack_.empty ()) {
                            node_ = previousStack_.top ();
                            previousStack_.pop ();
                            for (Node* cur = node_->right_; cur != nullptr; cur = cur->left_) {
                                previousStack_.push (cur);
                            }
                        }
                        else {
                            node_ = nullptr;
                        }
                        //auto retVal = Iterator { tree_, node_, std::move (previousStack_) };
                        //std::cerr << "operator ++ retVal done: " << *retVal << std::endl;
                        //return retVal;
                        return *this;
                    }
                    Iterator operator ++ (int) {
                        Iterator ans = *this;
                        ++(*this);
                        return ans;
                    }
                    friend bool operator == (const Iterator& lhs, const Iterator& rhs) { return lhs.node_ == rhs.node_; }
                    friend bool operator != (const Iterator& lhs, const Iterator& rhs) { return !(lhs == rhs); }
            };
    };
}