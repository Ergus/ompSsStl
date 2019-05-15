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
#include <set>
#include <cassert>

#include "ompss_static_set.hpp"

const static int Narray[] = {4, 5, 1, 9, 12, 8, 15};

using namespace std;

int main()
{
	set<int> theset;

	// Fill a set
	for (size_t i = 0; i < sizeof(Narray) / sizeof(int); ++i)
		theset.insert(Narray[i]);

	// Constructor from std:;set
	ompss_static_set<int> ompssset(theset);

	// test the search algorithm
	auto a = theset.begin();
	auto b = ompssset.begin();
	for (; a != theset.end() && b != ompssset.end(); ++a, ++b) {
		cout << *a << " =? " << *b <<endl;
		assert(*a == *b);
	}

	// Test the size function
	cout << "Size before: " << ompssset.size() << " =? " << theset.size() << endl;
	assert(ompssset.size() == theset.size());


	// Insert element
	ompssset.insert(-1);
	ompssset.insert(2);
	ompssset.insert(10);
	ompssset.insert(18);

	theset.insert(-1);
	theset.insert(2);
	theset.insert(10);
	theset.insert(18);

	a = theset.begin();
	b= ompssset.begin();
	for (; a != theset.end() && b != ompssset.end(); ++a, ++b) {
		cout << *a << " =? " << *b <<endl;
		assert(*a == *b);
	}

	// Test the size function
	cout << "Size after: " << ompssset.size() << " =? " << theset.size() << endl;
	assert(ompssset.size() == theset.size());

	for (int i = 0; i < 20; ++i)
	{
		a = theset.lower_bound(i);
		b = ompssset.lower_bound(i);

		if (a == theset.end()) {
			cout << i << ": After end? " ;
			assert(b == ompssset.end());
			cout << "OK!" << endl;
		} else {
			cout << i << " " << *a << " =? " << *b << endl;
			assert(*a == *b);
		}
	}

	return 0;
}
