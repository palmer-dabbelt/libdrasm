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

#ifndef LIBDRASM__INSTRUCTION_HXX
#define LIBDRASM__INSTRUCTION_HXX

namespace libdrasm {
    class instruction;
}

#include <memory>

namespace libdrasm {
    /* Stores a single DREAMER instruction, along with the circuit
     * nodes that are somehow coorelated with this instruction. */
    class instruction {
    public:
        /* Parses an instruction string, filling out all the required
         * information about that particular instuction. */
        /* FIXME: This should do _something_. */
        instruction(const std::string hex __attribute__((unused)))
            {
            }
    };
}

#endif
