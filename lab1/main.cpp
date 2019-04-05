#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <stack>
#include <queue>

struct Vertex
{
	int id;
	std::string name;
	float effectiveWeight = std::numeric_limits<float>::max();
	bool visited = 0;

	bool operator<(const Vertex &rhs)
	{
		return effectiveWeight < rhs.effectiveWeight;
	}
};

using Vertices = std::vector<Vertex>;

struct Edge
{
	Vertices::iterator a, b;
	float weight;
	std::string name;
};

using Edges = std::vector<Edge>;
using Matrix = std::vector<std::vector<Edges::iterator> >;

class Graph
{
public:
	void open(const std::string & path)
	{
		std::string line;
		std::ifstream file(path.c_str() );

		if(!file.is_open() ) return;

		while(std::getline(file, line, ' ') )
		{
			if(line.front() == '#') 
			{
				file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else if(line.front() == 'M')  // Vertex
			{
				Vertex vertex;

				std::getline(file, line, ' ');
				vertex.id = std::stoi(line);
				std::getline(file, line, '\n');

				vertex.name = line;

				m_vertices.push_back(vertex);
			}
			else
			{
				int a = std::stoi(line);
				std::getline(file, line, ' ');
				int b = std::stoi(line);
				std::getline(file, line, ' ');
				float weight = std::stof(line);
				std::getline(file, line, '\n');

				auto start = std::find_if(m_vertices.begin(), m_vertices.end(), [&](const Vertex &v)
				{
					return v.id == a;
				});

				auto end = std::find_if(m_vertices.begin(), m_vertices.end(), [&](const Vertex &v)
				{
					return v.id == b;
				});

				m_edges.push_back(Edge{start, end, weight, line} );
			}
		}

		m_matrix.resize(m_vertices.size(), std::vector<Edges::iterator>(m_vertices.size(), m_edges.end() ) );

		for(auto it = m_edges.begin(); it != m_edges.end(); it++) 
		{
			auto &m = m_matrix[it->a->id][it->b->id];
			m = it;
		}
	}

	void display()
	{
		std::string str;
		int i = 0;

		std::cout << "\\     ";

		for(int j = 0; j < m_matrix.size(); j++)
		{
			str = std::to_string(j);
			while(str.size() < 6) str.push_back(' ');
			std::cout << str;
		}

		std::cout << '\n';

		for(auto &array : m_matrix)
		{
			str = std::to_string(i);
			while(str.size() < 6) str.push_back(' ');

			std::cout << str;

			for(auto edge : array)
			{
				if(edge == m_edges.end() )
				{
					std::cout << "      ";
				}
				else
				{
					str = std::to_string(edge->weight);
					while(str.size() > 4) str.pop_back();
					std::cout << str << "  ";
				}
			}

			std::cout << '\n';
			++i;
		}
	}

	bool depthFirst()
	{

		std::stack<Vertices::iterator> stack;

		auto getStack = [&]() { return stack.top(); };
		
		return search(stack, getStack);
	}

	bool breadthFirst()
	{
		std::queue<Vertices::iterator> queue;

		auto getQueue = [&]() { return queue.front(); };

		return search(queue, getQueue);
	}

	void repair()
	{
		for(auto &vertex: m_vertices)
		{
			if(!vertex.visited)
			{
				for(int i = 0; i < m_matrix.size(); i++)
				{
					auto &a = m_matrix[vertex.id][i], &b = m_matrix[i][vertex.id];
					a = b = std::min(a, b);
				}
			}
		}
	}

	std::vector<Edges::iterator> djikstra(int start, int end)
	{
		auto startVertex = std::find_if(m_vertices.begin(), m_vertices.end(), [&](const Vertex &vertex)
		{
			return vertex.id == start;
		});
		
		auto endVertex = std::find_if(m_vertices.begin(), m_vertices.end(), [&](const Vertex &vertex)
		{
			return vertex.id == end;
		});

		reset();

		std::vector<Edges::iterator> history;
		std::vector<Vertices::iterator> queue;
		
		auto pop = [&]()
		{
			queue.erase(std::min_element(queue.begin(), queue.end() ) );
		};

		auto top = [&]()
		{
			return *std::max_element(queue.begin(), queue.end() );
		};

		queue.push_back(startVertex);
		startVertex->effectiveWeight = 0.f;
		startVertex->visited = true;

		std::cout << "Moving to: " << endVertex->name << '\n';

		while(!queue.empty() )
		{
			auto neighbours = getNeighbours(top() );

			if(neighbours.empty() )
			{
				pop();
				continue;
			}

			for(auto neighbour : neighbours)
			{
				float weight = m_matrix[startVertex->id][neighbour->id]->weight + top()->effectiveWeight;
				neighbour->visited = true;
				queue.push_back(neighbour);
				if(weight < neighbour->effectiveWeight) neighbour->effectiveWeight = weight;
			}

			auto cheapest = *std::min_element(neighbours.begin(), neighbours.end() );

			std::cout << cheapest->name << " was chosen\n";

			history.push_back(m_matrix[top()->id][cheapest->id]);
			pop();
		}

		return history;
	}
	
private:

	std::vector<Vertices::iterator> getNeighbours(Vertices::iterator vertex)
	{
		std::vector<Vertices::iterator> neighbours;

		for(auto edge : m_matrix[vertex->id])
		{
			if(edge != m_edges.end() )
			{
				Vertices::iterator neighbour = vertex->id == edge->a->id ?
					edge->b : edge->a;
				
				if(neighbour->visited) continue;

				neighbours.push_back(neighbour);
			}
		}

		return neighbours;
	}

	template<typename Container, typename GetElement>
	bool search(Container &frontier, GetElement get)
	{
		reset();

		frontier.push(m_vertices.begin() );
		m_vertices.begin()->visited = true;
		while(!frontier.empty() )
		{
			auto vertex = get();
			auto neighbours = getNeighbours(vertex);

			//std::cout << "Standing at: " << vertex->id << '\n';

			if(!neighbours.empty() )
			{
				neighbours.front()->visited = true;
				frontier.push(neighbours.front() );
			}
			else frontier.pop();
		}

		for(auto &vertex : m_vertices) if(!vertex.visited) return false;

		return true;
	}

	void reset()
	{

		for(auto &v : m_vertices) 
		{
			v.visited = false;
			v.effectiveWeight = std::numeric_limits<float>::max();
		}
	}

	Vertices m_vertices;
	Edges m_edges;
	Matrix m_matrix;
};

int main()
{

	Graph graph;

	try
	{
		graph.open("export.txt");
	}
	catch(std::invalid_argument err)
	{
		std::cout << "File could not be parsed: " << err.what() << '\n';
	}

	if(!graph.breadthFirst() && !graph.depthFirst() ) graph.repair();

	graph.display();
	std::cout << std::boolalpha << graph.breadthFirst() << ' ' << graph.depthFirst() << '\n';

	graph.djikstra(0, 10);

	return 0;
}
