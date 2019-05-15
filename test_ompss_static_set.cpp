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

const static int Narray[] = {4, 5, 1, 9, 20, 8, 51};

using namespace std;

int main()
{
	set<int> theset;

	for (size_t i = 0; i < sizeof(Narray) / sizeof(int); ++i)
		theset.insert(Narray[i]);

	// Constructor from std:;set
	ompss_static_set<int> ompssset(theset);

	auto a = theset.begin();
	auto b= ompssset.begin();
	for (; a != theset.end() && b != ompssset.end(); ++a, ++b)
		cout << *a << " =? " << *b <<endl;

	a = theset.begin();
	b= ompssset.begin();
	for (; a != theset.end() && b != ompssset.end(); ++a, ++b) {
		assert(*a == *b);
	}

	// Insert element
	ompssset.insert(-1);
	ompssset.insert(2);
	ompssset.insert(10);
	ompssset.insert(52);

	theset.insert(-1);
	theset.insert(2);
	theset.insert(10);
	theset.insert(52);

	a = theset.begin();
	b= ompssset.begin();
	for (; a != theset.end() && b != ompssset.end(); ++a, ++b)
		cout << *a << " =? " << *b <<endl;

	a = theset.begin();
	b= ompssset.begin();
	for (; a != theset.end() && b != ompssset.end(); ++a, ++b)
		assert(*a == *b);


	return 0;
}
