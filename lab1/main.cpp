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
using Matrix = std::vector<std::vector<float> >;

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
				//continue;
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
				Edge edge;

				edge.a = std::stoi(line);
				std::getline(file, line, ' ');
				edge.b = std::stoi(line);
				std::getline(file, line, ' ');
				edge.weight = std::stof(line);
				std::getline(file, line, '\n');
				edge.name = line;
				m_edges.push_back(edge);
			}
		}

		m_matrix.resize(m_vertices.size(), std::vector<float>(m_vertices.size(), 0.) );

		for(auto &edge : m_edges) m_matrix[edge.a][edge.b] = edge.weight;
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

			for(auto element : array)
			{
				if(element == 0)
				{
					std::cout << "N     ";
				}
				else
				{
					str = std::to_string(element);
					while(str.size() > 4) str.pop_back();
					std::cout << str << "  ";
				}
			}

			std::cout << '\n';
			++i;
		}
	}
	
private:
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

	graph.display();

	return 0;
}
