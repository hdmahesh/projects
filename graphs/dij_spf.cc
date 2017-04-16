#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <utility>
#include <ostream>
#include <set>
#include <list>
#include <map>

#define FFL 
// {std::cout << __FILE__ << " : " << __LINE__ << " : " << __FUNCTION__ << std::endl;}

typedef std::pair<int,int>                  VertexWeightInfo;
struct VertexWeightInfoComp {
	bool operator()(const VertexWeightInfo& left, const VertexWeightInfo& right) {return left.first < right.first;}
};

// typedef std::set<VertexWeightInfo, VertexWeightInfoComp>      VertexWeightInfoList;
typedef std::map<int, long>                  VertexWeightInfoList;
typedef std::map<int,VertexWeightInfoList>   AdjacencyList;

class adjacencyListBuilder {
	public:
		adjacencyListBuilder() {}
		~adjacencyListBuilder() {}

		const AdjacencyList&     getAJList() const {return _aj;};
		void                     addConnectivityInfo(int src, int dest, int weight) {
			auto it = _aj.find(src);
			if (it == _aj.end()) {
				_aj.emplace(std::pair<int,VertexWeightInfoList>(src, {VertexWeightInfo(dest,weight)}));
			} else {
				it->second.emplace(VertexWeightInfo(dest,weight));
			}
		}
		void                     addConnectivityInfoLeastWeight(int src, int dest, int weight) {
			auto it = _aj.find(src);
			if (it == _aj.end()) {
				_aj.emplace(std::pair<int,VertexWeightInfoList>(src, {VertexWeightInfo(dest,weight)}));
			} else {
				VertexWeightInfoList::iterator sit = it->second.find(dest);
				if (sit == it->second.end()) {
					it->second.emplace(VertexWeightInfo(dest,weight));
				} else if (sit->second > weight) {
					sit->second = weight;
				}
			}
		}

		void                     printAdjacencyMatrix(int numNodes) const {
			for (int index = 0; index < numNodes; ++index) {
				AdjacencyList::const_iterator it = _aj.find(index);
				for (int sindex = 0; sindex < numNodes; ++sindex) {
					if (it == _aj.end()) {
						std::cout << 0 << " ";
					} else {
						VertexWeightInfoList::const_iterator sit = it->second.find(sindex);
						if (sit == it->second.end()) {
							std::cout << 0 << " ";
						} else {
							std::cout << 1 << " ";
						}
					}
				}
				std::cout << std::endl;
			}
		}

		void                     justPrint() const {
			for (auto& it : _aj) {
				std::cout << it.first << " : " ;
				for (auto& sit : it.second) {
					std::cout << sit.first << " ";
				}
				std::cout << std::endl;
			}
		}

	private:
		AdjacencyList        _aj;
};

template <class T>
class lessThan {
	public:
		bool    operator()(const T& left, const T& right) {return (left < right);}
};

template <class T, class lt = lessThan<T> >
class minHeapPtr {
	public:
		minHeapPtr() {}
		~minHeapPtr() {}

		T*       getMin() {
			return _heap[0];
		}
		void     popMin() {
			popMinNoHeapify();
			_heapify(0);
		}
		void     popMinNoHeapify() {
			if (!_heap.size()) return;
			std::swap(_heap[0], _heap[_heap.size()-1]);
			_heap.pop_back();
		}
		void     insert(T* obj) {
			insertNoHeapify(obj);
			_correctHeap();
		}
		void     insertNoHeapify(T* obj) {_heap.push_back(obj);};
		void     print() const {
			for (auto it : _heap) std::cout << *it << std::endl;
		}
		size_t   size() const {return _heap.size();}
		void     heapify() {_correctHeap();}

	private:
		minHeapPtr(const minHeapPtr& other) = delete;
		minHeapPtr& operator=(const minHeapPtr& rhs) = delete;

		typedef std::vector<T*>        PtrArray;

		void    _correctHeap() {
			for (int si = _heap.size() / 2; si >= 0; --si) {
				_heapify(si);
			}
		}
		void    _heapify(int index) {
			int li = (2*(index+1)) - 1;
			int ri = (2*(index+1) +1) - 1;
			int sm = index;
			int sz = _heap.size();
			if (li < sz && _comp(*_heap[li], *_heap[sm])) sm = li;
			if (ri < sz && _comp(*_heap[ri], *_heap[sm])) sm = ri; 
			if (sm != index) {
				std::swap(_heap[index], _heap[sm]);
				_heapify(sm);
			}
		}

		PtrArray       _heap;
		lt             _comp;
};

class intc {
	public:
		intc(int val) : _val(val) {FFL;}
		~intc() {FFL;}

		bool    operator<(const intc& rhs) const {return _val < rhs._val;}
		friend std::ostream& operator<<(std::ostream& os, const intc& obj) {
			os << obj.get();
			return os;
		}
		int     get() const {return _val;}
		void    set(int v) {_val = v;}

	private:
		int      _val;
};

class dijSpf {
	public:
		dijSpf() {FFL;}
		~dijSpf() {FFL;}

		static void solve(unsigned int source, std::vector<std::vector<int>>& adjacencyMatrix) {
			std::vector<vertex>  nodeArr;
			size_t numNodes = adjacencyMatrix.size();
			for (size_t index = 0; index < numNodes; ++index) {
				nodeArr.emplace_back(vertex(index,std::numeric_limits<unsigned int>::max()));
			}
			nodeArr[source].setWeightFromSource(0);
			minHeapPtr<vertex>  mhObj;
			for(size_t index = 0; index < nodeArr.size(); ++index) mhObj.insertNoHeapify(&nodeArr[index]);
			mhObj.heapify();

			while (mhObj.size()) {
				vertex* u = mhObj.getMin();
				mhObj.popMinNoHeapify();
				size_t nodeIndex = u->getVertexNum();
				std::vector<int>& row = adjacencyMatrix[u->getVertexNum()];
				for (size_t index = 0; index < row.size(); ++index) {
					if (index == nodeIndex) continue;
					if (row[index] > 0) {
						vertex& v = nodeArr[index];
						if ((u->getWeightFromSource() + row[index]) < v.getWeightFromSource()) {
							v.setWeightFromSource(u->getWeightFromSource() + row[index]);
							v.setParent(nodeIndex);
						}
					}
				}
				mhObj.heapify();
			}
			for(auto it : nodeArr) {
				if (it.getVertexNum() != source) {
					std::cout << (int)it.getWeightFromSource() << " ";
				}
			}
			std::cout << std::endl;
		}

		static void solve(int numNodes, unsigned int source, const AdjacencyList& adjacencyList) {
			std::vector<vertex>  nodeArr;
			for (size_t index = 0; index < numNodes; ++index) {
				nodeArr.emplace_back(vertex(index,std::numeric_limits<unsigned long>::max()));
			}
			nodeArr[source].setWeightFromSource(0);
			minHeapPtr<vertex>  mhObj;
			for(size_t index = 0; index < nodeArr.size(); ++index) mhObj.insertNoHeapify(&nodeArr[index]);
			mhObj.heapify();

			while (mhObj.size()) {
				vertex* u = mhObj.getMin();
				mhObj.popMinNoHeapify();
				if (u->isEmitted() || ((long)u->getWeightFromSource() == -1)) {
					mhObj.heapify();
					continue;
				}
				u->setEmitted();
				size_t nodeIndex = u->getVertexNum();
				AdjacencyList::const_iterator it = adjacencyList.find(nodeIndex);
				if (it == adjacencyList.end()) continue;
				const VertexWeightInfoList& vAdjList = it->second;
				for (auto& it : vAdjList) {
					vertex& v = nodeArr[it.first];
					if (v.isEmitted()) continue;
					if ((u->getWeightFromSource() + it.second) < v.getWeightFromSource()) {
						v.setWeightFromSource(u->getWeightFromSource() + it.second);
						v.setParent(nodeIndex);
					}
				}
				mhObj.heapify();
			}
			for(size_t index = 0; index < nodeArr.size()-1; ++index) {
				if (nodeArr[index].getVertexNum() != source) {
					std::cout << (long)nodeArr[index].getWeightFromSource() << " ";
				}
			}
			std::cout << (long)nodeArr[nodeArr.size()-1].getWeightFromSource() << std::endl;
		}

	private:
		dijSpf(const dijSpf& other) = delete;
		dijSpf& operator=(const dijSpf& rhs) = delete;

		class vertex {
			public:
				vertex(unsigned int vNum, unsigned long weightFromSrc)
					: _vertexNum(vNum), _parent(-1), _weightFromSrc(weightFromSrc), _emitted(false), _otherVertexSpecificInfo(NULL)
				{FFL;}
				~vertex() {FFL;}

				bool             operator<(const vertex& rhs) const {
					return _weightFromSrc < rhs._weightFromSrc;
				}
				unsigned int     getVertexNum() const {return _vertexNum;}
				unsigned long    getWeightFromSource() const {return _weightFromSrc;}
				void*            getVertexSpecificInfo() const {return _otherVertexSpecificInfo;}
				bool             isEmitted() const {return _emitted;}

				void             setParent(unsigned int p) {_parent = p;}
				void             setWeightFromSource(unsigned long w) {_weightFromSrc = w;}
				void             setEmitted() {_emitted = true;}

			private:
				unsigned int     _vertexNum;
				unsigned int     _parent;
				unsigned long    _weightFromSrc;
				bool             _emitted;
				void            *_otherVertexSpecificInfo;
		};
};

#if 0
int
main()
{
	std::vector<intc>  vi = {intc(4), intc(9), intc(0), intc(3)};
	minHeapPtr<intc>   mhObj;
	for (size_t index = 0; index < vi.size(); ++index) {
		mhObj.insert(&vi[index]);
	}
	mhObj.print();
	std::cout << "----" << std::endl;
	vi[3].set(13);
	mhObj.heapify();
	mhObj.print();
	return 0;
}
#endif

#if 0
int
main()
{
	std::vector<std::vector<int>> directedAjdMatrix = {
		{ 0, 10,  5, -1, -1},
		{-1,  0,  2,  1, -1},
		{-1,  3,  0,  9,  2},
		{-1, -1, -1,  0,  4},
		{ 7, -1, -1,  6,  0},
	};
	std::vector<std::vector<int>> undirectedAjdMatrix = {
		{ 0, 10,  5, -1, -1},
		{10,  0,  2,  1, -1},
		{ 5,  2,  0,  9, -1},
		{-1,  1,  9,  0, -1},
		{-1, -1, -1, -1,  0},
	};
	dijSpf::solve(0, undirectedAjdMatrix);
	return 0;
}
#endif

void
buildAdjacencyListCase1(
	AdjacencyList& outAJ)
{
	outAJ.emplace(std::pair<int,VertexWeightInfoList>(0, {
			VertexWeightInfo(1, 10),
			VertexWeightInfo(2, 5)
		}));
	outAJ.emplace(std::pair<int,VertexWeightInfoList>(1, {
			VertexWeightInfo(2, 2),
			VertexWeightInfo(3, 1)
		}));
	outAJ.emplace(std::pair<int,VertexWeightInfoList>(2, {
			VertexWeightInfo(1, 3),
			VertexWeightInfo(3, 9),
			VertexWeightInfo(4, 2)
		}));
	outAJ.emplace(std::pair<int,VertexWeightInfoList>(3, {
			VertexWeightInfo(4, 4)
		}));
	outAJ.emplace(std::pair<int,VertexWeightInfoList>(4, {
			VertexWeightInfo(0, 7),
			VertexWeightInfo(3, 6)
		}));
}

void
buildAdjacencyListCase1(
	adjacencyListBuilder& ajb)
{
	ajb.addConnectivityInfo(0, 1, 10);
	ajb.addConnectivityInfo(0, 2, 5);
	ajb.addConnectivityInfo(1, 2, 2);
	ajb.addConnectivityInfo(1, 3, 1);
	ajb.addConnectivityInfo(2, 1, 3);
	ajb.addConnectivityInfo(2, 3, 9);
	ajb.addConnectivityInfo(2, 4, 2);
	ajb.addConnectivityInfo(3, 4, 4);
	ajb.addConnectivityInfo(4, 0, 7);
	ajb.addConnectivityInfo(4, 3, 6);
}

#if 0
int
main()
{
	adjacencyListBuilder  ajb;
	buildAdjacencyListCase1(ajb);
	dijSpf::solve(0, ajb.getAJList());
	return 0;
}
#endif

#if 1
//!
//! main to solve : https://www.hackerrank.com/challenges/dijkstrashortreach
//!
int
main()
{
	std::ios::sync_with_stdio(false);
	int T;
	std::cin >> T;
	while (T--) {
		int N, M;
		std::cin >> N >> M;
		adjacencyListBuilder ajb;
		while (M--) {
			int x, y, r;
			std::cin >> x >> y >> r;
			ajb.addConnectivityInfoLeastWeight(x-1,y-1,r);
			ajb.addConnectivityInfoLeastWeight(y-1,x-1,r);
		}
		int S;
		std::cin >> S;
		// ajb.justPrint();
		dijSpf::solve(N, S-1, ajb.getAJList());
	}
	return 0;
}
#endif
