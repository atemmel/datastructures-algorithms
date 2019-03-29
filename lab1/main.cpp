#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

struct Vertex
{
	int id;
	std::string name;
};

struct Edge
{
	int a, b;
	float weight;
	std::string name;
};

using Vertices = std::vector<Vertex>;
using Edges = std::vector<Edge>;

class Graph
{
public:
	bool open(const std::string & path)
	{
		std::string line;
		std::ifstream file(path.c_str() );

		if(!file.is_open() ) return false;

		while(std::getline(file, line, ' ') )
		{
			if(line.front() == '#') 
			{
				file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				continue;
			}


			if(line.front() == 'M')  // Vertex
			{
				Vertex vertex;

				std::getline(file, line, ' ');

				try
				{
					vertex.id = std::stoi(line);
				}
				catch(std::invalid_argument err)
				{
					std::cerr << "Exception caught: " << err.what() << " processing: " 
						<< line << '\n';

					return false;
				}

				std::getline(file, line, '\n');

				vertex.name = line;

				m_vertices.push_back(vertex);
			}
			else
			{
				Edge edge;

				try
				{
					edge.a = std::stoi(line);
					std::getline(file, line, ' ');
					edge.b = std::stoi(line);
					std::getline(file, line, ' ');
					edge.weight = std::stof(line);
					std::getline(file, line, '\n');
					edge.name = line;
					m_edges.push_back(edge);
				}
				catch(std::invalid_argument err)
				{
					std::cerr << "Exception caught: " << err.what() << " processing: " 
						<< line << '\n';

					return false;
				}
			}
		}

		/*
		for(auto & v : m_vertices)
		{
			std::cout << v.id << ' ' << v.name << '\n';
		}

		for(auto & e : m_edges)
		{
			std::cout << e.name << ' '  << e.weight << ' ' << e.a << ' ' << e.b << '\n';
		}
		*/

		return true;
	}

	
private:
	Vertices m_vertices;
	Edges m_edges;
};

int main()
{

	Graph graph;

	graph.open("export.txt");

	return 0;
}
