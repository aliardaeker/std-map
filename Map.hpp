#ifndef XXYYXX_MAP_HPP
#define XXYYXX_MAP_HPP

#include "SkipList.hpp"
#include <stdexcept>

namespace cs540
{
	template <typename T1, typename T2>
	class Map
	{
		private:
			SkipList <T1, T2> sl;
			size_t size_;
		public:
			class ConstIterator; // Forward decleration

			class Iterator 
			{
				private:
				public:
					Node <T1, T2> * iter;   
					Node <T1, T2> * header;

					Iterator (Node <T1, T2> * n, Node <T1, T2> * h) : iter(n), header(h) { };
					Iterator () = delete; 

					// Iterator (const Iterator &); 					// Copy Constructor -> (These might not be needed)
					// ~ Iterator (); 									// Destructor
					// Iterator & operator = (const Iterator &);  		// Copy Assignment Operator
					void operator = (const Iterator & it);

					Iterator operator ++ (int); 						// i++, post increment
					Iterator & operator ++ ();							// ++i, pre increment
					Iterator operator -- (int);							
					Iterator & operator -- ();							

					bool operator == (const Iterator & it); 			// iter == iter
					bool operator == (const ConstIterator & it); 		// iter == const iter
					bool operator != (const Iterator & it);  			// iter != iter
					bool operator != (const ConstIterator & it); 		// iter != const iter

					std::pair <T1, T2> & operator * () const;
					std::pair <T1, T2> * operator -> () const;
			};			

			class ConstIterator 
			{ 
				private:
				public:
					Node <T1, T2> * iter;
					Node <T1, T2> * header;

					ConstIterator (Node <T1, T2> * n, Node <T1, T2> * h) : iter(n), header(h) { };
					ConstIterator () = delete; 

					// ConstIterator (const Iterator &); 				// Copy Constructor -> (These might not be needed)
					// ~ ConstIterator (); 								// Destructor
					// ConstIterator & operator = (const Iterator &);  	// Copy Assignment Operator
					void operator = (const ConstIterator & it);

					ConstIterator operator ++ (int); 					// i++, post increment
					ConstIterator & operator ++ ();						// ++i, pre increment
					ConstIterator operator -- (int);
					ConstIterator & operator -- ();						

					bool operator == (const ConstIterator & it);  		// const iter == const iter
					bool operator == (const Iterator & it); 	  		// const iter == iter
					bool operator != (const ConstIterator & it);		// const iter != const iter
					bool operator != (const Iterator & it); 			// const iter != iter

					const std::pair <T1, T2> & operator * () const;
					const std::pair <T1, T2> * operator -> () const;
			};

			class ReverseIterator 
			{ 
				private:
				public:
					Node <T1, T2> * iter;
					Node <T1, T2> * header;

					ReverseIterator (Node <T1, T2> * n, Node <T1, T2> * h) : iter(n), header(h) {};
					ReverseIterator () = delete; 

					// ConstIterator (const Iterator &);
					// ~ ConstIterator (); 
					// ConstIterator & operator = (const Iterator &); 
					void operator = (const ReverseIterator & it);

					ReverseIterator operator ++ (int);
					ReverseIterator & operator ++ ();
					ReverseIterator operator -- (int);
					ReverseIterator & operator -- ();

					bool operator == (const ReverseIterator &);
					bool operator != (const ReverseIterator &);

					std::pair <T1, T2> & operator * () const;
					std::pair <T1, T2> * operator -> () const;
			};

			Map () : size_(0) { };
			Map (const Map & m); 											// Copy constructor, These should be O(N)
			Map (std::initializer_list <std::pair <const T1, T2>> l);		// Initializer list constructor
			Map & operator = (const Map & m); 								// Copy assignment operator

			~ Map ();																	/***** FIX THIS ******/
			size_t size () const;
			bool empty() const;

			std::pair <Iterator, bool> insert (std::pair <T1, T2> pair);
			template <typename IT_T> void insert (IT_T range_beg, IT_T range_end);   	/***** FIX THIS ******/
			void erase (Iterator i);
			void erase (const T1 & key);
			void clear ();															

			Iterator find (const T1 & key);
			Iterator begin ();
			Iterator end ();

			ConstIterator find (const T1 & key) const;
			ConstIterator begin() const;
			ConstIterator end() const;

			ReverseIterator rbegin ();
			ReverseIterator rend ();

			T2 & at (const T1 & key);
			const T2 & at (const T1 & key) const;
			T2 & operator [] (const T1 & key);

			bool operator == (const Map & m);
			bool operator != (const Map & m);
			bool operator < (const Map & m);
	};

	/*
	*
	*	MAP FUNCTIONS
	*
	*/

	template <typename T1, typename T2>
	Map <T1, T2> ::Map (const Map & m) 										// Copy constructor
	{ 
		this -> size_ = m.size_;
		this -> sl = m.sl;
	}

	template <typename T1, typename T2>
	Map <T1, T2> ::Map (std::initializer_list <std::pair <const T1, T2>> l)	// Initializer list constructor
	{ 
		for (auto p : l) { this -> sl.insert(p); size_++; }
	}

	template <typename T1, typename T2>		
	Map <T1, T2> ::~ Map ()													// Destructor
	{ 
		sl.clear();
		if (sl.header != nullptr) 
		{
			sl.header = nullptr;
			delete sl.header;
		}	
	}

	template <typename T1, typename T2>
	Map <T1, T2> & 
	Map <T1, T2> ::operator = (const Map & m)								// Copy assignment operator
	{
		this -> size_ = m.size_;
		this -> sl = m.sl;
		return (* this);
	}

	template <typename T1, typename T2>
	size_t 
	Map <T1, T2> ::size () const { return size_; }

	template <typename T1, typename T2>
	bool 
	Map <T1, T2> ::empty() const
	{
		if (size_) return true;
		else return false;
	}

	template <typename T1, typename T2>
	std::pair <typename Map <T1, T2> ::Iterator, bool>
	Map <T1, T2> ::insert (std::pair <T1, T2> pair) 
	{ 
		bool added = false;

		Node <T1, T2> * n = sl.insert(pair);

		if (n != nullptr)
		{ 
			size_++; 
			added = true; 
		}

		Iterator i(n, sl.header);
		std::pair <Iterator, bool> p = std::pair <Iterator, bool> (i, added);

		return p;
	}

	template <typename T1, typename T2>
	void
	Map <T1, T2> ::erase (Iterator i)
	{ 
		if (i.iter == nullptr) return;

		bool found = sl.remove(i.iter -> pair -> first);
		if (!found) throw std::out_of_range ("Erase(Iterator): Iter not Found !");

		size_--;
		i.iter = nullptr; 
	}

	template <typename T1, typename T2>
	void
	Map <T1, T2> ::erase (const T1 & key)
	{
		bool found = sl.remove(key);
		if (found) size_--;
		else throw std::out_of_range ("Erase(key): Key not Found !");
	}

	template <typename T1, typename T2>
	void 
	Map <T1, T2> ::clear ()
	{
		sl.clear();
		size_ = 0;
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::Iterator 
	Map <T1, T2> ::find (const T1 & key)
	{
		//std::cout << "Iter find called.\n"; 
		Node <T1, T2> * n = sl.search(key);
		
		if (n == nullptr) return end();
		else
		{
			Iterator i(n, sl.header);
			return i;
		}
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::Iterator
	Map <T1, T2> ::begin ()
	{
		Node <T1, T2> * n = sl.header -> forward[0];
		Iterator i(n, sl.header); 

		return i;
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::Iterator
	Map <T1, T2> ::end ()
	{
		Node <T1, T2> * n = sl.header -> forward[0];
		while (n != NULL) n = n -> forward[0];

		Iterator i(n, sl.header); 
		return i;
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::ConstIterator 
	Map <T1, T2> ::find (const T1 & key) const
	{
		//std::cout << "Const Iter find called.\n";
		Node <T1, T2> * n = sl.search(key);
		
		if (n == nullptr) return end();
		else
		{
			ConstIterator i(n, sl.header);
			return i;
		}
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::ConstIterator 
	Map <T1, T2> ::begin () const
	{
		Node <T1, T2> * n = sl.header -> forward[0];
		ConstIterator i(n, sl.header); 

		return i;
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::ConstIterator 
	Map <T1, T2> ::end () const
	{
		Node <T1, T2> * n = sl.header -> forward[0];
		while (n != NULL) n = n -> forward[0];

		ConstIterator i(n, sl.header); 
		return i;
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::ReverseIterator 
	Map <T1, T2> ::rbegin ()
	{
		if (size_ == 0) assert(false); // Map has no elements
		if (size_ == 1)
		{
			Node <T1, T2> * n = sl.header -> forward[0];
			ReverseIterator i(n, sl.header); 
			return i;
		}

		Node <T1, T2> * n = sl.header -> forward[0];
		Node <T1, T2> * n_next = n -> forward[0];

		while (n_next != NULL) 
		{
			n = n -> forward[0];
			n_next = n_next -> forward[0];
		}

		ReverseIterator i(n, sl.header); 
		return i;
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::ReverseIterator 
	Map <T1, T2> ::rend ()
	{
		Node <T1, T2> * n = sl.header;
		ReverseIterator i(n, sl.header); 
		return i;
	}

	template <typename T1, typename T2>
	T2 & 
	Map <T1, T2> ::at (const T1 & key)
	{
		Node <T1, T2> * n = sl.search(key);
		
		if (n == nullptr) throw std::out_of_range ("At(key): Key is not found !");
		else return n -> pair -> second;
	}

	template <typename T1, typename T2>
	const T2 & 
	Map <T1, T2> ::at (const T1 & key) const
	{
		Node <T1, T2> * n = sl.search(key);
		
		if (n == nullptr) throw std::out_of_range ("At(key) const: Key is not found !");
		else return n -> pair -> second;
	}

	template <typename T1, typename T2>
	T2 & 
	Map <T1, T2> ::operator [] (const T1 & key)
	{
		Node <T1, T2> * n = sl.search(key);

		if (n == nullptr)
		{
			T2 value = T2();
			Node <T1, T2> * n = sl.insert(std::make_pair(key, value));
			size_++;

			return n -> pair -> second;
		}
		else return n -> pair -> second;
	}

	template <typename T1, typename T2>
	bool 
	Map <T1, T2> ::operator == (const Map <T1, T2> & m)
	{
		if (this -> size_ == m.size_ && this -> sl == m.sl) return true;
		else return false;
	}

	template <typename T1, typename T2>
	bool 
	Map <T1, T2> ::operator != (const Map <T1, T2> & m)
	{
		if (this -> size_ != m.size_ || !(this -> sl == m.sl)) return true;
		else return false;
	}

	template <typename T1, typename T2>
	bool 
	Map <T1, T2> ::operator < (const Map <T1, T2> & m)
	{
		size_t m1_size = this -> size_;
		size_t m2_size = m.size_;
		char res = this -> sl < m.sl;

		if (res == 0) return true;
		else if (res == 1) return false;
		else if (res == 2) 
		{ 
			if (m1_size < m2_size) return true; 
			else return false;
		}
		else assert(false);
	}

	/*
	*
	*	ITERATOR FUNCTIONS
	*
	*/

	template <typename T1, typename T2>
	void
	Map <T1, T2> ::Iterator ::operator = (const Iterator & it)
	{
		this -> iter = it.iter;
		this -> header = it.header;
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::Iterator
	Map <T1, T2> ::Iterator ::operator ++ (int) 		// i++, post increment
	{
		Node <T1, T2> * original = this -> iter;
		this -> iter = this -> iter -> forward[0];

		return Iterator(original, this -> header);
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::Iterator &
	Map <T1, T2> ::Iterator ::operator ++ () 			// ++i, pre increment
	{
		if (this -> iter == nullptr) return * this;

		if (this -> iter -> forward[0] == nullptr) 
		{
			this -> iter = nullptr;
			return * this;
		}

		Node <T1, T2> * n = this -> iter -> forward[0]; 
		this -> iter = n;
	
		return * this;
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::Iterator
	Map <T1, T2> ::Iterator ::operator -- (int)
	{
		Node <T1, T2> * original = this -> iter;
		Node <T1, T2> * n = this -> header -> forward[0];
		Node <T1, T2> * prev_n = nullptr;
		bool iter_from_end = false;

		while (n != this -> iter) 
		{
			if (!n -> forward[0])
			{
				iter_from_end = true;
				break;
			}

			prev_n = n;
			n = n -> forward[0];
		}

		if (iter_from_end) { this -> iter = n; }
		else { this -> iter = prev_n; }

		return Iterator(original, this -> header);
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::Iterator &
	Map <T1, T2> ::Iterator ::operator -- ()
	{
		Node <T1, T2> * n = this -> header -> forward[0];
		Node <T1, T2> * prev_n = nullptr;
		bool iter_from_end = false;

		while (n != this -> iter) 
		{
			if (!n -> forward[0])
			{
				iter_from_end = true;
				break;
			}

			prev_n = n;
			n = n -> forward[0];
		}

		if (iter_from_end) { this -> iter = n; }
		else { this -> iter = prev_n; }
		return * this;
	}

	template <typename T1, typename T2>
	bool
	Map <T1, T2> ::Iterator ::operator == (const Iterator & it)
	{
		// If both nullptr, in case both sides are end()
		if (it.iter == nullptr && this -> iter == nullptr) return true;
		// If one is nullptr, in case one side is end()
		if (it.iter == nullptr || this -> iter == nullptr) return false;

		if (this -> iter -> pair -> first == it.iter -> pair -> first) return true;
		else return false;
	}

	template <typename T1, typename T2>
	bool
	Map <T1, T2> ::Iterator ::operator == (const ConstIterator & it)
	{
		// If both nullptr, in case both sides are end()
		if (it.iter == nullptr && this -> iter == nullptr) return true;
		// If one is nullptr, in case one side is end()
		if (it.iter == nullptr || this -> iter == nullptr) return false;

		if (this -> iter -> pair -> first == it.iter -> pair -> first) return true;
		else return false;
	}

	template <typename T1, typename T2>
	bool
	Map <T1, T2> ::Iterator ::operator != (const Iterator & it)
	{
		// In case of i++ resulting NULL is first operand to ==
		if (this -> iter == nullptr) return false;

		// In case of map::end() is second operand to !=
		if (it.iter == nullptr) return true;

		if (it.iter == nullptr || this -> iter == nullptr) return false;

		if (this -> iter -> pair -> first == it.iter -> pair -> first) return false;
		else return true;
	}

	template <typename T1, typename T2>
	bool
	Map <T1, T2> ::Iterator ::operator != (const ConstIterator & it)
	{
		// In case of i++ resulting NULL is first operand to ==
		if (this -> iter == nullptr) return false;

		// In case of map::end() is second operand to !=
		if (it.iter == nullptr) return true;

		if (it.iter == nullptr || this -> iter == nullptr) return false;

		if (this -> iter -> pair -> first == it.iter -> pair -> first) return false;
		else return true;
	}

	template <typename T1, typename T2>
	std::pair <T1, T2> &
	Map <T1, T2> ::Iterator ::operator * () const { return * (this -> iter -> pair); }

	template <typename T1, typename T2>
	std::pair <T1, T2> * 
	Map <T1, T2> ::Iterator ::operator -> () const { return this -> iter -> pair; }

	/*
	*
	*	CONST ITERATOR FUNCTIONS
	*
	*/

	template <typename T1, typename T2>
	void
	Map <T1, T2> ::ConstIterator ::operator = (const ConstIterator & it)
	{
		this -> iter = it.iter;
		this -> header = it.header;
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::ConstIterator
	Map <T1, T2> ::ConstIterator ::operator ++ (int) 		// i++, post increment
	{
		Node <T1, T2> * original = this -> iter;
		this -> iter = this -> iter -> forward[0];;

		return ConstIterator(original, this -> header);
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::ConstIterator &
	Map <T1, T2> ::ConstIterator ::operator ++ () 			// ++i, pre increment
	{
		Node <T1, T2> * n = this -> iter -> forward[0];
		this -> iter = n;
		return * this;
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::ConstIterator
	Map <T1, T2> ::ConstIterator ::operator -- (int)
	{
		Node <T1, T2> * original = this -> iter;
		Node <T1, T2> * n = this -> header -> forward[0];
		Node <T1, T2> * prev_n = nullptr;
		bool iter_from_end = false;

		while (n != this -> iter) 
		{
			if (!n -> forward[0])
			{
				iter_from_end = true;
				break;
			}

			prev_n = n;
			n = n -> forward[0];
		}

		if (iter_from_end) { this -> iter = n; }
		else { this -> iter = prev_n; }

		return ConstIterator(original, this -> header);
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::ConstIterator &
	Map <T1, T2> ::ConstIterator ::operator -- ()
	{
		Node <T1, T2> * n = this -> header -> forward[0];
		Node <T1, T2> * prev_n = nullptr;
		bool iter_from_end = false;

		while (n != this -> iter) 
		{
			if (!n -> forward[0])
			{
				iter_from_end = true;
				break;
			}

			prev_n = n;
			n = n -> forward[0];
		}

		if (iter_from_end) { this -> iter = n; }
		else { this -> iter = prev_n; }
		return * this;
	}

	template <typename T1, typename T2>
	bool
	Map <T1, T2> ::ConstIterator ::operator == (const ConstIterator & it)
	{
		// If both nullptr, in case both sides are end()
		if (it.iter == nullptr && this -> iter == nullptr) return true;
		// If one is nullptr, in case one side is end()
		if (it.iter == nullptr || this -> iter == nullptr) return false;

		if (this -> iter -> pair -> first == it.iter -> pair -> first) return true;
		else return false;
	}

	template <typename T1, typename T2>
	bool
	Map <T1, T2> ::ConstIterator ::operator == (const Iterator & it)
	{
		// If both nullptr, in case both sides are end()
		if (it.iter == nullptr && this -> iter == nullptr) return true;
		// If one is nullptr, in case one side is end()
		if (it.iter == nullptr || this -> iter == nullptr) return false;

		if (this -> iter -> pair -> first == it.iter -> pair -> first) return true;
		else return false;
	}

	template <typename T1, typename T2>
	bool
	Map <T1, T2> ::ConstIterator ::operator != (const ConstIterator & it)
	{
		// In case of i++ resulting NULL is first operand to ==
		if (this -> iter == nullptr) return false;

		// In case of map::end() is second operand to !=
		if (it.iter == nullptr) return true;

		if (it.iter == nullptr || this -> iter == nullptr) return false;

		if (this -> iter -> pair -> first == it.iter -> pair -> first) return false;
		else return true;
	}

	template <typename T1, typename T2>
	bool
	Map <T1, T2> ::ConstIterator ::operator != (const Iterator & it)
	{
		// In case of i++ resulting NULL is first operand to ==
		if (this -> iter == nullptr) return false;

		// In case of map::end() is second operand to !=
		if (it.iter == nullptr) return true;

		if (it.iter == nullptr || this -> iter == nullptr) return false;

		if (this -> iter -> pair -> first == it.iter -> pair -> first) return false;
		else return true;
	}

	template <typename T1, typename T2>
	const std::pair <T1, T2> &
	Map <T1, T2> ::ConstIterator ::operator * () const { return * (this -> iter -> pair); }

	template <typename T1, typename T2>
	const std::pair <T1, T2> * 
	Map <T1, T2> ::ConstIterator ::operator -> () const { return this -> iter -> pair; }

	/*
	*
	*	REVERSE ITERATOR FUNCTIONS
	*
	*/

	template <typename T1, typename T2>
	void
	Map <T1, T2> ::ReverseIterator ::operator = (const ReverseIterator & it)
	{
		this -> iter = it.iter;
		this -> header = it.header;
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::ReverseIterator
	Map <T1, T2> ::ReverseIterator ::operator ++ (int) 			// i++, post increment (actually post decrement)
	{
		Node <T1, T2> * original = this -> iter;
		Node <T1, T2> * n = this -> header -> forward[0];
		Node <T1, T2> * prev_n = nullptr;
		bool iter_from_end = false;

		while (n != this -> iter) 
		{
			if (!n -> forward[0])
			{
				iter_from_end = true;
				break;
			}

			prev_n = n;
			n = n -> forward[0];
		}

		if (iter_from_end) { this -> iter = n; }
		else { this -> iter = prev_n; }

		return ReverseIterator(original, this -> header);
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::ReverseIterator &
	Map <T1, T2> ::ReverseIterator ::operator ++ () 			// ++i, pre increment (Actually pre decrement)
	{
		Node <T1, T2> * n = this -> header -> forward[0];
		Node <T1, T2> * prev_n = this -> header;
		bool iter_from_end = false;

		while (n != this -> iter) 
		{
			if (!n -> forward[0])
			{
				iter_from_end = true;
				break;
			}

			prev_n = n;
			n = n -> forward[0];
		}

		if (iter_from_end) { this -> iter = n; }
		else { this -> iter = prev_n; }
		return * this;
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::ReverseIterator
	Map <T1, T2> ::ReverseIterator ::operator -- (int)
	{
		Node <T1, T2> * original = this -> iter;
		this -> iter = this -> iter -> forward[0];;

		return ReverseIterator(original, this -> header);
	}

	template <typename T1, typename T2>
	typename Map <T1, T2> ::ReverseIterator &
	Map <T1, T2> ::ReverseIterator ::operator -- ()
	{
		Node <T1, T2> * n = this -> iter -> forward[0];
		this -> iter = n;
		return * this;
	}

	template <typename T1, typename T2>
	bool
	Map <T1, T2> ::ReverseIterator ::operator == (const ReverseIterator & it)
	{
		// If both nullptr, in case both sides are end()
		if (it.iter == nullptr && this -> iter == nullptr) return true;

		// If one is nullptr, in case one side is end()
		if (it.iter == nullptr || this -> iter == nullptr) return false;

		if (this -> iter -> pair -> first == it.iter -> pair -> first) return true;
		else return false;
	}

	template <typename T1, typename T2>
	bool
	Map <T1, T2> ::ReverseIterator ::operator != (const ReverseIterator & it)
	{
		// In case of i++ resulting NULL is first operand to ==
		if (this -> iter == nullptr) return false;

		// In case of map::end() is second operand to !=
		if (it.iter == nullptr) return true;

		if (this -> iter -> is_header && it.iter -> is_header) return false;

		// In case of map::rend() is second operand to !=
		if (it.iter -> is_header) return true;

		if (this -> iter -> pair -> first == it.iter -> pair -> first) return false;
		else return true;
	}

	template <typename T1, typename T2>
	std::pair <T1, T2> &
	Map <T1, T2> ::ReverseIterator ::operator * () const { return * (this -> iter -> pair); }

	template <typename T1, typename T2>
	std::pair <T1, T2> * 
	Map <T1, T2> ::ReverseIterator ::operator -> () const { return this -> iter -> pair; }
}

#endif
