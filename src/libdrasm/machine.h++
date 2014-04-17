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

#ifndef LIBDRASM__MACHINE_HXX
#define LIBDRASM__MACHINE_HXX

#include <libocn/network.h++>
#include <libocn/mesh_network.h++>
#include <stddef.h>

namespace libdrasm {
    /* Holds the configuration of a machine, which contains what sort
     * of operations it has, the number of registers, etc.  This is
     * split into two classes: "machine" holds generic functionality,
     * and "machine_t" holds information about the network.  The
     * general idea is to avoid a bunch of recursive templates
     * here. */
    class machine {
    public:
        /* Returns the number of registers that a machine has. */
        size_t register_width(void) const { return 32; }
        size_t register_count(void) const { return 65536; }

        /* Returns the machine's code memory size: width in bits and
         * depth in words. */
        size_t code_mem_width(void) const { return 32; }
        size_t code_mem_depth(void) const { return 65536; }

        /* Returns the machine's data memory size: width in bits and
         * depth in words. */
        size_t data_mem_width(void) const { return 32; }
        size_t data_mem_depth(void) const { return 65536; }
    };

    template<class node_t>
    class machine_t: public machine {
    private:
        std::shared_ptr<libocn::network<node_t>> _network;
        typedef std::shared_ptr<node_t> node_ptr;

    public:
        machine_t(size_t w, size_t h,
                  std::function<node_ptr(size_t, size_t)> f)
            : machine(),
              _network(std::make_shared<libocn::mesh_network<node_t>>(w, h, f))
            {
            }

        /* Returns an on-chip network that paramaterizes the chip. */
        std::shared_ptr<libocn::network<node_t>> network(void) const
            { return _network; }
    };
}

#endif
