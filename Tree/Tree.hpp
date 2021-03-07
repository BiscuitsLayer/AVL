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

    class Iterator final {

    };

    class Tree final {
        private:
            Node* head_ = nullptr;
            std::vector <Node*> nodes_ {};

            //  INSERTION
            Node* InsertRecursive (char key, Node* node) {
                if (!node) {
                    node = new Node (key);
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

            //  DOT IMAGE
            void MakeDotRecursive (std::ofstream* outfile, Node* node) {
                if (!node) { return; }
                *outfile << node->id_ << "[label = \"" << node->key_ << "\"]" << std::endl;
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
            friend Iterator;

            //  CTORS
            Tree ():
                head_ (nullptr)
                {}  
            Tree (std::vector <char>& data) {
                for (auto&& item : data) {
                    Insert (item);
                }
            }

            //  COPY
            Tree (const Tree& rhs) = delete;
            Tree& operator = (Tree& rhs) = delete;

            //  MOVE
            Tree (Tree&& rhs) = delete;
            Tree& operator = (Tree&& rhs) = delete;

            //  DTOR
            ~Tree () {
                for (auto&& node : nodes_) {
                    delete node;
                }
            }

            //  INSERTION AND DELETION
            Node* Insert (char key) {
                head_ = InsertRecursive (key, head_);
                return head_;
            }
            void Delete (Iterator nodeIt) {

            }

            //  ROTATIONS
            Node* RotateLeft (Node* node) {
                Node* temp = node->right_;
                node->right_ = temp->left_;
                temp->left_ = node;
                node->Update ();
                temp->Update ();
                return temp;
            }
            Node* RotateRight (Node* node) {
                Node* temp = node->left_;
                node->left_ = temp->right_;
                temp->right_ = node;
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
            void MakeDot (std::ofstream* outfile) {
                *outfile << "digraph G {" << std::endl << "fontsize = 50" << std::endl;
                MakeDotRecursive (outfile, head_);
                *outfile << "}";
            }
    };
}

