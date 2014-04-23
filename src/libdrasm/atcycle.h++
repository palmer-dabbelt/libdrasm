/*
 * Copyright (C) 2014 Palmer Dabbelt
 *   <palmer.dabbelt@eecs.berkeley.edu>
 *
 * This file is part of libdrasm.
 *
 * libdrasm is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * libdrasm is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libdrasm.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef LIBDRASM__ATCYCLE_HXX
#define LIBDRASM__ATCYCLE_HXX

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

namespace libdrasm {
    /* Here's a class that deals with somethat that may or may not
     * happen at a particular cycle. */
    class atcycle {
    private:
        bool _busy;
        size_t _cycle;

    public:
        atcycle(void): _busy(false) {}

        bool busy(void) const { return _busy; }

        void update(size_t cycle)
            {
                if (_busy == true) {
                    fprintf(stderr, "Updated an already used atcycle\n");
                    abort();
                }

                _cycle = cycle;
                _busy = true;
            }

        size_t cycle(void) const
            {
                if (_busy == false) {
                    fprintf(stderr, "Accessed cycle of invalid instruction\n");
                    abort();
                }

                return _cycle;
            }
    };
}

#endif
