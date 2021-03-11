#pragma once

#include <iostream>
#include <vector>

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
            Tree (std::vector <T>& data):
                nodes_ ({}),
                head_ (nullptr)
                {
                    for (auto&& item : data) {
                        Insert (item);
                    }
                }

            //  DTOR
            ~Tree () {
                for (auto&& node : nodes_) {
                    delete node;
                }
            }

            //  COPY
            Tree (const Tree& rhs):
                nodes_ (rhs.nodes_.size ()),
                head_ (nullptr)
                {
                    for (int i = 0; i < rhs.nodes_.size (); ++i) {
                        nodes_[i] = new Node { rhs.nodes_[i]->key_ };
                        nodes_[i]->id_ = i;
                    }
                    head_ = nodes_[rhs.head_->id_];
                    for (auto&& node : nodes_) {
                        if (nodes_[node->id_]->left_) {
                            node->left_ = nodes_[node->left_->id_];
                        }
                        if (nodes_[node->id_]->right_) {
                            node->right_ = nodes_[node->right_->id_];
                        }
                    }
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
            void Clear () { 
                head_ = nullptr;
                for (auto&& node : nodes_) {
                    delete node;
                }
                nodes_.clear ();
            }
            Iterator Insert (const T& key) {
                Node* result = InsertRecursive (key, head_);
                if (nodes_.size () == 1) {
                    head_ = result;
                }
                return MakeIterator (this, result);
            }
            void Extract (const T& key) {
                Node* result = ExtractRecursive (key, head_);
                if (!head_) {
                    head_ = result;
                }
            }
            void Extract (Iterator it) {
                Node* result = ExtractRecursive (*it, static_cast <Node*> (it));
                if (!head_) {
                    head_ = result;
                }
            }

            //  ITERATORS
            Iterator begin () {
                Node* ansNode = head_;
                if (ansNode) {
                    while (ansNode->left_) {
                        ansNode = ansNode->left_;
                    }
                }
                return MakeIterator (this, ansNode);
            }
            Iterator end () {
                return MakeIterator (this, nullptr);
            }
            Iterator LowerBound (const T& key) {
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
            Iterator UpperBound (const T& key) {
                Iterator ans = LowerBound (key);
                if (ans && *ans == key) {
                    ++ans;
                }
                return ans;
            }

            //  CAPACITY
            size_t Size () { return nodes_.size (); }
            bool Empty () { return nodes_.empty (); }

            //  LOOKUP
            Iterator Find (const T& key) {
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
            void MakeDot (std::ofstream* outfile) {
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
                    T key_ {};
                    int height_ = 1;
                    int balanceFactor_ = 0;
                    int id_ = -1;
                    Node* left_ = nullptr;
                    Node* right_ = nullptr;

                    //  CTOR
                    Node (T key):
                        key_ (key),
                        height_ (1),
                        balanceFactor_ (0),
                        id_ (-1),
                        left_ (nullptr),
                        right_ (nullptr)
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
            template <typename U>
            friend typename Tree <U>::Iterator MakeIterator (Tree <U>* tree, typename Tree <U>::Node* node) {
                return { tree, node };
            }
            
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
                }
                else { // key >= node->key_
                    node->right_ = InsertRecursive (key, node->right_);
                }
                return Balance (node);
            }
            Node* RemoveMin (Node* node) {
                return (node->left_ ? node->left_ = RemoveMin (node->left_), Balance (node) : node->right_);
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
                node->right_ = temp->left_;
                temp->left_ = node;
                if (node == head_) {
                    head_ = temp;
                }
                node->Update ();
                temp->Update ();
                return temp;
            }
            Node* RotateRight (Node* node) {
                Node* temp = node->left_;
                node->left_ = temp->right_;
                temp->right_ = node;
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
                if (node->balanceFactor_ == 2) {
                    if (node->right_ && node->right_->balanceFactor_ < 0) {
                        node->right_ = RotateRight (node->right_);
                        return RotateLeft (node);
                    }
                }
                if (node->balanceFactor_ == -2) {
                    if (node->left_ && node->left_->balanceFactor_ > 0) {
                        node->left_ = RotateLeft (node->left_);
                        return RotateRight (node);
                    }
                }
                return node;
            }

            //  DOT IMAGE
            void MakeDotRecursive (std::ofstream* outfile, Node* node) {
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
                    Node* node_ = nullptr;
                    Tree* tree_ = nullptr;
                    
                    //  SERVICE CTOR
                    Iterator (Tree* tree, Node* node):
                        tree_ (tree),
                        node_ (node)
                        {}

                    template <typename U>
                    friend typename Tree <U>::Iterator MakeIterator (Tree <U>* tree, typename Tree <U>::Node* node);
                    
                    
                public:
                    //  CTOR
                    Iterator ():
                        node_ (nullptr),
                        tree_ (nullptr)
                        {}

                    //  CAST OPERATORS
                    explicit operator Node* () { return node_; }
                    explicit operator bool () { return node_; }

                    //  OPERATORS
                    const T operator * () const { return node_->key_; }
                    const T* operator -> () const { return node_; }
                    Iterator operator ++ () {
                        Node* cur = tree_->head_;
                        Node* ans = nullptr;
                        while (cur) {
                            if (cur->key_ > node_->key_) {
                                ans = cur;
                                cur = cur->left_;
                            }
                            else {
                                cur = cur->right_;
                            }
                        }
                        node_ = ans;
                        return { tree_, ans };
                    }
                    Iterator operator ++ (int) {
                        Iterator ans = *this;
                        ++(*this);
                        return ans;
                    }
                    Iterator operator -- () {
                        Node* cur = tree_->head_;
                        Node* ans = nullptr;
                        while (cur) {
                            if (cur->key_ > node_->key_) {
                                cur = cur->left_;
                            }
                            else {
                                ans = cur;
                                cur = cur->right_;
                            }
                        }
                        node_ = ans;
                        return { tree_, ans };
                    }
                    Iterator operator -- (int) {
                        Iterator ans = *this;
                        --(*this);
                        return ans;
                    }
                    bool operator == (const Iterator& rhs) { return node_ == rhs.node_; }
                    bool operator != (const Iterator& rhs) { return !(*this == rhs); }
            };
    };
}