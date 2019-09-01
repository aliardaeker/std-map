#ifndef XXYYXX_NODE_HPP
#define XXYYXX_NODE_HPP

#include <iostream>
#include <utility>
#include <cstring>

namespace cs540
{
	template <typename T1, typename T2>
	class Node
	{
		private:
		public:
			std::pair <T1, T2> * pair;
			Node ** forward; // Array to hold node pointers to nodes in different levels
			bool is_header;
			int forward_size;

			Node () {} ;
			Node (const Node & n);							// Copy Constructor		    Node n = n2
			Node & operator = (const Node & n); 			// Assignment operator      n1 = n2

			~ Node ();										// ***** FIX THIS ******

			Node (int level);
			Node (std::pair <T1, T2> pair, int level);
	};

	template <typename T1, typename T2>
	Node <T1, T2>::Node (const Node <T1, T2> & n)
	{
		this -> pair = n.pair;
		this -> forward = n.forward;
		this -> is_header = n.is_header;
		this -> forward_size = n.forward_size;
	}

	template <typename T1, typename T2>
	Node <T1, T2>::~ Node ()
	{ }

	template <typename T1, typename T2>
	Node <T1, T2> & 
	Node <T1, T2>::operator = (const Node <T1, T2> & n)
	{
		this -> pair = n.pair;
		this -> forward = n.forward;
		this -> is_header = n.is_header;
		this -> forward_size = n.forward_size;
		return (* this);
	}

	template <typename T1, typename T2>
	Node <T1, T2>::Node (int level) : pair(nullptr), is_header(true)
	{
		forward = new Node * [level + 1];
		memset(forward, 0, sizeof (Node *) * (level + 1));

		forward_size = level + 1;
	}

	template <typename T1, typename T2>
	Node <T1, T2>::Node (std::pair <T1, T2> pair, int level) : is_header(false)
	{
		this -> pair = new std::pair <T1, T2> (pair.first, pair.second);

		forward = new Node * [level + 1]; // Level 0 nodes has just 1 node pointer, to point the next node in the same level
		memset(forward, 0, sizeof(Node *) * (level + 1)); // Set forward array cells to 0

		forward_size = level + 1;
	}
}

#endif
