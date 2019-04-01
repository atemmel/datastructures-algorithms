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
};

using Vertices = std::vector<Vertex>;

struct Edge
{
	Vertices::iterator a, b;
	float weight;
	std::string name;
};

using Edges = std::vector<Edge>;

struct Index
{
	Edges::iterator edge;
	float weight = 0.f;
};

using Matrix = std::vector<std::vector<Index> >;

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

		m_matrix.resize(m_vertices.size(), std::vector<Index>(m_vertices.size() ) );

		for(auto it = m_edges.begin(); it != m_edges.end(); it++) 
		{
			auto &m = m_matrix[it->a->id][it->b->id];

			m.weight = it->weight;
			m.edge = it;
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

			for(auto index : array)
			{
				if(index.weight == 0.f)
				{
					std::cout << "N     ";
				}
				else
				{
					str = std::to_string(index.weight);
					while(str.size() > 4) str.pop_back();
					std::cout << str << "  ";
				}
			}

			std::cout << '\n';
			++i;
		}
	}

	bool depth_first()
	{

		std::stack<Vertices::iterator> stack;

		auto getStack = [&]() { return stack.top(); };
		
		return search(stack, getStack);
	}

	bool breadth_first()
	{
		std::queue<Vertices::iterator> queue;

		auto getQueue = [&]() { return queue.back(); };

		return search(queue, getQueue);
	}

	void repair()
	{
		for(auto &vertex: m_vertices)
		{
			if(!vertex.visited)
			{
				for(int i = 0, j = 0; j < m_matrix.size(); i++, j++)
				{
					auto &a = m_matrix[vertex.id][i], &b = m_matrix[j][vertex.id];

					if(a.weight != b.weight)
					{
						a.weight = b.weight = std::max(a.weight, b.weight);
					}
				}
			}
		}
	}
	
private:

	template<typename Container, typename GetElement>
	bool search(Container &frontier, GetElement get)
	{
		auto getNeighbours = [&](Vertices::iterator vertex)
		{
			std::vector<Vertices::iterator> neighbours;

			for(auto &index : m_matrix[vertex->id])
			{

				if(index.weight > 0.f)
				{
					Vertices::iterator neighbour = vertex->id == index.edge->a->id ?
						index.edge->b : index.edge->a;
					
					if(neighbour->visited) continue;

					neighbours.push_back(neighbour);
				}
			}

			return neighbours;
		};

		frontier.push(m_vertices.begin() );

		for(auto &v : m_vertices) v.visited = false;

		while(!frontier.empty() )
		{
			auto vertex = get();
			vertex->visited = true;

			auto neighbours = getNeighbours(vertex);
			//std::cout << "Standing at: " << vertex->id << '\n';

			if(!neighbours.empty() )
			{
				Edges::iterator road = m_matrix[neighbours.front()->id][vertex->id].edge;
				//std::cout << "Will go to: " << neighbours.front()->id << " through " << road->name << '\n';
				frontier.push(neighbours.front() );
			}
			else frontier.pop();
		}

		for(auto &vertex : m_vertices) if(!vertex.visited) return false;

		return true;
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

	if(!graph.breadth_first() && !graph.depth_first() ) graph.repair();

	graph.display();

	return 0;
}
