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

#include "machine.h++"
#include "memword.h++"
#include "instruction.h++"
#include "regval.h++"
#include "transmission.h++"
#include <vector>

namespace libdrasm {
    class tile {
        /* This is a weak pointer */
        std::weak_ptr<machine> _machine;

        /* The code memory is split up into two parts: "instructions"
         * (things that touch the ALU) and "transmissions" (things
         * that touch the network).  Both operations can be performed
         * somewhat independently in a single cycle, which is why
         * they're split like this. */
        std::vector<instruction> _instructions;
        std::vector<transmission> _transmissions;
        std::vector<memword> _data;
        std::vector<regval> _regs;

    public:
        /* Creates a new Tile, given a machine configuration. */
        tile(const std::shared_ptr<machine>& machine);

        /* Returns the first cycle that an instruction can be placed
         * at, or -1 if this tile is full. */
        ssize_t find_free_instruction(void) const;

        /* Returns the first memory location that a single word can be
         * placed at, or -1 if there's no memory space left. */
        ssize_t find_free_word(void) const;

        /* Returns the first register that's free to be used. */
        ssize_t find_free_register(void) const;

        /* Takes up an instruction slot. */
        void use_instruction(ssize_t i);

        /* Takes up a memory word. */
        void use_word(ssize_t i);

        /* Takes up a memory word. */
        void use_register(ssize_t i);
    };
}

#endif
