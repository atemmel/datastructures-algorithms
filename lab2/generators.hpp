#pragma once
#include <limits>
#include <random>

struct Random
{
public:
	Random(unsigned seed)
		: m_gen(seed),
		m_dist(0, std::numeric_limits<int>::max() )
	{}

	int operator()()
	{
		return m_dist(m_gen);
	}

private:
	std::mt19937 m_gen;
	std::uniform_int_distribution<int> m_dist;
};

struct MonoGrowing
{
public:
	MonoGrowing(unsigned seed)
		: m_gen(seed),
		m_dist(0, 10),
		m_prev(m_dist(m_gen) )
	{}

	int operator()()
	{
		int next = m_prev + m_dist(m_gen);
		m_prev = next;
		return m_prev;
	}

private:
	std::mt19937 m_gen;
	std::uniform_int_distribution<int> m_dist;
	int m_prev;
};

struct MonoFalling
{
public:
	MonoFalling(unsigned seed)
		: m_gen(seed),
		m_dist(0, 10),
		m_prev(m_dist(m_gen) )
	{}

	int operator()()
	{
		int next = m_prev - m_dist(m_gen);
		m_prev = next;
		return next;
	}

private:
	std::mt19937 m_gen;
	std::uniform_int_distribution<int> m_dist;
	int m_prev;
};

struct Constant
{
public:
	Constant(int value)
		: m_value(value)
	{}

	int operator()() const
	{
		return m_value;
	}

private:
	int m_value;
};
