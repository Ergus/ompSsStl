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

#ifndef OMPSS_STATIC_SET
#define OMPSS_STATIC_SET


#include <set>
#include <algorithm>

#include "ompss_static_buffer.hpp"

/*
 *  This is the simplest implementation for a fix sized map with
 *  templates, keeping the log complexity to search and compatible with
 *  ompss without using allocation hacks.
 */
template <typename _Key,
          typename _Alloc = std::allocator<_Key>>
class ompss_static_set
{
public:
	typedef _Key     key_type;
	typedef _Key     value_type;
	typedef _Alloc   allocator_type;

	// Iterator
	typedef  value_type *iterator;
	typedef const value_type *const_iterator;

	ompss_static_set(std::set<key_type> &in) :
		_buffer(in.size() + 10)  // TODO: This 10 is completely arbitrary now.
	{
		for (auto const &a :in)
			_buffer.push_back(a);
	}

	std::pair<iterator, bool> insert(const key_type &k)
	{
		iterator it = _buffer.lower_bound(k, _buffer.begin(), _buffer.end());

		const bool existed = (*it == k);
		if (!existed)
			_buffer.insert(it, k);

		return std::make_pair(it, existed);
	}

	iterator begin() { return _buffer.begin(); }
	const_iterator begin() const { return _buffer.begin(); }

	iterator end() { return _buffer.end(); }
	const_iterator end() const { return &_buffer.end(); }

	value_type *data() { return _buffer; }

	std::size_t size() const { return _buffer.size(); }
	std::size_t max_size() const { return _buffer.max_size(); }

	iterator lower_bound(const key_type &k)
	{
		return _buffer.lower_bound(k, _buffer.begin(), _buffer.end());
	}

private:
	omp_static_buffer <_Key, _Key, _Alloc> _buffer;

};

#endif //OMPSS_STATIC_SET
