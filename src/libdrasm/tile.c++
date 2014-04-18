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

#include "tile.h++"
using namespace libdrasm;

tile::tile(const std::shared_ptr<machine>& machine)
    : _machine(machine),
      _instructions(machine->code_mem_depth(), instruction()),
      _transmissions(machine->code_mem_depth(), transmission()),
      _data(machine->data_mem_depth(), memword()),
      _regs(machine->register_count(), regval())
{
}

ssize_t tile::find_free_instruction(size_t start) const
{
    for (size_t i = start; i < _instructions.size(); ++i)
        if (_instructions[i].busy() == false)
            return i;

    return -1;
}

ssize_t tile::find_free_array(size_t size) const
{
    for (size_t i = 0; i < _data.size(); ++i) {
        if (i + size >= _data.size())
            return -1;

        bool any_busy = false;
        for (size_t j = 0; j < size; ++j)
            if (_data[i+j].busy() == true)
                any_busy = true;

        if (any_busy == false)
            return i;
    }

    return -1;
}

ssize_t tile::find_free_register(void) const
{
    for (size_t i = 0; i < _regs.size(); ++i)
        if (_regs[i].busy() == false)
            return i;

    return -1;
}

void tile::use_instruction(ssize_t i)
{
    _instructions[i].update(i);
}

void tile::use_array(size_t offset, size_t size)
{
    for (size_t i = offset; i < (offset + size); ++i)
        _data[i].update(i);
}

void tile::use_register(ssize_t i)
{
    _regs[i].update(i);
}
