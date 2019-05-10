#pragma once
#include <forward_list>
#include <vector>

template<typename T>
class HashTable
{
public:
	template<typename ForwardIterator>
	HashTable(ForwardIterator first, ForwardIterator last)
		: m_table(std::distance(first, last) << 1)
	{
		for(auto it = first; it != last; it++)
		{
			m_table[*it % m_table.size()].push_front(*it);
		}
	}

	T* find(const T& value)
	{
		auto it = m_table[value % m_table.size() ].begin();
		auto end = m_table[value % m_table.size() ].end();

		while(it != end && *it != value) ++it;

		if(it == end) return nullptr;

		return &*it;
	}

private:
	std::vector<std::forward_list<T> > m_table;
};
