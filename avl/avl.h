#ifndef __AVL_H__
#define __AVL_H__

#include <iostream>

namespace avl {

	template <typename T>
	class avlNode {
		public:
			avlNode(const T& value) : _data(value), _left(NULL), _right(NULL) {}
			avlNode() : _left(NULL), _right(NULL) {}

			inline T&           get() {return _data;}
			inline const T&     get() const {return _data;}
			inline avlNode*     getLeft() const {return _left;}
			inline avlNode*     getRight() const {return _right;}

			inline void         set(const T& value) {_data = value;}
			inline void         setLeft(avlNode* left) {_left = left;}
			inline void         setRight(avlNode* right) {_right = right;}

		private:
			avlNode(const avlNode& other);
			avlNode& operator=(const avlNode& right);

			T            _data;
			avlNode     *_left;
			avlNode     *_right;
	};

	template <typename T>
	class avlTree {
		public:
			avlTree() : _root(NULL) {}
			~avlTree() {_clean();}

			void     print() const {_print(_root);}
			void     add(T& nObj);

		private:
			avlTree(const avlTree& other);
			avlTree& operator=(const avlTree& rhs);

			inline void    _clean() {
				if (!_root) return;
				// We need a post-order traversal here, and we would be
				// using the famous trick of right-rotation to achieve the
				// traversal without stack or recursion
				avlNode<T>* iter = _root;
				while (iter) {
					if (iter->getLeft()) {
						avlNode<T>* l = iter->getLeft();
						iter->setLeft(l->getRight());
						l->setRight(iter);
						iter = l;
					} else {
						avlNode<T>* toDelete = iter;
						iter = iter->getRight();
						delete toDelete;
					}
				}
			}

			void    _print(const avlNode<T>* root) const;
			void    _rebalanceNodes();

			avlNode<T>    *_root;
	};

	template <typename T>
	void avlTree<T>::add(T& nObj)
	{
		if (!_root) {
			_root = new avlNode<T>(nObj);
			return;
		}
		avlNode<T>* iter = _root;
		while (iter) {
			if (iter->get() < nObj) {
				if (iter->getLeft()) {
					iter = iter->getLeft();
				} else {
					iter->setLeft(new avlNode<T>(nObj));
					break;
				}
			} else {
				if (iter->getRight()) {
					iter = iter->getRight();
				} else {
					iter->setRight(new avlNode<T>(nObj));
					break;
				}
			}
		}
		_rebalanceNodes();
	}

	template <typename T>
	void avlTree<T>::_print(const avlNode<T>* root) const {
		if (!root) return;
		_print(root->getLeft());
		std::cout << root->get() << std::endl;
		_print(root->getRight());
	}

	template <typename T>
	void avlTree<T>::_rebalanceNodes()
	{
	}

}

#endif // __AVL_H__
