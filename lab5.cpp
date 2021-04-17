#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <vector>

template <class T>
class CircularBuffer {
	T* bufferData;
	
	size_t minCapacity = 0;
	size_t maxCapacity = 0;

	size_t capacity = 0;
	size_t size = 0;
	
	size_t beginIndex = 0;
	size_t endIndex = 0;
	
	int beginOffset = 0;
	int endOffset = 0;
	
	
	void updateCapacity(size_t newCapacity) {
		if (newCapacity != -1 && minCapacity <= newCapacity && newCapacity <= maxCapacity) {
			setCapacity(newCapacity);
		}
	}
	
	
	void setCapacity(size_t capacity) {
		if (capacity == this->capacity || capacity < 0)
			return;
		
		T* temp = new T[capacity];
		size_t copied = 0;
		size_t index = beginIndex;
		size_t newSize = this->size < capacity ? this->size : capacity;
		
		while (copied < newSize) {
			temp[copied++] = bufferData[index];
			index = (index + 1) % this->capacity;
		}
		
		delete[] bufferData;
		bufferData = temp;
		
		beginIndex = 0;
		endIndex = newSize == capacity ? 0 : newSize;
		beginOffset = 0;
		endOffset = newSize == capacity ? 1 : 0;
		
		this->capacity = capacity;
		this->size = newSize;
		
	}
	
public:
		
	CircularBuffer(size_t maxCapacity) {
		this->maxCapacity = maxCapacity;
		this->capacity = minCapacity;
		bufferData = new T[capacity];
	}
	
	
	CircularBuffer(const CircularBuffer& other) : 
		capacity(other.capacity),
		size(other.size),
		beginIndex(other.beginIndex),
		endIndex(other.endIndex),
		beginOffset(other.beginOffset),
		endOffset(other.endOffset),
		minCapacity(other.minCapacity),
		maxCapacity(other.maxCapacity) {
		
		bufferData = new T[capacity];
		for (size_t i = 0; i < capacity; i++)
			bufferData[i] = other.bufferData[i];
	
	}
	
	
	void swap(CircularBuffer& other) {
		std::swap(capacity, other.capacity);
		std::swap(size, other.size);
		std::swap(beginIndex, other.beginIndex);
		std::swap(endIndex, other.endIndex);
		std::swap(beginOffset, other.beginOffset);
		std::swap(endOffset, other.endOffset);
		std::swap(bufferData, other.bufferData);
		std::swap(minCapacity, other.minCapacity);
		std::swap(maxCapacity, other.maxCapacity);
	}
	
	
	CircularBuffer& operator=(const CircularBuffer& other) {
		CircularBuffer<T> temp(other);
		(*this).swap(temp);
		return *this;
	}
	
	
	~CircularBuffer() {
		delete[] bufferData;
	};
	
	
	T& operator[](int index) {
		return bufferData[(beginIndex + index) % capacity];
	}
	
	
	const T& front() {
		return bufferData[beginIndex];
	}
	
	const T& back() {
		return bufferData[(endIndex + capacity - 1) % capacity];
	}
	
	
	
	void setMinCapacity(size_t capacity) {
		if (capacity >= 0) {
			minCapacity = capacity;
			if (this->capacity < minCapacity)
				setCapacity(minCapacity);
		}
	}
	
	void setMaxCapacity(size_t capacity) {
		if (capacity >= 0) {
			maxCapacity = capacity;
			if (this->capacity > maxCapacity)
				setCapacity(maxCapacity);
		}
	}
	
	size_t getMinCapacity() {
		return minCapacity;
	}
	
	size_t getMaxCapacity() {
		return maxCapacity;
	}
	
	
	void pushFront(const T& element) {
		updateCapacity(capacity + 1);
		
		if (beginIndex == 0) {
			beginIndex = capacity - 1;
			beginOffset--;
		} else {
			beginIndex--;
		}
		bufferData[beginIndex] = element;
		
		if (size == capacity) {
			if (endIndex == 0) {
				endOffset--;
				endIndex = capacity - 1;
			} else {
				endIndex--;
			}
			size--;
		}
		size++;
	}
	
	
	void pushBack(const T& element) {
		updateCapacity(capacity + 1);
		
		bufferData[endIndex] = element;
		if (endIndex == capacity - 1)
			endOffset++;
		endIndex = (endIndex + 1) % capacity;
		
		if (size == capacity) {
			if (beginIndex == capacity - 1)
				beginOffset++;
			beginIndex = (beginIndex + 1) % capacity;
			size--;
		}
		
		size++;	
	}
	
	
	void popFront() {
		if (capacity == 0 || capacity > 0 && capacity - 1 < minCapacity)
			return;
		
		if (size > 0) {
			if (beginIndex == capacity - 1) {
				beginIndex = 0;
				beginOffset++;	
			} else {
				beginIndex++;
			}
			
			size--;
		}
		
		updateCapacity(capacity - 1);
	}
	
	
	void popBack() {
		if (capacity == 0 || capacity > 0 && capacity - 1 < minCapacity)
			return;
		
		if (size > 0) {
			if (endIndex == 0) {
				endIndex = capacity - 1;
				endOffset--;
			} else {
				endIndex--;
			}
			
			size--;
		}
		
		updateCapacity(capacity - 1);
	}
	
	

	


	class iterator {
		CircularBuffer<T>* buffer;
		T* pointer_;
		int offset;
		
		
	public:
		
        typedef int difference_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
		typedef std::random_access_iterator_tag iterator_category;
		
		iterator(CircularBuffer<T>& bf, T* ptr, int offs) :
			buffer(&bf),
			pointer_(ptr),
			offset(offs) {

		}
		
		iterator() {
			
		}
		
		T& operator*() const {
			return *pointer_;
			
		}
		
		T& operator->() const {
			return &(*pointer_);
		}

        iterator& operator++() {
        	pointer_++;
			if (pointer_ == (buffer->bufferData + buffer->capacity)) {
				pointer_ = buffer->bufferData;
				offset++;
			}            
            
            return *this;
        };
		
		iterator operator++(int) {
			iterator temp(*this);
			this->operator++();
			return temp;
		}
		
        iterator& operator--() {
			if (pointer_ == buffer->bufferData) {
				pointer_ = buffer->bufferData + buffer->capacity - 1;
				offset--;
			} else {
				pointer_--;
			}            
            
            return *this;
        };
		
		iterator operator--(int) {
			iterator temp(*this);
			this->operator--();
			return temp;
		}
		
		difference_type operator-(const iterator& iter2) {
			return buffer->capacity * offset + (pointer_ - buffer->bufferData) 
				- iter2.buffer->capacity * iter2.offset - (iter2.pointer_ - iter2.buffer->bufferData);
		}
		
		iterator operator-(int number) {
			int dif = buffer->capacity * offset + (pointer_ - buffer->bufferData) - number;
			int offs = dif / buffer->capacity;
			T* ptr = buffer->bufferData + (dif % buffer->capacity);
			return iterator(*buffer, ptr, offs);
		}
		
		difference_type operator+(const iterator& iter2) {
			return buffer->capacity * offset + (pointer_ - buffer->bufferData) 
				+ iter2.buffer->capacity * iter2.offset + (iter2.pointer_ - iter2.buffer->bufferData);
		}
		
		iterator operator+(int number) {
			int dif = buffer->capacity * offset + (pointer_ - buffer->bufferData) + number;
			int offs = dif / buffer->capacity;
			T* ptr = buffer->bufferData + (dif % buffer->capacity);
			return iterator(*buffer, ptr, offs);
		}
		
		bool operator==(const iterator& iter2) const {
			return pointer_ == iter2.pointer_ && offset == iter2.offset;
		}
		
		bool operator!=(const iterator& iter2) const {
			return !(*this == iter2);
		}
		
		bool operator<(const iterator& iter2) const {
			return offset == iter2.offset && pointer_ < iter2.pointer_
					|| offset < iter2.offset;
		}
		
		bool operator<=(const iterator& iter2) const {
			return offset == iter2.offset && pointer_ <= iter2.pointer_
					|| offset < iter2.offset;
		}
		
		bool operator>(const iterator& iter2) const {
			return iter2 < *this;
		}
		
		bool operator>=(const iterator& iter2) const {
			return iter2 <= *this;
		}
		
	
		T& operator[](int index) const {
			return *pointer_;
		}
	
	};
	
	
	iterator begin() {
		return iterator(*this, bufferData + beginIndex, 0);
	}
	
	
	iterator end() {
		if (size == 0)
			return begin();
		return iterator(*this, bufferData + endIndex, endIndex <= beginIndex);
	}
	
	
	size_t getCapacity() const {
		return capacity;
	}
	
	
	bool empty() const {
		return size == 0;
	}
	
	size_t getSize() const {
		return size;
	}
};







int main() {
	
	
	
	// create
	CircularBuffer<int> buf1(5);
	std::cout << "created buffer with capacity = " << buf1.getCapacity() << "\n\n\n";
	
	// fill
	buf1.pushBack(1);
	buf1.pushBack(2);
	buf1.pushBack(3);
	buf1.pushBack(4);
	buf1.pushBack(5);
	buf1.pushBack(6);
	
	
	// output elements
	std::cout << "elements:\n";
	for (CircularBuffer<int>::iterator iter = buf1.begin(); iter != buf1.end(); iter++)
		std::cout << "\t" << *iter << "\n";
	std::cout << "\n\n\n";

	
	// operations
	buf1.popFront();
	std::cout << "after popFront():\n";
	for (CircularBuffer<int>::iterator iter = buf1.begin(); iter != buf1.end(); iter++)
		std::cout << "\t" << *iter << "\n";
	std::cout << "\n\n\n";
	
	
	buf1.pushBack(10);
	buf1.pushBack(12);
	std::cout << "after pushBack(10), pushBack(12):\n";
	for (CircularBuffer<int>::iterator iter = buf1.begin(); iter != buf1.end(); iter++)
		std::cout << "\t" << *iter << "\n";
	std::cout << "\n\n\n";	
	
	buf1.setMaxCapacity(10);
	buf1.pushBack(31);
	buf1.pushBack(32);
	buf1.pushBack(33);
	std::cout << "after setCapacity(10), pushBack(31), pushBack(32), pushBack(33):\n";
	for (CircularBuffer<int>::iterator iter = buf1.begin(); iter != buf1.end(); iter++)
		std::cout << "\t" << *iter << "\n";
	std::cout << "\n\n\n";	
	
	buf1.setMaxCapacity(4);
	std::cout << "after setCapacity(4):\n";
	for (CircularBuffer<int>::iterator iter = buf1.begin(); iter != buf1.end(); iter++)
		std::cout << "\t" << *iter << "\n";
	std::cout << "\n\n\n\n\n";	

	
	
	// apply algorithms
	std::cout << "all_of( x -> x < 30 ): " << std::all_of(buf1.begin(), buf1.end(), [](int x){ return x < 12; }) << "\n\n";
	std::cout << "index of find( 6 ): " << (std::find(buf1.begin(), buf1.end(), 6) - buf1.begin()) << "\n\n";
	std::cout << "find( 1 ) ? : " << (std::find(buf1.begin(), buf1.end(), 1) != buf1.end()) << "\n\n";
	
	CircularBuffer<int> buf2(4);
	buf2.pushBack(7);
	buf2.pushBack(16);
	buf2.pushBack(10);
	buf2.pushBack(6);
	for (CircularBuffer<int>::iterator iter = buf2.begin(); iter != buf2.end(); iter++)
		std::cout << "\t" << *iter << "\n";
	std::cout << "\n\n\n";	
	std::sort(buf2.begin(), buf2.end(), [](int n1, int n2){ return n1 < n2; });
	std::cout << "After sort by (n1, n2) -> n1 < n2 :\n";
	for (CircularBuffer<int>::iterator iter = buf2.begin(); iter != buf2.end(); iter++)
		std::cout << "\t" << *iter << "\n";
	std::cout << "\n\n\n";
	
	//buf2.setCapacity(10);
	buf2.pushFront(31);
	buf2.pushFront(32);
	std::cout << "After setCapacity(10), pushFront(31), pushFront(32), sort by (n1, n2) -> n1 < n2 :\n";
	std::sort(buf2.begin(), buf2.end(), [](int n1, int n2){ return n1 < n2; });	
	for (CircularBuffer<int>::iterator iter = buf2.begin(); iter != buf2.end(); iter++)
		std::cout << "\t" << *iter << "\n";
	std::cout << "\n\n\n";
	
	std::replace_if(buf2.begin(), buf2.end(), [](int x){ return x % 2 == 1; }, -5);	
	std::cout << "After replace_if( x -> x % 2 == 1 ) with -5: \n";
	for (CircularBuffer<int>::iterator iter = buf2.begin(); iter != buf2.end(); iter++)
		std::cout << "\t" << *iter << "\n";
	std::cout << "\n\n\n";
	
	
	// dereference
	std::cout << "buf2[2] = " << buf2[2] << "\n";
	std::cout << "buf2.front() = " << buf2.front() << "\n";
	std::cout << "buf2.back() = " << buf2.back() << "\n";
	std::cout << "after buf2[3] = 45\n";
	buf2[3] = 45;
	for (CircularBuffer<int>::iterator iter = buf2.begin(); iter != buf2.end(); iter++)
		std::cout << "\t" << *iter << "\n";
	std::cout << "\n\n\n";
	
	// maxCapacity
	std::cout << "Current capacity: " << buf2.getCapacity() << "\n";
	std::cout << "Max capacity: " << buf2.getMaxCapacity() << "\n";
	std::cout << "Set maxCapacity to 10\n\n";
	buf2.setMaxCapacity(10);
	
	for (int i = 0; i < 12; i++)
	{
		std::cout << "after push_back(" << i << ")\n";
		buf2.pushBack(i);
		std::cout << "current capacity : " << buf2.getCapacity() << "\n";
		for (CircularBuffer<int>::iterator iter = buf2.begin(); iter != buf2.end(); iter++)
			std::cout << "\t" << *iter << "\n";
		std::cout << "\n\n";
		
	}
	


	return 0;
}
