#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <utility>
#include <ostream>
#include <set>

template <class T>
class lessThan {
	public:
		bool    operator()(const T& left, const T& right) {return (left < right);}
};

template <>
class lessThan<const char*> {
	public:
		bool    operator()(const char*& left, const char*& right) {return (strcmp(left, right) < 0);};
};

template <typename T, class lt>
class minHeap {
	public:
		minHeap() {}
		~minHeap() {}

		const T&     getMin() const {
			if (_heap.size()) return _heap[0];
			static T def;
			return def;
		}

		void         popMin() {
			if (!_heap.size()) return;
			std::swap(_heap[0], _heap[_heap.size()-1]);
			_heap.pop_back();
			_heapify(0);
		}

		void          insert(T&& value) {
			_heap.push_back(std::move(value));
			_correctHeap();
		}

		inline void   print() const {
			std::for_each(_heap.begin(), _heap.end(), [](const T& val){std::cout << val << std::endl;});
		}

		inline size_t size() const {return _heap.size();} 

	private:
		typedef std::vector<T>        Array;

		void         _correctHeap() {
			for (int index = _heap.size() / 2; index >= 0; --index) {
				_heapify(index);
			}
		}

		void         _heapify(int index) {
			int left  = (2*(index+1)) - 1;
			int right = (2*(index+1) + 1) - 1;
			int smallest = index;
			if (left < _heap.size() && _comparator(_heap[left], _heap[index]))
				smallest = left;
			if (right < _heap.size() && _comparator(_heap[right], _heap[smallest]))
				smallest = right;
			if (smallest != index) {
				std::swap(_heap[index], _heap[smallest]);
				_heapify(smallest);
			}
		}

		Array          _heap;
		lt             _comparator;
};

int
main2()
{
	minHeap<int,lessThan<int> > mhObj;
	mhObj.insert(14);
	mhObj.insert(32);
	mhObj.insert(42);
	mhObj.insert(100);

	while (mhObj.size()) {
		std::cout << mhObj.getMin() << std::endl;
		mhObj.popMin();
	}

	return 0;
}

int
main3()
{
	minHeap<const char*,lessThan<const char*> >    mhObj;
	mhObj.insert("z");
	mhObj.insert("x");
	mhObj.insert("a");
	mhObj.insert("c");

	while (mhObj.size()) {
		std::cout << mhObj.getMin() << std::endl;
		mhObj.popMin();
	}

	return 0;
}

#define FFL 
// {std::cout << __FILE__ << " : " << __LINE__ << " : " << __FUNCTION__ << std::endl;}

struct customObj {
	customObj() {FFL;};
	customObj(int v1, int v2, std::string s, double d) : val1(v1), val2(v2), sval(s), dval(d) {FFL;}
	customObj(customObj&& other) {
		FFL;
		val1 = other.val1;
		val2 = other.val2;
		dval = other.dval;
		sval = std::move(other.sval);
	}
	customObj& operator=(customObj&& rhs) {
		FFL;
		val1 = rhs.val1;
		val2 = rhs.val2;
		dval = rhs.dval;
		std::swap(this->sval, rhs.sval);
		return *this;
	}
	~customObj() {
		FFL;
	}
#if 0
	bool operator()(const customObj& l, const customObj& r) const {
		FFL;
		return (l.sval.compare(r.sval) < 0);
	}
#endif
	bool operator<(const customObj& rhs) const {
		FFL;
		return (sval.compare(rhs.sval) < 0);
	}
	friend std::ostream& operator<<(std::ostream& os, const customObj& obj) {
		os << obj.sval << std::endl;
		return os;
	}
	int          val1;
	int          val2;
	std::string  sval;
	double       dval;
};


struct sortDesc {
	bool operator()(const std::string& l, const std::string& r) {
		return l.compare(r) > 0;
	}
};

int
main()
{
	minHeap<customObj, lessThan<customObj> > mhObj;
	mhObj.insert(customObj(1, 2, "string10", 10.10));
	mhObj.insert(customObj(2, 3, "string9", 11.9));
	mhObj.insert(customObj(3, 4, "string12", 18.15));
	mhObj.insert(customObj(4, 5, "string2", 3.15));

	for (auto sVal : std::set<std::string, sortDesc>({"string10", "string9", "string12", "string2"})) std::cout << std::string(">> ") + sVal << std::endl;

	while (mhObj.size()) {
		std::cout << "<< " << mhObj.getMin();
		mhObj.popMin();
	}

	return 0;
}
