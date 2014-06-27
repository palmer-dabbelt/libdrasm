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

namespace libdrasm {
    class machine;
}

#include <map>
#include <memory>
#include "tile.h++"

namespace libdrasm {
    /* Represents the configuration of a DREAMER machine.  This should
     * contain enough information to paramaterize the RTL to actually
     * generate an instance of DREAMER. */
    template<class tile_type>
    class machine_t {
        typedef std::shared_ptr<tile_type> tile_ptr;

    private:
        size_t _x, _y;
        std::map<std::pair<size_t, size_t>, tile_ptr> _tiles_xy;

    public:
        machine_t(size_t X, size_t Y)
            : _x(X),
              _y(Y)
            {
                /* FIXME: DREAMER's network appears to have this bit
                 * of oddness still builtin.  I should really be using
                 * libocn here in order to build up the networks, but
                 * I'm a bit too lazy tonight... */
                for (size_t x = 1; x <= X; ++x) {
                    for (size_t y = 0; y < Y; ++y) {
                        auto tile = std::make_shared<tile_type>();
                        _tiles_xy[std::make_pair(x, y)] = tile;
                    }
                }
            }

    public:
        tile_ptr tile(size_t x, size_t y)
            {
                auto l = _tiles_xy.find(std::make_pair(x, y));
                if (l == _tiles_xy.end())
                    return NULL;
                return l->second;
            }
    };

    class machine: public machine_t<tile> {
    public:
        machine(size_t x, size_t y)
            : machine_t<typename libdrasm::tile>(x, y)
            {
            }
    };
    extern template class machine_t<tile>;
}

#endif
