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

#ifndef OMPSS_ALLOCATOR_HPP
#define OMPSS_ALLOCATOR_HPP

#include <cstddef>
#include <cstring>
#include <cassert>
#include <cstdlib>

template<typename _Tp>
class ompss_allocator {
public:
	typedef std::ptrdiff_t   difference_type;
	typedef std::size_t      size_type;
	typedef _Tp              value_type;

	ompss_allocator() = default;

	_Tp *allocate(std::size_t n)
	{
		if (n > std::size_t(-1) / sizeof(_Tp))
			throw std::bad_alloc();

		_Tp * const p = static_cast<_Tp*>(std::malloc(n * sizeof(_Tp)));
		if (!p)
			throw std::bad_alloc();

		return p;
	 }

	void deallocate(_Tp* p, std::size_t n) noexcept
	{
		(void *) n;
		std::free(p);
	}
};

#endif // OMPSS_ALLOCATOR_HPP
