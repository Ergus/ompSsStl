/*
 * Copyright (C) 2019  Jimmy Aguilar Mena
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <vector>
#include <cassert>

#include "ompss_static_vector.hpp"

const static int Narray[] = {4, 5, 1, 9, 12, 8, 15};

using namespace std;

int main()
{
	vector<int> thevector;

	// Fill a vector
	for (size_t i = 0; i < sizeof(Narray) / sizeof(int); ++i)
		thevector.push_back(Narray[i]);

	// Constructor from std::vector
	ompss_static_vector<int> ompssvector(thevector);

	// test the search algorithm
	auto a = thevector.begin();
	auto b = ompssvector.begin();
	for (; a != thevector.end() && b != ompssvector.end(); ++a, ++b) {
		cout << *a << " =? " << *b <<endl;
		assert(*a == *b);
	}

	// Test the size function
	cout << "Size before: " << ompssvector.size() << " =? " << thevector.size() << endl;
	assert(ompssvector.size() == thevector.size());

	const int tmp_array[] = {-1, 2, 10, 18, 4, 5};

	cout << "Test insert" << endl;
	for (auto &i : tmp_array) {
		ompssvector.push_back(i);
		thevector.push_back(i);
	}

	a = thevector.begin();
	b= ompssvector.begin();
	for (; a != thevector.end() && b != ompssvector.end(); ++a, ++b) {
		cout << *a << " =? " << *b <<endl;
		assert(*a == *b);
	}

	// Test the size function
	cout << "Size after: " << ompssvector.size() << " =? " << thevector.size() << endl;
	assert(ompssvector.size() == thevector.size());


	// Test operator []
	const size_t lastindex = ompssvector.size();

	for (size_t i = 0; i < lastindex; ++i) {
		cout << i << ": " << thevector[i] << " =? " << ompssvector[i] <<endl;
		assert(thevector[i] == ompssvector[i]);
	}

	return 0;
}
