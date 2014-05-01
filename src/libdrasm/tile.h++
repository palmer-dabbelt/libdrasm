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

        /* The storage elements associated with a tile. */
        std::vector<memword> _data;
        std::vector<regval> _regs;

        /* This stores a mapping from a cycle to the instruction that
         * will be executed at that cycle. */
        std::map<size_t, std::shared_ptr<instruction>> _instructions;
        ssize_t _last_inst_cycle;

        /* This stores a mapping from a cycle to the transmission that
         * will be executed at that cycle. */
        std::map<size_t, std::shared_ptr<instruction>> _transmissions;
        ssize_t _last_trans_cycle;

    public:
        /* Creates a new Tile, given a machine configuration. */
        tile(const std::shared_ptr<machine>& machine);

        /* This allows for access to arbitrary machine configuration
         * parameters. */
        std::shared_ptr<machine> m(void) const { return _machine.lock(); }

        /* Returns the first cycle that an instruction can be placed
         * at, or -1 if this tile is full. */
        ssize_t find_free_instruction(ssize_t start = 0) const;
        void use_instruction(ssize_t i);

        /* Returns the first cycle that a transmission can be placed
         * at, or -1 if the tile is full. */
        ssize_t find_free_transmission(ssize_t start = 0) const;
        void use_transmission(ssize_t i);

        /* Returns the first memory location that a single word (or an
         * array) can be placed at, or -1 if there's no memory space
         * left. */
        ssize_t find_free_word(void) const { return find_free_array(1); }
        ssize_t find_free_array(size_t size) const;

        /* Returns the first register that's free to be used. */
        ssize_t find_free_register(void) const;

        /* Takes up a memory word or an array */
        void use_word(size_t offset) { return use_array(offset, 1); }
        void use_array(size_t offset, size_t size);

        /* Takes up a memory word. */
        void use_register(ssize_t i);

        /* Returns the cycle at which the last used instruction will
         * execute. */
        ssize_t last_used_instruction(void) const;
    };
}

#endif
