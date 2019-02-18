#ifndef PROJ4_RBTREE_H
#define PROJ4_RBTREE_H

#include "Node.h"
#include <iostream>
#include <bits/stdc++.h>
#include <queue>
using namespace std;

template <typename Key, typename Element>
class RBTree {
private:
	// Add class members here
	Node<Key, Element>* r;
	size_t s;
public:
	// Implement each of these methods
	RBTree() {
		this->r = NULL;
		this->s = 0;	
	}
	void insert(const Key& k, const Element& e);
	void fixViolation(Node<Key, Element> *&p);
	void rotateLeft(Node<Key, Element> *p);
	void rotateRight(Node<Key, Element> *p);
	Node<Key, Element>* search(const Key& k);
	void del(const Key& k);
	void fixDelete(Node<Key, Element>* parent, Node<Key, Element>* node);
	Node<Key, Element>* inOrderSuccessor(Node<Key, Element>* p);
    	void inorder(ostream& out) const;
	void inorderHelper(ostream& out, Node<Key, Element>* r) const;
    	void levelOrder(ostream& out) const;
	void levelOrderHelper(ostream& out, Node<Key, Element>* r) const;
	size_t size() const;
	Node<Key, Element>* getRoot();
};


// Function to insert a new node with given data
template <typename Key, typename Element>
void RBTree<Key, Element>::insert(const Key& k, const Element& e) {
	Node<Key, Element>* p = new Node<Key, Element>();
	Node<Key, Element>* x;
	Node<Key, Element>* y;
	p->setKey(k);
	p->setElement(e);
	x = this->r;
	y = NULL;
	if(this->r == NULL){
		this->r = p;
		p->setParent(NULL);	
	}else{
		while(x != NULL){
			y = x;
			if(x->getKey() > p->getKey()) x = x->getLeft();			
			else x = x->getRight();		
		}
		p->setParent(y);
		if(y->getKey() < p->getKey()) y->setRight(p);
		else y->setLeft(p);		
	}
	fixViolation(p);
	this->s++;
}

template <typename Key, typename Element>
void RBTree<Key, Element>::fixViolation(Node<Key, Element>* &p){
	Node<Key, Element>* p_p = NULL;
	Node<Key, Element>* gp_p = NULL;
	Node<Key, Element>* u_p;
	while((p->getColor() != BLACK) && (p != this->r) && (p->getParent()->getColor() == RED)){
		p_p = p->getParent();
		gp_p = p->getParent()->getParent();
		//Case A:
		if(p_p == gp_p->getLeft()){
			u_p = gp_p->getRight();
			//Case 1:
			if(u_p != NULL && u_p->getColor() == RED){
				gp_p->setColor(RED);
				p_p->setColor(BLACK);
				u_p->setColor(BLACK);
				p = gp_p;		
			}else{
				//Case 2:
				if(p == p_p->getRight()){
					rotateLeft(p_p);
					p = p_p;
					p_p = p->getParent();
				}
				//Case 3:
				rotateRight(gp_p);
				color t1 = p_p->getColor();
				color t2 = gp_p->getColor();
				p_p->setColor(t2);
				gp_p->setColor(t1);
				p = p_p;
			}	
		//Case B:		
		}else{
			u_p = gp_p->getLeft();
			//Case 1:
			if((u_p != NULL) && (u_p->getColor() == RED)){
				gp_p->setColor(RED);
				p_p->setColor(BLACK);
				u_p->setColor(BLACK);
				p = gp_p;		
			}else{
				//Case 2:
				if(p == p_p->getLeft()){
					rotateRight(p_p);
					p = p_p;
					p_p = p->getParent();
				}
				//Case 3 :
				rotateLeft(gp_p);
				color t1 = p_p->getColor();
				color t2 = gp_p->getColor();
				p_p->setColor(t2);
				gp_p->setColor(t1);
				p = p_p;
			}	
		}
	}
	r->setColor(BLACK);
}

template <typename Key, typename Element>
void RBTree<Key, Element>::rotateLeft(Node<Key, Element> *p){
	if(p->getRight() == NULL) return;	
	Node<Key, Element>* p_r = p->getRight();
 	p->setRight(p_r->getLeft()); 
	if (p->getRight() != NULL)p->getRight()->setParent(p);
	p_r->setParent(p->getParent());
	if (p->getParent() == NULL)r = p_r;
	else {
		if (p == p->getParent()->getLeft())p->getParent()->setLeft(p_r);
		else p->getParent()->setRight(p_r);
	}	
	p_r->setLeft(p);
	p->setParent(p_r);
}

template <typename Key, typename Element>
void RBTree<Key, Element>::rotateRight(Node<Key, Element> *p){
	if(p->getLeft() == NULL) return;
	Node<Key, Element>* p_l = p->getLeft();
 	p->setLeft(p_l->getRight()); 
	if (p->getLeft() != NULL)p->getLeft()->setParent(p);
	p_l->setParent(p->getParent());
	if (p->getParent() == NULL)r = p_l;
	else {
		if (p == p->getParent()->getLeft())p->getParent()->setLeft(p_l);
		else p->getParent()->setRight(p_l);
	}
	p_l->setRight(p);
	p->setParent(p_l);
}

// Function to search a node with given data
template <typename Key, typename Element>
Node<Key, Element>* RBTree<Key, Element>::search(const Key& k) {
	Node<Key, Element>* e = this->r;
	while(e != NULL){
		if(e->getKey() == k) return e;
		if(k > e->getKey()) e = e->getRight();
		else e = e->getLeft();
	}
	return NULL;
}

// Function to delete a node with given data
template <typename Key, typename Element>
void RBTree<Key, Element>::del(const Key& k) {
	Node<Key, Element> *n = search(k);
	if(n == NULL) return;
	this->s--;
	Node<Key, Element> *child, *parent;
	color color;
	if(n->getLeft() == NULL || n->getRight() == NULL) {
		if(n->getLeft() == NULL) child = n->getRight();
		else child = n->getLeft();
		parent = n->getParent();
		color = n->getColor();
		if(child) child->setParent(parent);
		if(parent){
			if(parent->getLeft() == n)parent->setLeft(child);
			else parent->setRight(child);	
		}else{
			this->r = child;	
		}
		
		if(color == BLACK) fixDelete(parent, child);
		delete n;
	}else{
		Node<Key, Element> *replace;
		replace = inOrderSuccessor(n);
		Node<Key, Element> *oldParent = n->getParent();
		if(oldParent){
			if(oldParent->getLeft() == n) oldParent->setLeft(replace);
			else oldParent->setRight(replace);
		}else this->r = replace;
		child = replace->getRight();
		parent = replace->getParent();
		color = n->getColor();
		if(parent == n){
			parent = replace;
			if(color == RED) replace->getRight()->setColor(BLACK);		
		}else{
			if(child) child->setParent(parent);
			parent->setLeft(child);
			replace->setRight(n->getRight());
			n->getRight()->setParent(replace);		
		}
		replace->setParent(n->getParent());
		replace->setColor(n->getColor());
		replace->setLeft(n->getLeft());
		n->getLeft()->setParent(replace);
		
		if(color == BLACK) fixDelete(parent, child);
		
		delete n;
	}
}

template <typename Key, typename Element>
void RBTree<Key, Element>::fixDelete(Node<Key, Element>* parent, Node<Key, Element>* node){
	//Node<Key, Element> *parent = node->getParent();
	Node<Key, Element> *subling;
	while((!node || node->getColor() != BLACK) && (node != this->r)){
		if(parent->getLeft() == node){			
			subling = parent->getRight();			
			if(subling->getColor() == RED){			
				subling->setColor(BLACK);				
				parent->setColor(RED);
				rotateLeft(parent);
				subling = parent->getRight();				
			}
			if((subling->getLeft() == NULL || subling->getLeft()->getColor() == BLACK) && (subling->getRight() == NULL || subling->getRight()->getColor() == BLACK)){				
				subling->setColor(RED);
				if(node){
					node = node->getParent();
					//node->setParent(parent->getParent());
					parent = node->getParent();
					subling->getParent()->setColor(BLACK);
				}else{
					subling->getParent()->setColor(BLACK);
					break;				
				}
			}else{
				if(subling->getRight() == NULL || subling->getRight()->getColor() == BLACK){
					subling->getLeft()->setColor(BLACK);
					subling->setColor(RED);
					rotateRight(subling);
					subling = parent->getRight();	
				}			
				subling->setColor(parent->getColor());
				parent->setColor(BLACK);
				subling->getRight()->setColor(BLACK);
				rotateLeft(parent);
				node = this->r;
				break;
			}				
		
		}else{	
			//here is all right
			subling = parent->getLeft();
			if(subling->getColor() == RED){
				subling->setColor(BLACK);
				parent->setColor(RED);
				rotateRight(parent);
				subling = parent->getLeft();//?
			}
			if((subling->getLeft() == NULL || subling->getLeft()->getColor() == BLACK) && (subling->getRight() == NULL || subling->getRight()->getColor() == BLACK)){
					subling->setColor(RED);
					//cout << node->getKey();
					if(node){
						node = node->getParent();
						parent = node->getParent();
						subling->getParent()->setColor(BLACK);
					}else{
						subling->getParent()->setColor(BLACK);
						break;
					}
			}else{
				if(subling->getLeft() == NULL || subling->getLeft()->getColor() == BLACK){
					subling->getRight()->setColor(BLACK);
					subling->setColor(RED);
					rotateLeft(subling);
					subling = parent->getLeft();				
				}
				subling->setColor(parent->getColor());
				parent->setColor(BLACK);
				subling->getLeft()->setColor(BLACK);
				rotateRight(parent);
				node = this->r;
				break;
			
			}
		}
	}
	if(node) node->setColor(BLACK);	
}

template <typename Key, typename Element>
Node<Key, Element>* RBTree<Key, Element>::inOrderSuccessor(Node<Key, Element>* p){
	if(p == NULL) return NULL;
	Node<Key, Element>* result = p->getRight();	
	while(result->getLeft() != NULL)result = result->getLeft();
	return result;
}

// Function to do inorder traversal
template <typename Key, typename Element>
void RBTree<Key, Element>::inorder(ostream& out) const { inorderHelper(out, this->r); }

template <typename Key, typename Element>
void RBTree<Key, Element>::inorderHelper(ostream& out, Node<Key, Element>* r) const {
	if(r == NULL) return;
	inorderHelper(out, r->getLeft());
	out << *r << endl;	
	inorderHelper(out, r->getRight());
}

// Function to perform level-order traversal
template <typename Key, typename Element>
void RBTree<Key, Element>::levelOrder(ostream& out) const { levelOrderHelper(out, this->r); }

template <typename Key, typename Element>
void RBTree<Key, Element>::levelOrderHelper(ostream& out, Node<Key, Element>* r) const {
	if(r == NULL) return;
	std::queue<Node<Key, Element>*> q;
	q.push(r);
	while(!q.empty()){
		Node<Key, Element>* t = q.front();
		out << *t << endl;
		q.pop();
		if(t->getLeft() != NULL)q.push(t->getLeft());
		if(t->getRight() != NULL)q.push(t->getRight());	
	}
}

// Function to return the number of nodes in the RBTree
template <typename Key, typename Element>
size_t RBTree<Key, Element>::size() const { return this->s; }

// Function to return a pointer to the root of the RBTree
template <typename Key, typename Element>
Node<Key, Element>* RBTree<Key, Element>::getRoot() { return this->r; }


// The following functions have been provided for you, and do not
// need to be modified:


// Output stream operator overload -- writes the contents of the
// entire RBTree to the specified output stream, using an in-order
// traversal. Note that there must be a similar operator overload
// for Key and Element types in order for this to compile.
// Example usage:
//		RBTree<K,E> rbt;
//		cout << rbt << endl;
template <typename Key, typename Element>
ostream& operator<<(ostream& out, const RBTree<Key, Element>& rbtree) {
	out << rbtree.size() << endl;
	rbtree.inorder(out);
	return out;
}

// Input stream operator overload -- reads the contents of an RBTree
// from the specified input stream, inserting each Node as it is
// read. Note that there must be a similar operator overload for Key
// and Element types in order for this to compile.
// Example usage:
//		RBTree<K,E> rbt;
//		cin >> rbt;
template <typename Key, typename Element>
istream& operator>>(istream& in, RBTree<Key, Element>& rbtree) {
	// Clear existing contents, if any
	rbtree = RBTree<Key, Element>();

	// Read in the number of elements to read
	size_t size;
	in >> size;
	for (size_t i = 0; i < size; i++) {
		// Read in each node as it appears in the stream
		Node<Key, Element> n;
		in >> n;
		// Insert the node into the RBTree
		rbtree.insert(n.getKey(), n.getElement());
	}

	return in;
}

#endif //PROJ4_RBTREE_H
