#pragma once
#include <memory>

template<typename T>
class BinarySearchTree
{
public:
	template<typename ForwardIterator>
	BinarySearchTree(ForwardIterator first, ForwardIterator last)
	{
		add(first, last, root);
	}

	size_t depth() const
	{
		size_t depth = 0;
		auto ptr = root.get();

		while(ptr) 
		{
			++depth, ptr = ptr->left.get();
		}

		return depth;
	}

	T* find(const T& value)
	{
		auto ptr = root.get();

		while(ptr)
		{
			if(value < ptr->value) ptr = ptr->left.get();
			else if(value > ptr->value) ptr = ptr->right.get();
			else return &ptr->value;
		}

		return nullptr;
	}

private:
	struct Node
	{
		Node(const T& _value = T() )
			: value(_value)
		{}

		T value;
		std::unique_ptr<Node> left = nullptr;
		std::unique_ptr<Node> right = nullptr;
	};

	using NodePtr = std::unique_ptr<Node>;

	template<typename ForwardIterator>
	void add(ForwardIterator first, ForwardIterator last, NodePtr& node)
	{
		if(first >= last) return;

		auto middle = first + std::distance(first, last) / 2;
		node = std::make_unique<Node>(*middle);

		add(first, middle, node->left);
		add(std::next(middle), last, node->right);
	}

	NodePtr root;
};
