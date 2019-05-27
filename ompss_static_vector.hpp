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

#ifndef OMPSS_STATIC_VECTOR
#define OMPSS_STATIC_VECTOR


#include <vector>
#include <algorithm>

#include "ompss_static_buffer.hpp"

/*
 *  This is the simplest implementation for a fix sized map with
 *  templates, keeping the log complexity to search and compatible with
 *  ompss without using allocation hacks.
 */
template <typename _Tp,
          typename _Alloc = std::allocator<_Tp>>
class ompss_static_vector
{
public:
	typedef _Tp     value_type;
	typedef _Alloc   allocator_type;

	// Iterator
	typedef  value_type *iterator;
	typedef const value_type *const_iterator;

	ompss_static_vector(std::vector<value_type> &in,
	                    const allocator_type& __a = allocator_type()) :
		_buffer(in, __a)
	{}

	// The = operator may be used only with empty containers.
	ompss_static_vector &operator=(const std::vector<value_type> &in)
	{
		_buffer.copy(in);
		return *this;
	}

	void push_back(const value_type &in)
	{
		_buffer.push_back(in);
	}

	value_type &operator[](size_t index)
	{
		return _buffer.data()[index];
	}

	iterator begin() { return _buffer.begin(); }
	const_iterator begin() const { return _buffer.begin(); }

	iterator end() { return _buffer.end(); }
	const_iterator end() const { return &_buffer.end(); }

	value_type *data() { return _buffer; }

	bool empty() const { return _buffer.empty(); }

	void clear() { _buffer.clear(); }

	void reserve(std::size_t new_cap) { _buffer.reserve(new_cap); }

	std::size_t size() const { return _buffer.size(); }
	std::size_t max_size() const { return _buffer.max_size(); }

private:
	omp_static_buffer <_Tp, _Tp, _Alloc> _buffer;

};

#endif //OMPSS_STATIC_VECTOR
