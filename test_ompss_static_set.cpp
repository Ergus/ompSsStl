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

const static int Narray[] = {4, 5, 1, 9, 20, 8, 51};

using namespace std;

int main()
{
	map<int, int> themap;

	for (size_t i = 0; i < sizeof(Iarray) / sizeof(int); ++i)
		themap[Iarray[i]] = Narray[i];

	ompss_static_map<int, int> ompssmap(themap);

	for (size_t i = 0; i < sizeof(Iarray) / sizeof(int); ++i)
		cout << "["<< Iarray[i] <<"]: "
		     << themap[Iarray[i]] << " =? "
		     << ompssmap[Iarray[i]] << endl;

	size_t i = 0;
	for (auto a : ompssmap) {
		assert(a.first == Iarray[i]);
		assert(a.second == Narray[i]);
		assert(themap[Iarray[i]] == ompssmap[Iarray[i]]);
		++i;
	}

	ompssmap[1] = -1;
	ompssmap[5] = -2;
	ompssmap[6] = -3;
	ompssmap[7] = -4;
	ompssmap[15] = -5;
	for (auto a : ompssmap)
		cout << a.first << " " << a.second << endl;

	return 0;
}
