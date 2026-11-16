// src/memory/memory_index.cpp — YOU IMPLEMENT. Spec §4.3.

#include "memory/memory_index.h"
#include <algorithm>
#include <utility>
MemoryIndex::~MemoryIndex() {
clear();}
 void MemoryIndex::destroy(Node* Nde){
    if(Nde==nullptr){
        return;
    }
    destroy(Nde->left);
    destroy(Nde->right);
    delete Nde;


 }



bool MemoryIndex:: insert(MemoryRecord rec){
        Node* newnNde = new Node{std::move(rec),nullptr,nullptr};
        Node* Nde = root_;
        if(Nde == nullptr){
            root_ = newnNde;
            count_++;
            return 1;
        }
        while (true){
            if(newnNde->rec.id == Nde->rec.id){
                delete newnNde;
                return 0;
            }
            if(newnNde->rec.id < Nde->rec.id){
                if(Nde->left == nullptr){
                    Nde->left = newnNde;
                    count_++;
                    return 1;
                }
                else{Nde = Nde->left;}
            }
            if(newnNde->rec.id > Nde->rec.id){
                if(Nde->right == nullptr){
                    Nde->right = newnNde;
                    count_++;
                    return 1;
                }
                else{Nde = Nde->right;}
            }

        }

    }

const MemoryRecord* MemoryIndex::find(std::uint64_t id) const {
    Node* Nde = root_;
        if(Nde == nullptr){
            return nullptr;
        }
        while (true){
            if(id == Nde->rec.id){
                return &Nde->rec;
            }
            if(id < Nde->rec.id){
                if(Nde->left == nullptr){
                    return nullptr;
                }
                else{Nde = Nde->left;}
            }
            if(id > Nde->rec.id){
                if(Nde->right == nullptr){
                    return nullptr;
                }
                else{Nde = Nde->right;}
            }

        }
}

bool MemoryIndex::remove(std::uint64_t id) {
    if(root_ == nullptr){
        return 0;
    }
    Node* Nde = root_;
    Node* Ndeparent = nullptr;
    int children = 0; //0 child, 1 child, 3 child
    while (true){
            if(id == Nde->rec.id){
                if((Nde->left == nullptr)&&(Nde->right == nullptr)){
                    children = 0;
                    break;
                }
                if((Nde->left == nullptr)||(Nde->right == nullptr)){
                    children = 1;
                    break;
                }
                if((Nde->left != nullptr)&&(Nde->right != nullptr)){
                    children = 2;
                    break;
                }

            }
            if(id < Nde->rec.id){
                if(Nde->left == nullptr){
                    return 0;
                }
                else{Ndeparent = Nde;
                    Nde = Nde->left;}
            }
            if(id > Nde->rec.id){
                if(Nde->right == nullptr){
                    return 0;
                }
                else{Ndeparent = Nde;
                    Nde = Nde->right;}
            }}

        if(children == 0 ){
            if(Ndeparent == nullptr){
            root_ = nullptr; }
            else if(Ndeparent->left == Nde){
                Ndeparent->left = nullptr;
            }
            else{Ndeparent->right = nullptr;
            }
            delete(Nde);
            
        }
        if(children == 1){
            Node*  temp;
            if(Nde->left !=nullptr){temp = Nde->left;}
            else{temp = Nde->right;}

            if(Ndeparent == nullptr){
            root_ = temp; }
            else if(Ndeparent->left == Nde){
                Ndeparent->left = temp;
            }
            else{Ndeparent->right = temp;
            }
            delete(Nde);        
        }
        if(children ==2){
            Node*  temp = Nde->right;
            Node*  tempparent=Nde;
            while(temp->left != nullptr){
                tempparent = temp;
                temp = temp->left;
            }
            Nde->rec = std::move(temp->rec);

            if(tempparent == Nde){
            tempparent->right = temp->right;}
            else{tempparent->left = temp->right;}
            delete(temp);

        

        }
        count_--;
        return 1;

    // TODO: leaf, one child, two children (use the in-order successor or
    // predecessor). Spec Appendix C draws all three.
    // Clue: the successor (leftmost node of the right subtree) has no left
    // child but MAY have a right child: that child takes its place.
    // Clue: --count_ exactly once per successful remove. Counting inside
    // the helper AND here double-counts the two-children case.
    // Clue: when the removed node is the root, root_ itself must change.
    
    
}

std::size_t MemoryIndex::size() const noexcept { return count_; }

std::size_t MemoryIndex::height() const {
    Node* Nde = root_;
    if(root_ == nullptr){
        return 0;
    }

    // TODO: empty tree = 0, one node = 1, otherwise 1 + the taller child.
    return (std::max(height_recur(Nde->left), height_recur(Nde->right)) +1 );
}
 std::size_t MemoryIndex::height_recur( Node* Nde) {
        if(Nde == nullptr){
            return 0;
        }
        return (std::max(height_recur(Nde->left), height_recur(Nde->right)) +1 );
 }



void MemoryIndex::for_each_in_order(const std::function<void(const MemoryRecord&)>& fn) const {
    // TODO: left subtree, then fn(node->rec), then right subtree.
    in_order_recur(root_,fn);

}
void MemoryIndex::in_order_recur(const Node* Nde, const std::function<void(const MemoryRecord&)>& fn){
    if(Nde == nullptr){
        return;
    }
    in_order_recur(Nde->left,fn);
    fn(Nde->rec);
    in_order_recur(Nde->right,fn);
}
void MemoryIndex::clear() {
    destroy(root_);
    root_ = nullptr;
    count_ = 0;
    // TODO
    // Clue: after freeing the nodes, set root_ = nullptr and count_ = 0, or
    // the destructor frees them a second time.
}
