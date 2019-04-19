# Copyright (C) 2019  Jimmy Aguilar Mena

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

FILES_IN := test_ompss_static_map.cpp
FILES_OUT := $(FILES_IN:.cpp=.x)

CXXFLAGS := ${CXXFLAGS}
CXXFLAGS += -g -ggdb -Wall -Wextra -pedantic

all : $(FILES_OUT)

test_%.x : test_%.cpp %.hpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean test

clean:
	rm -rf *.x

test: $(FILES_OUT)
	for executable in $<; do \
		./$$executable ;   \
	done
