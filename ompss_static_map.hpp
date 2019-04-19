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
#include <utility>
#include <cstddef>
#include <cstring>

/*
 *  This is the simplest implementation for a fix sized map with
 *  templates, keeping the log complexity to search and compatible with
 *  ompss without using allocation hacks.
 */
template<typename S>
class ompss_static_map_base {
public:

	typedef S* iterator;
	typedef decltype(S::first) I;
	typedef decltype(S::second) N;

	ompss_static_map_base(std::size_t max_elements):
		_elements(0),
		_max_elements(max_elements)
	{}

	ompss_static_map_base(const std::map<I, N> &in):
		_elements(in.size()),
		_max_elements(_elements + 10), // This is kind of arbitrary
		_buffer(new S[_max_elements]())
	{
		size_t idx = 0;
		for (auto &it : in)
			_buffer[idx++] = it;

		assert(_elements == idx);
	}

	N &operator[] (const I &k)
	{
		std::size_t it;

		const int pos = find_pos(k, it);

		if (pos >= 0)  // element exist
			return _buffer[pos].second;

		if (_elements >= _max_elements)
			throw std::out_of_range("Static map");
;
		std::size_t nelems = _elements - it;
		if (nelems > 0) // Element in the middle (or first)
			memmove(&_buffer[it + 1], &_buffer[it], sizeof(S) * nelems);

		_buffer[it] = {k, N()};
		++_elements;
		return _buffer[it].second;

	}

	std::size_t size() { return _elements; }
	std::size_t max_size() { return _max_elements; }

	S *begin() { return _buffer; }
	S *end() { return &_buffer[_elements]; }

	S *data() { return _buffer; }

protected:
	std::size_t _elements;
	const std::size_t _max_elements;
	S *_buffer;

	// This function returns the index position for key k if the key
	// is not in the array returns -1, but sets index position for
	// the closest higher value to k.
	int find_pos(const I &k, std::size_t &it) const
	{
		I min = 0, max = _elements;

		it = _elements;
		if (k > _buffer[it - 1].first)
			return -1;
		it = 0;
		if (k < _buffer[it].first)
			return -1;

		while (min < max) {
			it = (min + max) / 2;

			if (k == _buffer[it].first)
				return it;

			if (k >_buffer[it].first)
				min = it + 1;
			else
				max = it;
		}
		it += (_buffer[it].first < k);
		return -1;
	}
};

// Set

template <typename I>
union _single_t {
	I first, second;
};

template <typename I>
using ompss_static_set = ompss_static_map_base<_single_t<I>>;

// Map
template <typename I, typename N>
using ompss_static_map = ompss_static_map_base<std::pair<I, N>>;

#endif //OMPSS_STATIC_MAP
