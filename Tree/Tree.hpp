#pragma once

#include <iostream>
#include <vector>

#include "../Settings/Settings.hpp"

//  when you finish, dont forget to subst. char with T

namespace AVL {
    struct Node final {
        public:
            //  DATA
            char key_ {};
            int height_ = 1;
            int balanceFactor_ = 0;
            int id_ = -1;
            Node* left_ = nullptr;
            Node* right_ = nullptr;

            //  CTOR
            Node (char key):
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

            void Update () {
                int leftHeight = (left_ ? left_->height_ : 0);
                int rightHeight = (right_ ? right_->height_ : 0);
                balanceFactor_ = rightHeight - leftHeight;
                height_ = std::max (leftHeight, rightHeight) + 1;
            }
    }; 

    class Tree final {
        private:
            std::vector <Node*> nodes_ {};
            Node* head_ = nullptr;
            
            //  SHALLOW SWAP
            void ShallowSwap (Tree& rhs) {
                std::swap (this->head_, rhs.head_);
                std::swap (this->nodes_, rhs.nodes_);
            }

            //  INSERTION
            Node* InsertRecursive (char key, Node* node) {
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
            class Iterator final {
                private:
                    Node* node_ = nullptr;
                    Tree* tree_ = nullptr;
                    
                    //  SERVICE CTOR
                    Iterator (Tree* tree, Node* node):
                        tree_ (tree),
                        node_ (node)
                        {}
                    friend Iterator MakeIterator (Tree* tree, Node* node);
                    
                public:
                    //  CTOR
                    Iterator ():
                        node_ (nullptr),
                        tree_ (nullptr)
                        {}

                    //  OPERATORS
                    const char operator * () const { return node_->key_; }
                    template <typename T>
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

            //  CTORS
            Tree ():
                nodes_ ({}),
                head_ (nullptr)
                {}  
            Tree (std::vector <char>& data):
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
                    std::cerr << "Copy ctor" << std::endl;
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
                std::cerr << "Copy assign" << std::endl;
                if (this != &rhs) {
                    Tree temp { rhs };
                    ShallowSwap (temp);
                }
                return *this;
            }

            //  MOVE
            Tree (Tree&& rhs) noexcept {
                std::cerr << "Move ctor" << std::endl;
                ShallowSwap (rhs);
            }
            Tree& operator = (Tree&& rhs) {
                std::cerr << "Move assign" << std::endl;
                if (this != &rhs) {
                    ShallowSwap (rhs);
                }
                return *this;
            }

            //  MODIFIERS
            void Clear () { nodes_.clear (); }
            Node* Insert (char key) {
                Node* result = InsertRecursive (key, head_);
                if (nodes_.size () == 1) {
                    head_ = result;
                }
                return result;
            }

            Node* RemoveMin (Node* node) {
                return (node->left_ ? node->left_ = RemoveMin (node->left_), Balance (node) : node->right_);
            }

            Node* ExtractRecursive (char key, Node* node) {
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
                    nodes_[node->id_] = nullptr;
		            delete node;
		            if (!rhs) {
                        return lhs;
                    }

		            Node* min = rhs;
                    while (min->left_) {
                        min = min->left_;
                    }
                    Node* temp = min->right_;
                    if (rhs->left_) {
                        //? removemin убрать бы
                        rhs->left_ = RemoveMin (rhs->left_);
                        min->right_ = Balance (rhs);
                    }
                    else { 
                        min->right_ = rhs->right_;
                    };
		            min->left_ = rhs;
		            return Balance (min);
                }
                return Balance (node);
            }

            Node* Extract (char key) {
                Node* result = ExtractRecursive (key, head_);
            }
            //?

            //  ITERATORS
            friend Iterator MakeIterator (Tree* tree, Node* node) {
                return { tree, node };
            }
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

            //  CAPACITY
            size_t Size () { return nodes_.size (); }
            bool Empty () { return nodes_.size (); }

            //  DOT IMAGE
            void MakeDot (std::ofstream* outfile) {
                *outfile << "digraph G {" << std::endl << "fontsize = 50" << std::endl;
                if (head_) {
                    *outfile << head_->id_ << "[label = \"" << head_->key_ << "\"]" << std::endl;
                    MakeDotRecursive (outfile, head_);
                }
                *outfile << "}";
            }
    };
}