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
	bool visited = 0;
	std::vector<Vertex>::iterator parent;
	float effectiveWeight = std::numeric_limits<float>::infinity();

	bool operator<(const Vertex &rhs) const
	{
		return effectiveWeight < rhs.effectiveWeight;
	}
};

using Vertices = std::vector<Vertex>;

struct Edge
{
	Vertices::iterator a, b;
	float weight = std::numeric_limits<float>::infinity();
	std::string name;

	bool operator<(const Edge &rhs) const
	{
		return weight < rhs.weight;
	}

	bool operator>(const Edge &rhs) const
	{
		return weight > rhs.weight;
	}

	operator bool () const
	{
		return weight != std::numeric_limits<float>::infinity();
	}
};

using Edges = std::vector<Edge>;
using Matrix = std::vector<Edges>;

class Graph
{
public:
	void open(const std::string & path)
	{
		std::string line;
		std::ifstream file(path.c_str() );

		if(!file.is_open() ) return;

		Edges edges;

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

				edges.push_back(Edge{start, end, weight, line} );
			}
		}

		m_matrix.resize(m_vertices.size(), Edges(m_vertices.size() ) );

		for(auto &e : edges) m_matrix[e.a->id][e.b->id] = e;
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

			for(auto &edge : array)
			{
				//if(edge == m_edges.end() )
				if(!edge)
				{
					std::cout << "      ";
				}
				else
				{
					str = std::to_string(edge.weight);
					while(str.size() > 4) str.pop_back();
					std::cout << str << "  ";
				}
			}

			std::cout << '\n';
			++i;
		}
	}

	void csv()
	{
		std::ofstream file("export.csv");
		for(auto &row : m_matrix)
		{
			for(auto &edge : row) file << (edge.weight == std::numeric_limits<float>::infinity() ? 0.f : edge.weight ) << ',';
			file.seekp(-1, std::ios_base::cur);
			file << '\n';
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
					
					if(a && !b)
					{
						b = a;
						std::swap(b.a, b.b);
					}
					else if(!a && b)
					{
						a = b;
						std::swap(a.a, a.b);
					}
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

		auto completed = [&]()
		{
			for(auto &v : m_vertices) if(!v.visited) return false;
			return true;
		};

		reset();

		std::priority_queue<Edge, Edges, std::greater<Edge> > edges;
		startVertex->visited = true;
		startVertex->effectiveWeight = 0.f;

		auto findEdges = [&](const Vertex &vertex)
		{
			for(auto &edge : m_matrix[vertex.id])
				if(edge) 
					edges.push(edge);
		};

		findEdges(*startVertex);

		while(!edges.empty() )
		{
			auto min = edges.top();
			edges.pop();

			float newWeight = min.weight + min.a->effectiveWeight;

			if(newWeight < min.b->effectiveWeight)
			{
				min.b->effectiveWeight = newWeight;
				min.b->parent = min.a;
			}

			if(min.b->visited) 
			{
				std::cout << min.a->id << "\t->\t" << min.b->id << "\tcosts\t" << min.weight << "\tSkipped!\n" ;
				continue;
			}
			else findEdges(*min.b);

			min.b->visited = true;

			std::cout << min.a->id << "\t->\t" << min.b->id << "\tcosts\t" << min.weight << "\tcost for: " << min.b->id << '\t' << min.b->effectiveWeight << '\n';
		}

		printPath(endVertex);
		printPath(m_vertices.begin() + 28);

		return std::vector<Edges::iterator>();
	}
	
private:
	float getDistance(Vertices::iterator it)
	{
		if(it->parent != m_vertices.end() ) return m_matrix[it->id][it->parent->id].weight + getDistance(it->parent);
		return 0.f;
	}

	void printPath(Vertices::iterator it)
	{
		std::cout << it->id << " <- ";
		if(it->parent != m_vertices.end() ) printPath(it->parent);
		else puts("START");
	}

	std::vector<Vertices::iterator> getNeighbours(Vertices::iterator vertex)
	{
		std::vector<Vertices::iterator> neighbours;

		for(auto &edge : m_matrix[vertex->id])
		{
			if(edge)
			{
				Vertices::iterator neighbour = vertex->id == edge.a->id ?
					edge.b : edge.a;
				
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
			v.effectiveWeight = std::numeric_limits<float>::infinity();
			v.parent = m_vertices.end();
		}
	}

	Vertices m_vertices;
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

	//graph.display();
	//std::cout << std::boolalpha << graph.breadthFirst() << ' ' << graph.depthFirst() << '\n';

	auto s = graph.djikstra(24, 37);

	//graph.csv();

	return 0;
}
