#include <iostream>
#include <map>

using namespace std;


typedef std::pair<int,int>                  VertexWeightInfo;
typedef std::map<int, long>                  VertexWeightInfoList;
typedef std::map<int,VertexWeightInfoList>   AdjacencyList;
typedef std::map<int,int>   VisitedNodeMap;

class adjacencyListBuilder {
	public:
		adjacencyListBuilder() {}
		~adjacencyListBuilder() {}

		const AdjacencyList&     getAJList() const {return _aj;};
		void                     addConnectivityInfo(int src, int dest, int weight) {
			AdjacencyList::iterator it = _aj.find(src);
			if (it == _aj.end()) {
				VertexWeightInfoList vil;
				vil.insert(std::pair<int,long>(dest, weight));
				_aj.insert(std::pair<int,VertexWeightInfoList>(src, vil));
			} else {
				it->second.insert(VertexWeightInfo(dest,weight));
			}
		}

		void print() const {
			AdjacencyList::const_iterator cit = _aj.begin();
			for (; cit != _aj.end(); ++cit) {
				std::cout << cit->first << std::endl;
				VertexWeightInfoList::const_iterator cit2 = cit->second.begin();
				for (; cit2 != cit->second.end(); ++cit2) {
					std::cout << "    " << cit2->first << std::endl;
				}
			}
		}

	private:
		AdjacencyList        _aj;
};

class graph {
	public:
		graph() {}
		~graph() {}

		static bool    hasLoop(const AdjacencyList& adj, VisitedNodeMap& vnm) {
			AdjacencyList::const_iterator it = adj.begin();
			for (; it != adj.end(); ++it) {
				if (vnm.count(it->first) == 0) {
					vnm.insert(std::pair<int,int>(it->first, 1));
					if (_hasLoop(it->first, adj, vnm)) {
						return true;
					}
					vnm.find(it->first)->second = 0;
				}
			}
			return false;
		}

	private:
		static bool    _hasLoop(int v, const AdjacencyList& adj, VisitedNodeMap& vnm) {
			bool toReturn = false;
			AdjacencyList::const_iterator cit = adj.find(v);
			if (cit != adj.end()) {
				const VertexWeightInfoList& adjList = cit->second;
				VertexWeightInfoList::const_iterator it = adjList.begin();
				for (; it != adjList.end(); ++it) {
					if (vnm.count(it->first) != 0 && vnm.find(it->first)->second != 0) {
						toReturn = true;
						break;
					} else {
						vnm.insert(std::pair<int,int>(it->first, 1));
						if (toReturn = _hasLoop(it->first, adj, vnm)) {
							break;
						}
						vnm.find(it->first)->second = 0;
					}
				}
			}
			return toReturn;
		}
};


int main()
{
	int T = 0;
	cin >> T;
	while (T--) {
		adjacencyListBuilder ajb;
		int N = 0;
		int M = 0;
		cin >> N >> M;
		while (M--) {
			int u = 0, v = 0;
			cin >> u >> v;
			ajb.addConnectivityInfo(u, v, 1);
		}
		// ajb.print();
		VisitedNodeMap vnm;
		cout << !graph::hasLoop(ajb.getAJList(), vnm) << endl;
	}
	return 0;
}

