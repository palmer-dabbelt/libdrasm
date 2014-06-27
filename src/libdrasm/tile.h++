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

#ifndef LIBDRASM__TILE_HXX
#define LIBDRASM__TILE_HXX

namespace libdrasm {
    class tile;
}

#include <map>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "instruction.h++"

namespace libdrasm {
    /* Holds the contents of a single tile. */
    template <class instruction_type>
    class tile_t {
        typedef std::shared_ptr<instruction_type> instruction_ptr;

    private:
        /* Maps a PC to the instruction that will execute when that PC
         * is reached. */
        std::map<size_t, instruction_ptr> _pc;

    public:
        tile_t(void)
            : _pc()
            {
            }

    public:
        void insert_at_pc(const instruction_ptr& inst, size_t pc)
            {
                auto l = _pc.find(pc);
                if (l != _pc.end()) {
                    fprintf(stderr, "Overwrote PC\n");
                    abort();
                }

                _pc[pc] = inst;
            }
    };

    class tile: public tile_t<instruction> {
    public:
        tile(void)
            : tile_t<instruction>()
            {
            }
    };
}

#endif
