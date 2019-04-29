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


#ifndef OMPSS_STATIC_BUFFER
#define OMPSS_STATIC_BUFFER

#include <memory>
#include <utility>

#include <cstddef>
#include <cstring>
#include <cassert>

template <typename _Key, typename _Val,
          typename _Alloc = std::allocator<_Val> >
class omp_static_buffer {
public:
	typedef  _Val *iterator;
	typedef  const _Val *const_iterator;

	typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template
	rebind<_Val>::other _Tp_alloc_type;


	// The key must be always the first element and of type
	// _Key
	_Key KeyOfIterator(const _Val *it) const
	{
		return _Key()(*it);
	}

	_Key &KeyOfIterator(_Val * const it)
	{
		return *(_Key*)it;
	}

// This function returns the index position for key k if the key
// is not in the array returns false, but sets index position for
// the closest higher value to k.
	bool find_pos(const _Key &k, iterator &it,
	              const _Val *begin, const _Val *end)
	{
		size_t _min = 0, _max = end - begin;

		it = (_Val*) begin;
		_Key _it_key = KeyOfIterator(it);

		if (k < _it_key)
			return false;

		while (_min < _max) {
			const size_t i = (_min + _max) / 2;
			it = (_Val*) &begin[i];
			_it_key = KeyOfIterator(it);

			if (k == _it_key)
				return true;

			if (k > _it_key)
				_min = i + 1;
			else
				_max = i;
		}

		if (k > _it_key)
			++it;

		return false;
	}

	_Val &insert(_Val * const place, _Key keyvalue)
	{
		assert(KeyOfIterator(place) != keyvalue);

		if (_elements >= _max_elements)
			throw std::out_of_range("Static map");

		std::size_t nelems = end() - place;
		if (nelems > 0) // Element in the middle (or first)
			memmove((void*) (place + 1), (void *) place, sizeof(_Val) * nelems);

		++_elements;
		KeyOfIterator(place) = keyvalue;
		return *place;
	}

	void push_back(const _Val &in)
	{
		if (_elements >= _max_elements)
			throw std::out_of_range("Static map");

		_Val * const ptr = end();
		memcpy((void*) ptr, (void*) &in, sizeof(_Val));
		++_elements;
	}

	omp_static_buffer(std::size_t max_elements,
	                  const _Alloc &__a = _Alloc()):
		_elements(0),
		_max_elements(max_elements),
		_alloc(__a),
		_buffer(_alloc.allocate(max_elements))
	{}

	_Val *begin() const { return &_buffer[0]; }
	_Val *begin() { return &_buffer[0]; }

	_Val *end() const { return &_buffer[_elements]; }
	_Val *end() { return &_buffer[_elements]; }

	_Val *data() const { return _buffer; }

	std::size_t size() const { return _elements; }
	std::size_t max_size() const { return _max_elements; }

protected:
	std::size_t _elements;
	const std::size_t _max_elements;
	_Tp_alloc_type _alloc;
	_Val *_buffer;
};

#endif //OMPSS_STATIC_BUFFER
