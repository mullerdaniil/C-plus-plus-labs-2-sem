#include <iostream>
#include <vector>


template <class Iterator, class Predicate>
bool all_of(Iterator begin, Iterator end, Predicate predicate) {
	for (Iterator iter = begin; iter != end; iter++) {
		if (!predicate(*iter))
			return false;
	}
	return true;
}


template <class Iterator, class Predicate>
bool any_of(Iterator begin, Iterator end, Predicate predicate) {
	for (Iterator iter = begin; iter != end; iter++) {
		if (predicate(*iter))
			return true;
	}
	return false;
}


template <class Iterator, class Predicate>
bool none_of(Iterator begin, Iterator end, Predicate predicate) {
	for (Iterator iter = begin; iter != end; iter++) {
		if (predicate(*iter))
			return false;
	}
	return true;
}


template <class Iterator, class Predicate>
bool one_of(Iterator begin, Iterator end, Predicate predicate) {
	size_t count = 0;
	for (Iterator iter = begin; iter != end; iter++) {
		if (predicate(*iter))
			count++;
	}
	return count == 1;
}


template <class Iterator, class Criteria>
bool is_sorted(Iterator begin, Iterator end, Criteria criteria) {
	if (begin == end)
		return false;
	
	Iterator current = begin;
	Iterator prev = current;
	
	while (++current != end) {
		if (!criteria(*prev, *current))
			return false;
		prev = current;
	}
	return true;
}


template <class Iterator, class Predicate>
bool is_partitioned(Iterator begin, Iterator end, Predicate predicate) {
	Iterator current = begin;
	while (current != end && predicate(*current)) {
		current++;
	}
	
	while (current != end) {
		if (predicate(*current))
			return false;
		current++;
	}
	
	return true;
}


template <class Iterator, class T>
Iterator find_not(Iterator begin, Iterator end, const T& element) {
	for (Iterator iter = begin; iter != end; iter++) {
		if (*iter != element)
			return iter;
	}
	return end;
}


template <class Iterator, class T>
Iterator find_backward(Iterator begin, Iterator end, const T& element) {
	Iterator current = end;
	while (current-- != begin) {
		if (*current == element)
			return current;
	}
	return end;
}


template <class Iterator, class Condition>
bool is_palindrome(Iterator begin, Iterator end, Condition condition) {
	Iterator left = begin;
	Iterator right = end;
	right--;
	
	while (left < right) {
		if (!condition(*left, *right))
			return false;
		left++;
		right--;
	}
	
	return true;
}







struct CPoint {
	int x;
	int y;
};


int main() {
	
	
	// vector / CPoint test
	std::vector<CPoint> v1;
	v1.push_back({1, 2});
	v1.push_back({2, 4});
	v1.push_back({3, 8});
	
	std::cout << "all_of: " << all_of(v1.begin(), v1.end(), [](const CPoint& p){ return p.y % 2 == 0; }) << std::endl;
	std::cout << "any_of: " << any_of(v1.begin(), v1.end(), [](const CPoint& p){ return p.y % 2 == 1; }) << std::endl;
	std::cout << "none_of: " << none_of(v1.begin(), v1.end(), [](const CPoint& p){ return p.x > 12; }) << std::endl;
	std::cout << "one_of: " << one_of(v1.begin(), v1.end(), [](const CPoint& p){ return p.x == 3; }) << std::endl;
	
	
	// vector / int test
	std::vector<int> v2;
	v2.push_back(3);
	v2.push_back(5);
	v2.push_back(5);
	
	std::cout << "is_sorted: " << is_sorted(v2.begin(), v2.end(), [](int n1, int n2){ return n1 <= n2; }) << std::endl;
	std::cout << "is_partitioned: " << is_partitioned(v2.begin(), v2.end(), [](int n){ return n == 3; }) << std::endl;
	
	if (find_not(v2.begin(), v2.end(), 3) != v2.end())
		std::cout << "find_not(3): index = " << find_not(v2.begin(), v2.end(), 3) - v2.begin() << std::endl;
	
	if (find_backward(v2.begin(), v2.end(), 3) != v2.end())
		std::cout << "find_backward(5): index = " << find_backward(v2.begin(), v2.end(), 5) - v2.begin() << std::endl;
		
	
	// string test
	std::string stringsArray[] = { "hello", "", "abacaba", "abbac", "daad", "c", "cacc"};
	int arrSize = 7;
	
	std::cout << "is_palindrome:" << std::endl;
	for (int i = 0; i < arrSize; i++)
		std::cout << "\"" << stringsArray[i] << "\"      \t" << is_palindrome(stringsArray[i].begin(), stringsArray[i].end(), [](char c1, char c2){ return c1 == c2;}) << std::endl;
	
		
	
	return 0;
}
