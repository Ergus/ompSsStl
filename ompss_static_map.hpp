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

#ifndef OMPSS_STATIC_MAP
#define OMPSS_STATIC_MAP


#include <map>
#include <algorithm>
#include <sstream>

#include "ompss_static_buffer.hpp"

/*
 *  This is the simplest implementation for a fix sized map with
 *  templates, keeping the log complexity to search and compatible with
 *  ompss without using allocation hacks.
 */
template <typename _Key,
          typename _Tp,
          typename _Alloc = std::allocator<std::pair<const _Key, _Tp>>>
class ompss_static_map
{
public:
	typedef _Key     key_type;
	typedef _Tp      mapped_type;
	typedef std::pair<const _Key, _Tp>  value_type;
	typedef _Alloc   allocator_type;

	// Iterator
	typedef  value_type *iterator;
	typedef const value_type *const_iterator;

	ompss_static_map(const std::map<key_type, mapped_type> &in) :
		_buffer(in)  // TODO: This 10 is completely arbitrary now.
	{}

	ompss_static_map &operator=(const std::map<key_type, mapped_type> &in)
	{
		_buffer.copy(in);
		return *this;
	}

	mapped_type &operator[] (const key_type &k)
	{
		iterator it = _buffer.lower_bound(k, begin(), end());

		if (it->first != k)
			_buffer.insert(it, k).second = mapped_type();

		return it->second;
	}

	iterator begin() { return _buffer.begin(); }
	const_iterator begin() const { return _buffer.begin(); }

	iterator end() { return _buffer.end(); }
	const_iterator end() const { return &_buffer.end(); }

	mapped_type *data() { return _buffer; }

	bool empty() const { return _buffer.empty(); }

	void clear() { _buffer.clear(); }

	std::size_t size() const { return _buffer.size(); }
	std::size_t max_size() const { return _buffer.max_size(); }

	const_iterator lower_bound(const key_type &k) const
	{
		return _buffer.lower_bound(k, _buffer.begin(), _buffer.end());
	}

	iterator lower_bound(const key_type &k)
	{
		return _buffer.lower_bound(k, _buffer.begin(), _buffer.end());
	}

private:
	omp_static_buffer <_Key, std::pair<const _Key, _Tp>, _Alloc> _buffer;
};

template <typename _Key, typename _Tp>
std::string pairToStr(const std::pair<_Key, _Tp> &in)
{
	std::ostringstream ss;
	ss << "<" << in.first << ";" << in.second << ">";
	return ss.str();
}

template <typename _Key, typename _Tp>
std::ostream& operator<< (std::ostream& os, const std::pair<_Key, _Tp> &in)
{
	os << pairToStr(in);
	return os;
}

#endif //OMPSS_STATIC_MAP
