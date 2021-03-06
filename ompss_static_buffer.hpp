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

	// typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template
	// rebind<_Val>::other _Tp_alloc_type;

	typedef typename std::allocator_traits<_Alloc>::template
	rebind_alloc<_Val> _Tp_alloc_type;

	// The key must be always the first element and of type _Key
	_Key KeyOfIterator(const _Val *it) const
	{
		return *(_Key*)(it);
	}

	_Key &KeyOfIterator(const _Val * it)
	{
		return *(_Key*)(it);
	}

	// This function returns the index position for key k if the key
	// is not in the array returns false, but sets index position for
	// the closest higher value to k.
	iterator lower_bound(const _Key &k)
	{
		return std::lower_bound(begin(), end(), k,
		                        [&](const _Val &a, const _Key &b) -> bool
					{
						return  KeyOfIterator(&a) < b;
					});
	}

	_Val &insert(_Val * const place, _Key keyvalue)
	{
		assert(KeyOfIterator(place) != keyvalue);

		if (_elements >= _max_elements)
			throw std::out_of_range("Static buffer");

		std::size_t nelems = end() - place;
		if (nelems > 0) // Element in the middle (or first)
			memmove((void*) (place + 1), (void *) place,
			        sizeof(_Val) * nelems);

		++_elements;
		KeyOfIterator(place) = keyvalue;
		return *place;
	}

	void push_back(const _Val &in)
	{
		if (_elements >= _max_elements)
			throw std::out_of_range("Static buffer");

		_Val * const ptr = end();
		memcpy((void*) ptr, (void*) &in, sizeof(_Val));
		++_elements;
	}

	void clear()
	{
		_elements = 0;
	}

	void reserve(std::size_t n)
	{
		// Reallocation is not supported. So only empty buffers can be reserved
		assert(_buffer == nullptr);
		assert(_elements == 0);
		assert(_max_elements == 0);

		_buffer = _alloc.allocate(n);
		_max_elements = n;
	}

	// Constructor
	omp_static_buffer(const _Alloc &__a = _Alloc()):
		_elements(0), _max_elements(0),
		_alloc(__a), _buffer(nullptr)
	{}

	template<typename Tarray>
	omp_static_buffer(const _Alloc &__a = _Alloc()):
		_elements(0), _max_elements(0),
		_alloc(__a), _buffer(nullptr)
	{}

	template<typename Tarray>
	omp_static_buffer(const Tarray &in,
	                  const _Alloc &__a = _Alloc()):
		_elements(0), _max_elements(0),
		_alloc(__a), _buffer(nullptr)
	{
		copy(in);
	}

	template<typename Tarray>
	void copy(const Tarray &in)
	{
		const std::size_t in_size = in.size();

		assert(_buffer == nullptr || max_size() >= in.size());

		if (_buffer == nullptr)
			reserve(in_size + 10); // TODO: this 10 is totally arbitrary now
		else if (max_size() >= in.size())
			clear();
		else
			throw std::out_of_range("Static buffer");

		for (auto const &a :in)
			push_back(a);
	}

	~omp_static_buffer()
	{
		assert(_buffer != nullptr);
		_alloc.deallocate(_buffer, _max_elements);
		_elements = 0;
		_max_elements = 0;

	}

	const _Val *begin() const { return &_buffer[0]; }
	_Val *begin() { return &_buffer[0]; }

	const _Val *end() const { return &_buffer[_elements]; }
	_Val *end() { return &_buffer[_elements]; }

	const _Val *data() const { return _buffer; }
	_Val *data() { return _buffer; }

	bool empty() const { return (_elements == 0); }

	_Val *find(const _Key &key) const
	{
		_Val *tmp = lower_bound(key);
		return KeyOfIterator(tmp) == key ? tmp : end();
	}

	_Val *find(const _Key &key)
	{
		_Val *tmp = lower_bound(key);
		return KeyOfIterator(tmp) == key ? tmp : end();
	}

	std::size_t size() const { return _elements; }
	std::size_t max_size() const { return _max_elements; }

protected:
	std::size_t _elements;
	std::size_t _max_elements;
	_Tp_alloc_type _alloc;
	_Val *_buffer;
};

#endif //OMPSS_STATIC_BUFFER
