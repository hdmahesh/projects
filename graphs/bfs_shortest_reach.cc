#if 0
https://www.hackerrank.com/challenges/bfsshortreach

Consider an undirected graph consisting of  nodes where each node is labeled from  to  and the edge between any two nodes is always of length . We define node  to be the starting position for a BFS.

It is required to calculate the shortest distance from start position (Node ) to all of the other nodes in the graph.

Note 1: If a node is unreachable, the distance is assumed as . 
Note 2: The length of each edge in the graph is  units.

Input Format

The first line contains an integer, , denoting the number of queries. The subsequent lines describe each query in the following format:

The first line contains two space-separated integers describing the respective values of  (the number of nodes) and  (the number of edges) in the graph.
Each line  of the  subsequent lines contains two space-separated integers,  and , describing an edge connecting node  to node .
The last line contains a single integer, , denoting the index of the starting node.
Constraints

Output Format

For each of the  queries, print a single line of  space-separated integers denoting the shortest distances of the  from starting position . This will be done for all nodes same as in the order of input  to .

If a node is unreachable, print .
#endif

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>

void
shortestPathBFS(
	std::vector<std::vector<int> >& adjacencyMatrix,
	int startIndex)
{
	enum color {
		WHITE = 0, GREY, BLACK
	};
	struct vertexInfo {
		vertexInfo(int index) : v(index), c(WHITE), cost(-1) {}
		int   v;
		color c;
		int   cost;
	};
	std::vector<vertexInfo>  vlist;
	for (size_t index = 0; index < adjacencyMatrix.size(); ++index)
		vlist.emplace_back(index);

	std::queue<std::reference_wrapper<vertexInfo> >  visited;
	int cost = 0;
	visited.push(vlist[startIndex]);
	vlist[startIndex].cost = 0;
	vertexInfo nn(-1);
	visited.push(nn);
	while (visited.front().get().v != -1) {
		vertexInfo& vi = visited.front();
		vi.cost = cost;
		// std::cout << "Visiting : " << vi.v << ", cost : " << vi.cost << std::endl;
		visited.pop();
		bool flag = (visited.front().get().v == -1);
		if (flag) {
			cost += 6;
			visited.pop();
		}
		size_t colIndex = 0;
		std::for_each(adjacencyMatrix[vi.v].begin(),
		              adjacencyMatrix[vi.v].end(),
		              [&vi, &adjacencyMatrix, &vlist, &visited, &colIndex](int& col)
		              {
		                if (adjacencyMatrix[vi.v][colIndex] == 1 && vlist[colIndex].c == WHITE) {
		                  visited.push(vlist[colIndex]);
		                  vlist[colIndex].c = GREY;
		                }
		                colIndex++;
		              });
		vi.c = BLACK;
		if (flag) visited.push(nn);
	}

	for (size_t index = 0; index < vlist.size(); ++index)
		if (index != startIndex) std::cout << vlist[index].cost << " ";
	std::cout << std::endl;

	// std::for_each(vlist.begin(), vlist.end(), [](const vertexInfo& vi) {std::cout << vi.v << " : " << vi.cost << std::endl;});
}


int
main2(int argc, char* argv[])
{
	std::vector<std::vector<int> > am(4);
	std::for_each(am.begin(), am.end(), [](std::vector<int> &v) {v.resize(4);});

	am[0][0] = 0;
	am[0][1] = 1;
	am[0][2] = 1;
	am[0][3] = 0;
	am[1][0] = 1;
	am[1][1] = 0;
	am[1][2] = 0;
	am[1][3] = 0;
	am[2][0] = 1;
	am[2][1] = 0;
	am[2][2] = 0;
	am[2][3] = 1;
	am[3][0] = 0;
	am[3][1] = 0;
	am[3][2] = 1;
	am[3][3] = 0;
	shortestPathBFS(am, atoi(argv[1]));
	return 0;
}

int
main()
{
	int Q;
	std::cin >> Q;
	while (Q--) {
		int m, n;
		std::cin >> n >> m;

		std::vector<std::vector<int> > am(n);
		std::for_each(am.begin(), am.end(), [&n](std::vector<int> &v) {v.resize(n);});

		while (m--) {
			int u, v;
			std::cin >> u >> v;
			am[u-1][v-1] = 1;
			am[v-1][u-1] = 1;
		}

		int s;
		std::cin >> s;
		shortestPathBFS(am, s-1);
	}
	return Q;
}
