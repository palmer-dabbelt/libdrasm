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

#include <libdrasm/program.h++>
#include <stdio.h>
#include <string>
#include <string.h>
#include "version.h"

int main(int argc, const char **argv)
{
    if ((argc == 1) || (strcmp(argv[1], "--help") == 0)) {
        printf("dreamer-objdump <*.hex>: A DREAMER objdump replacement\n");
        printf("  --help:    Print this help text\n");
        printf("  --version: Print the version number\n");
        printf("  -d:        Disassemble\n");
        return (argc == 1) ? 1 : 0;
    }

    if ((argc == 2) && (strcmp(argv[1], "--version") == 0)) {
        printf("dreamer-objdump %s\n", PCONFIGURE_VERSION);
        return 0;
    }

    auto obj = libdrasm::program::parse_object(argv[1]);

    return 0;
}
