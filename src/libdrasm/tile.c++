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
      _data(machine->data_mem_depth(), memword()),
      _regs(machine->register_count(), regval()),
      _instructions(),
      _last_inst_cycle(0)
{
}

ssize_t tile::find_free_instruction(ssize_t start) const
{
    /* If there's no space left on this tile then we can't allocate an
     * instruction. */
    if (_instructions.size() >= _machine.lock()->code_mem_depth())
        return -1;

    /* Otherwise we can allocate an instruction, so pass back the
     * cycle. */
    if (start > _last_inst_cycle)
        return start;
    return _last_inst_cycle + 1;
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
    const auto l = _instructions.find(i);
    if (l != _instructions.end()) {
        fprintf(stderr, "Attempted to re-use an instruction %ld!\n",
                (long)i);
        abort();
    }

    if (i <= _last_inst_cycle) {
        fprintf(stderr, "Attempted to use an instruction too early\n");
        abort();
    }

    _instructions[i] = NULL;
    _last_inst_cycle = i;
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

ssize_t tile::last_used_instruction(void) const
{
    return _last_inst_cycle;
}
