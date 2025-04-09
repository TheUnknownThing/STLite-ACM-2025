#include "../../src/map.hpp"
#include <iostream>
#include <cassert>
#include <string>

class Integer {
public:
	static int counter;
	int val;
	
	Integer(int val) : val(val) {
		counter++;
	}

	Integer(const Integer &rhs) {
		val = rhs.val;
		counter++;
	}

	Integer& operator = (const Integer &rhs) {
		assert(false);
	}

	~Integer() {
		counter--;
	}
};

int Integer::counter = 0;

class Compare {
public:
	bool operator () (const Integer &lhs, const Integer &rhs) const {
		return lhs.val < rhs.val;
	}
};

void tester(void) {
	//	test: constructor
	sjtu::map<Integer, std::string, Compare> map;
	//	test: empty(), size()
	assert(map.empty() && map.size() == 0);
	//	test: operator[], insert()
	for (int i = 0; i < 10000; ++i) {
		std::string string = "";
		for (int number = i; number; number /= 10) {
			char digit = '0' + number % 10;
			string = digit + string;
		}
		// std::cout << "string: " << string << std::endl; // debug
		if (i & 1) {
			// std::cout << 1 << std::endl; // debug
			map[Integer(i)] = string;
			/*std::string* traverse_all = map.traverse_all();
			for (int j = 0; j < map.size(); ++j) {
				std::cout << traverse_all[j] << " ";
			}
			std::cout << std::endl;*/ // debug*
			auto result = map.insert(sjtu::pair<Integer, std::string>(Integer(i), string));
			assert(!result.second);
		} else {
			// std::cout << 0 << std::endl; // debug
			auto result = map.insert(sjtu::pair<Integer, std::string>(Integer(i), string));
			assert(result.second);
		}
	}
	//	test: count(), find(), erase()
	for (int i = 0; i < 10000; ++i) {
		if (i > 1896 && i <= 2016) {
			continue;
		}
		assert(map.count(Integer(i)) == 1);
		assert(map.find(Integer(i)) != map.end());
		map.erase(map.find(Integer(i)));
	}
	std::cout << "second test done" << std::endl;
	//	test: constructor, operator=, clear();
	for (int i = 0; i < (int)map.size(); ++i) {
		sjtu::map<Integer, std::string, Compare> copy(map);
		map.clear();
		std::cout << map.size() << " " << copy.size() << " ";
		map = copy;
		copy.clear();
		std::cout << map.size() << " " << copy.size() << " ";
		copy = map;
		map.clear();
		std::cout << map.size() << " " << copy.size() << " ";
		map = copy;
		copy.clear();
		std::cout << map.size() << " " << copy.size() << " ";
	}
	std::cout << std::endl;
	std::cout << "third test done" << std::endl;
	//	test: const_iterator, cbegin(), cend(), operator++, at()
	sjtu::map<Integer, std::string, Compare>::const_iterator const_iterator;
	const_iterator = map.cbegin();
	int counter = 0;
	while (const_iterator != map.cend()) {
		const Integer integer(const_iterator->first);
		counter++;
		const_iterator++;
		std::cout << map.at(integer) << " ";
		std::cout << counter << std::endl; // debug
	}
	std::cout << std::endl;
	std::cout << "fourth test done" << std::endl;
	//	test: iterator, operator--, operator->
	sjtu::map<Integer, std::string, Compare>::iterator iterator;
	iterator = map.end();
	while (true) {
		sjtu::map<Integer, std::string, Compare>::iterator peek = iterator;
		if (peek == map.begin()) {
			std::cout << std::endl;
			break;
		}
		std::cout << (--iterator)->second << " ";
	}
	//	test: erase()
	while (map.begin() != map.end()) {
		map.erase(map.begin());
	}
	assert(map.empty() && map.size() == 0);
	//	test: operator[]
	for (int i = 0; i < 100000; ++i) {
		std::cout << map[Integer(i)];
	}
	std::cout << map.size() << std::endl;
}

int main(void) {
	tester();
	std::cout << Integer::counter << std::endl;
}
