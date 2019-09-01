#ifndef XXYYXX_SKIPLIST_HPP
#define XXYYXX_SKIPLIST_HPP

#include "Node.hpp"
#include <assert.h>

namespace cs540
{
	template <typename T1, typename T2>
	class SkipList 
	{
		private:
			int max_level;
			float p;
			int current_level;			// Highest level
			
		public:
			Node <T1, T2> * header;

			SkipList ();
			SkipList (const SkipList & sl);					// Copy Constructor
			SkipList & operator = (const SkipList & sl); 	// Assignment Operator

			~ SkipList ();									

			Node <T1, T2> * create_node (std::pair <T1, T2> pair, int level);
			Node <T1, T2> * insert (std::pair <T1, T2> pair);
			Node <T1, T2> * search (T1 key) const;

			int random_level ();
			bool remove (T1 key);
			void clear ();

			bool operator == (const SkipList & sl);  /*** Might be a bug here ***/
			char operator < (const SkipList & sl);
	};

	template <typename T1, typename T2>
	SkipList <T1, T2> ::SkipList () : max_level(10), p(0.5)
	{
		current_level = 0;
	 	header = new Node <T1, T2> (max_level);
	}

	template <typename T1, typename T2>
	SkipList <T1, T2> ::SkipList (const SkipList <T1, T2> & sl) : max_level(10), p(0.5)
	{
		this -> current_level = sl.current_level;
		this -> header = sl.header;
	}

	template <typename T1, typename T2>
	SkipList <T1, T2> ::~ SkipList ()
	{ }

	template <typename T1, typename T2>
	SkipList <T1, T2> & 
	SkipList <T1, T2> ::operator = (const SkipList <T1, T2> & sl)
	{
		this -> max_level = 10;
		this -> p = 0.5;
		this -> current_level = sl.current_level;
		this -> header = sl.header;
		return (* this);
	}

	template <typename T1, typename T2>
	int 
	SkipList <T1, T2> ::random_level ()
	{
		float r = (float) rand() / RAND_MAX;
		int level = 0;

		while (r < p && level < max_level)
		{
			level++;
			r = (float) rand() / RAND_MAX;
		}

		return level;
	}

	template <typename T1, typename T2>
	Node <T1, T2> * 
	SkipList <T1, T2> ::create_node (std::pair <T1, T2> pair, int level)
	{
		Node <T1, T2> * n = new Node <T1, T2> (pair, level);
		return n;
	}

	template <typename T1, typename T2>
	Node <T1, T2> *
	SkipList <T1, T2> ::insert (std::pair <T1, T2> pair)
	{
		Node <T1, T2> * current = header;
		Node <T1, T2> * n = nullptr;

		Node <T1, T2> ** update; 
		update = new Node <T1, T2> * [max_level + 1];
		memset(update, 0, sizeof(Node <T1, T2> *) * (max_level + 1));

		for (int i = current_level; i >= 0; i--)
		{
			while (current -> forward[i] != NULL && current -> forward[i] -> pair -> first < pair.first)
			{
				current = current -> forward[i];
			}

			update[i] = current;
		}

		current = current -> forward[0];

		if (current == NULL || !(current -> pair -> first == pair.first))
		{
			int rand_level = random_level();

			if (rand_level > current_level)
			{
				for (int i = current_level + 1; i < rand_level + 1; i++) update[i] = header;
				current_level = rand_level;
			}

			n = create_node(pair, rand_level);

			for (int i = 0; i <= rand_level; i++)
			{
				n -> forward[i] = update[i] -> forward[i];
				update[i] -> forward[i] = n;
			}
		}
		else n = current;

		delete [] update;
		return n;
	}

	template <typename T1, typename T2>
	bool
	SkipList <T1, T2> ::remove (T1 key)
	{
		Node <T1, T2> * current = header;
		bool found = false;

		Node <T1, T2> ** update; 
		update = new Node <T1, T2> * [max_level + 1];
		memset(update, 0, sizeof(Node <T1, T2> *) * (max_level + 1));

		for (int i = current_level; i >= 0; i--)
		{
			while (current -> forward[i] != NULL && current -> forward[i] -> pair -> first < key)
			{
				current = current -> forward[i];
			}

			update[i] = current;
		}

		current = current -> forward[0]; 

		if (current != NULL && current -> pair -> first == key)
		{
			Node <T1, T2> * to_be_deleted = current;

			found = true;
			for (int i = 0; i <= current_level; i++) 
        	{
        		if (update[i] -> forward[i] != current) break;
        		update[i] -> forward[i] = current -> forward[i];
        	}

        	while (current_level > 0 && header -> forward[current_level] == 0) current_level--;

        	delete [] to_be_deleted -> forward;  
			delete to_be_deleted -> pair;			
			delete to_be_deleted;					
		}

		delete [] update;
		return found;
	}

	template <typename T1, typename T2>
	Node <T1, T2> * 
	SkipList <T1, T2> ::search (T1 key) const
	{
		Node <T1, T2> * current = header;

		for (int i = current_level; i >= 0; i--) 
    	{
    		while (current -> forward[i] != NULL && current -> forward[i] -> pair -> first < key) 
    		{
            	current = current->forward[i];
            }
    	}

    	current = current -> forward[0]; 

        if (current != NULL && current -> pair -> first == key) return current;
        else return nullptr;
	}

	template <typename T1, typename T2>
	void 
	SkipList <T1, T2> ::clear ()
	{
		Node <T1, T2> * current = header -> forward[0];
		Node <T1, T2> * forward = header -> forward[0];

		if (forward == nullptr) return; // Empty map being tried to be cleaned

		while (forward -> forward[0] != NULL)
		{
			forward = forward -> forward[0];

			//for (int i = 0; i < current -> forward_size; i++) delete current -> forward[i];
			delete [] current -> forward;  // Free forward array of the Node
			
			delete current -> pair;			 // Free pair of the Node
			delete current;					 // Free the Node itself
 
			current = forward;
		}

		//for (int i = 0; i < forward -> forward_size; i++) delete forward -> forward[i];
		delete [] (forward -> forward);  

		delete forward -> pair;			
		delete forward; 
		header -> forward[0] = nullptr;

		current_level = 0;
	}

	template <typename T1, typename T2>
	bool 
	SkipList <T1, T2> ::operator == (const SkipList & sl)
	{
		Node <T1, T2> * left = this -> header -> forward[0];
		Node <T1, T2> * right = sl.header -> forward[0];

		while (left != NULL && right != NULL)
		{
			if (! (left -> pair -> first == right -> pair -> first)) return false;

			/**** Not Sure About This ****/

			if (! (left -> pair -> second == right -> pair -> second)) return false;  

			left = left -> forward[0];
			right = right -> forward[0];
		}

		if (left != NULL || right != NULL) assert(false); // Compared maps must be in same size
		return true;
	}

	template <typename T1, typename T2>
	char 
	SkipList <T1, T2> ::operator < (const SkipList & sl)
	{
		Node <T1, T2> * left = this -> header -> forward[0];
		Node <T1, T2> * right = sl.header -> forward[0];

		while (left != NULL && right != NULL)
		{
			/**** Not Sure About This ****/

			if (left -> pair -> first < right -> pair -> first &&
				left -> pair -> second < right -> pair -> second) return 0; // left is less than right
		}

		while (left != NULL && right != NULL)
		{
			if (! (left -> pair -> first == right -> pair -> first) ||
				! (left -> pair -> second == right -> pair -> second)) return 1; // left is not less than right
		}

		return 2; // left might be less than right if it is shorter
	}
}

#endif
