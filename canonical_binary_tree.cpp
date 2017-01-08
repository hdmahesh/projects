//!
//! Problem 12.4:
//! Given a set of binary trees A It ... , An how would you compute a new
//! set of binary trees 8 1, ..• , B; such that for each i, 1 SiS n, AI and 8 1 are isomorphic,
//! and no pair of isomorphic nodes exists in the set of nodes defined by BIt ... , Bn. (This
//! is sometimes referred to as the canonical form.) Assume nodes are not shared in
//! AI, ... ,An· See Figure 12.2 for an example.
//!

#include <iostream>
#include <vector>
#include <unordered_map>

template <typename T>
class bNode {
	public:
		bNode() : _value(T()), _left(NULL), _right(NULL), _hash(0), _refCount(0) {}
		~bNode() {
			if (_left)  {deleteNode(_left);  _left = NULL;}
			if (_right) {deleteNode(_right); _right = NULL;}
		}

		inline T&      getValue() {return _value;}
		inline bNode*  getLeft() {return _left;}
		inline bNode*  getRight() {return _right;}

		inline void    setValue(T& value) {_value = value;}
		inline void    setRight(bNode* r) {
			if (_right) {
				deleteNode(_right); incrRefCount(r);
			}
			_right = r;
		}
		inline void    setLeft(bNode* l) {
			if (_left) {
				deleteNode(_left); incrRefCount(l);
			}
			_left = l;
		}

		inline void    print() const {
			printf("Hash = %10ld, Object = %p, Value = ", _hash, this);
			std::cout << _value << std::endl;
		}

		inline unsigned int  getHash() {
			if (_hash) return _hash;
			int nFactor = 3 * getValue();
			int lnFactor = getLeft()  == NULL ? 5 : 5 * getLeft()->getHash();
			int rnFactor = getRight() == NULL ? 7 : 7 * getRight()->getHash();
			_hash = nFactor + lnFactor + rnFactor;
			return _hash;
		}

		inline static void  deleteNode(bNode<T>*& n) {
			if (!n->_refCount) {
				delete n; n = NULL;
			} else {
				n->_refCount--;
			}
		}
		inline static void  incrRefCount(bNode<T>*& n) {if (n) n->_refCount++;};

	private:
		bNode(const bNode& other);
		bNode& operator=(const bNode& rhs);

		T       _value;
		bNode*  _left;
		bNode*  _right;

		//! Used in canonicalization
		unsigned long  _hash;
		unsigned int   _refCount;
};

template <typename T>
class binSTree {
	public:
		typedef std::pair<unsigned long, bNode<T>*>           bNodeHashPair;
		typedef std::unordered_map<unsigned long, bNode<T>*>  bNodeHashMap;

		enum order {
			PRE_ORDER,
			IN_ORDER,
			POST_ORDER
		};

		binSTree() : _size(0), _root(NULL) {}
		~binSTree() {
			if (_root) {
				bNode<T>::deleteNode(_root);
			}
		}

		inline size_t     size() const {return _size;}
		inline bNode<T>*  getRoot() {return _root;}

		inline void       setRoot(bNode<T>* newRoot) {
			if (_root) bNode<T>::deleteNode(_root);
			_root = newRoot;
			bNode<T>::incrRefCount(_root);
		}

		inline void       add(T& value) {
			++_size;
			bNode<T>* nn = new bNode<T>();
			nn->setValue(value);
			if (!_root) {
				_root = nn;
			} else {
				_add(_root, nn);
			}
		}

		inline void       print(order ot = IN_ORDER) const {
			std::cout << "Printing tree in order : " << ot << std::endl;
			switch (ot) {
				case PRE_ORDER:  _printPre(_root); break;
				case POST_ORDER: _printPost(_root); break;
				case IN_ORDER:
				default: _printIn(_root); break;
			}
		}

		inline unsigned long  getHash() {return _root ? _root->getHash() : 0;}

		static void canonicalize(std::vector<binSTree<T>*>& trees);

	private:
		binSTree(const binSTree& other);
		binSTree& operator=(const binSTree& rhs);

		inline void    _add(bNode<T>* node, bNode<T>* nn) {
			if (nn->getValue() <= node->getValue()) {
				if (!node->getLeft()) {
					node->setLeft(nn);
				} else {
					_add(node->getLeft(), nn);
				}
			} else {
				if (!node->getRight()) {
					node->setRight(nn);
				} else {
					_add(node->getRight(), nn);
				}
			}
		}
		inline void    _printPre(bNode<T>* n) const {
			if (!n) return;
			n->print();
			_printPre(n->getLeft());
			_printPre(n->getRight());
		}
		inline void    _printPost(bNode<T>* n) const {
			if (!n) return;
			_printPost(n->getLeft());
			_printPost(n->getRight());
			n->print();
		}
		inline void    _printIn(bNode<T>* n) const {
			if (!n) return;
			_printIn(n->getLeft());
			n->print();
			_printIn(n->getRight());
		}
		static inline void    _canonicalizeChildren(bNodeHashMap& hMap, bNode<T>* n) {
			// left
			bNode<T>* child = n->getLeft();
			if (child) {
				unsigned long hash = child->getHash();
				if (hMap.count(hash)) {
					n->setLeft(hMap.find(hash)->second);
				} else {
					hMap.insert(bNodeHashPair(hash, child));
					_canonicalizeChildren(hMap, child);
				}
			}
			// right
			child = n->getRight();
			if (child) {
				unsigned long hash = child->getHash();
				if (hMap.count(hash)) {
					n->setRight(hMap.find(hash)->second);
				} else {
					hMap.insert(bNodeHashPair(hash, child));
					_canonicalizeChildren(hMap, child);
				}
			}
		}

		size_t      _size;
		bNode<T>*   _root;
};

template <typename T>
void
binSTree<T>::canonicalize(std::vector<binSTree<T>*>& trees)
{
	bNodeHashMap hMap;
	for (size_t index = 0; index < trees.size(); ++index) {
		binSTree<T>*  bst  = trees[index];
		unsigned long hash = bst->getHash(); 
		if (hMap.count(hash)) {
			bst->setRoot(hMap.find(hash)->second);
		} else {
			hMap.insert(bNodeHashPair(hash,bst->getRoot()));
			_canonicalizeChildren(hMap, bst->getRoot());
		}
	}
}

int
main()
{
	binSTree<int>    intBST1;
	int value = 10;
	intBST1.add(value);
	intBST1.add(value = 12);
	intBST1.add(value = 21);
	intBST1.add(value = 5);
	intBST1.add(value = 7);
	intBST1.add(value = 1);

	binSTree<int>    intBST2;
	intBST2.add(value = 5);
	intBST2.add(value = 1);
	intBST2.add(value = 7);

	binSTree<int>    intBST3;
	intBST3.add(value = 12);
	intBST3.add(value = 21);

	binSTree<int>    intBST4;
	intBST4.add(value = 122);

	std::vector<binSTree<int>*> trees;
	trees.push_back(&intBST1);
	trees.push_back(&intBST2);
	trees.push_back(&intBST3);
	trees.push_back(&intBST4);
	binSTree<int>::canonicalize(trees);
	intBST1.print();
	intBST2.print();
	intBST3.print();
	intBST4.print();
	return 0;
}
