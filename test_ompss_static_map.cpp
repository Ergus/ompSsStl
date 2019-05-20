


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
#include <map>
#include <cassert>

#include "ompss_static_map.hpp"

const static int Iarray[] = {2, 3, 4, 7, 9, 12, 13}; // keep this sorted
const static int Narray[] = {4, 5, 1, 9, 20, 8, 51};

using namespace std;

int main()
{
	map<int, int> themap;

	// Fill a map
	for (size_t i = 0; i < sizeof(Iarray) / sizeof(int); ++i)
		themap[Iarray[i]] = Narray[i];

	// Constructor from an std::map
	ompss_static_map<int, int> ompssmap(themap);

	// Test the search algorithm
	for (size_t i = 0; i < sizeof(Iarray) / sizeof(int); ++i) {
		cout << "["<< Iarray[i] <<"]: "
		     << themap[Iarray[i]] << " =? "
		     << ompssmap[Iarray[i]] << endl;
		assert(themap[Iarray[i]] == ompssmap[Iarray[i]]);
	}

	// Check they are on the same order and the iterator
	size_t i = 0;
	for (auto a : ompssmap) {
		cout << a << "=? <" << Iarray[i] <<","<< Narray[i] << ">" << endl;
		assert(a.first == Iarray[i]);
		assert(a.second == Narray[i]);
		assert(themap[Iarray[i]] == ompssmap[Iarray[i]]);
		++i;
	}

	// Test the size function
	cout << "Size before: " << ompssmap.size() << " =? " << themap.size() << endl;
	assert(ompssmap.size() == themap.size());

	// Insert elements in different positions
	ompssmap[1] = -1;
	ompssmap[6] = -3;
	ompssmap[5] = -2;
	ompssmap[9] = -4;
	ompssmap[15] = -5;

	themap[1] = -1;
	themap[6] = -3;
	themap[5] = -2;
	themap[9] = -4;
	themap[15] = -5;

	auto a = themap.begin();
	auto b = ompssmap.begin();
	for (; a != themap.end() && b != ompssmap.end(); ++a, ++b) {
		cout << *a << " =? " << *b << endl;
		assert(*a == *b);
	}

	// Test the size function
	cout << "Size after: " << ompssmap.size() << " =? " << themap.size() << endl;
	assert(ompssmap.size() == themap.size());

	// lower_bounds test
	for (int i = 0; i < 20; ++i)
	{
		a = themap.lower_bound(i);
		b = ompssmap.lower_bound(i);

		if (a == themap.end()) {
			cout << i << ": After end? " ;
			assert(b == ompssmap.end());
			cout << "OK!" << endl;
		} else {
			cout << i << " " << *a << " =? " << *b << endl;
			assert(*a == *b);
		}
	}


	return 0;
}
