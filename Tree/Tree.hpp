#pragma once

#include <iostream>
#include <vector>

#include "../Settings/Settings.hpp"

//  when you finish, dont forget to subst. char with T

namespace AVL {
    struct Node final {
        char key_ {};
        int height_ = 1;
        int balanceFactor_ = 0;
        int id_ = -1;
        Node* left_ = nullptr;
        Node* right_ = nullptr;

        Node (char key):
            key_ (key),
            height_ (1),
            balanceFactor_ (0),
            id_ (-1),
            left_ (nullptr),
            right_ (nullptr)
            {}

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
            
            //  SWAP
            void Swap (Tree& rhs) {
                this->nodes_ = rhs.nodes_;
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
                else {
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

                public:

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

            //  COPY
            Tree (const Tree& rhs):
                nodes_ (rhs.nodes_),
                head_ (nodes_[rhs.head_->id_])
                {
                    std::cerr << "Copy ctor" << std::endl;
                    for (auto&& node : nodes_) {
                        if (node->left_) {
                            node->left_ = nodes_[node->left_->id_];
                        }
                        if (node->right_) {
                            node->right_ = nodes_[node->right_->id_];
                        }
                    }
                }
            Tree& operator = (Tree& rhs) {
                std::cerr << "Copy assign" << std::endl;
                Tree temp { rhs };
                Swap (rhs);
                return *this;
            }

            //  MOVE
            Tree (Tree&& rhs) = delete;
            Tree& operator = (Tree&& rhs) = delete;

            //  MODIFIERS
            void Clear () { nodes_.clear (); }
            Node* Insert (char key) {
                Node* result = InsertRecursive (key, head_);
                if (nodes_.size () == 1) {
                    head_ = result;
                }
                return result;
            }
            void Extract (char key) {

            }


            //  CAPACITY
            size_t Size () {
                return nodes_.size ();
            }
            bool Empty () {
                return nodes_.size ();
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
    };
}