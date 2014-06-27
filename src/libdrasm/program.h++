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

#ifndef LIBDRASM__PROGRAM_HXX
#define LIBDRASM__PROGRAM_HXX

namespace libdrasm {
    class program;
}

#include <ctype.h>
#include <memory>
#include <map>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include "instruction.h++"
#include "machine.h++"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

namespace libdrasm {
    /* Represents a single program executing on DREAMER, which
     * consists of a number of instructions executing on each tile
     * along with the associated debugging information. */
    template<class instruction_type,
             class machine_type>
    class program_t {
        typedef program_t<instruction_type, machine_type> program;
        typedef std::shared_ptr<program> program_ptr;
        typedef std::shared_ptr<instruction_type> instruction_ptr;
        typedef std::shared_ptr<machine_type> machine_ptr;

    private:
        /* Holds every instruction in the program. */
        std::vector<instruction_ptr> _instructions;

    public:
        /* Creates a program from a list of instructions.  Note that
         * these instructions will all be executed in order. */
        program_t(std::vector<instruction_ptr> instructions)
            : _instructions(instructions)
            {
            }

    public:
        /* Takes the path to a HEX file (which is as close to an
         * object file as we have here) and parses it, producing a
         * valid program. */
        static program_ptr parse_object(const std::string path)
            {
                auto file = fopen(path.c_str(), "r");
                if (file == NULL) {
                    perror("Unable to open file");
                    fprintf(stderr, "  path: '%s'\n", path.c_str());
                    abort();
                }

                char line[BUFFER_SIZE];

                /* This only supports version 5 of the HEX format. */
                if (fgets(line, BUFFER_SIZE, file) == NULL) {
                    perror("Unable to read first line");
                    abort();
                }

                if (strcmp(line, "VERSION 5\n") != 0) {
                    fprintf(stderr, "Expected 'VERSION 5', get '%s'\n", line);
                    abort();
                }

                /* We can't rely on the debug info headers to be in
                 * any sane order, so instead all the debug info needs
                 * to be stored as strings into memory so it can be
                 * parsed later.  Luckily the format here is somewhat
                 * consistant... :). */
                std::multimap<std::string, std::string> debug_info;
                machine_ptr mach = NULL;

                while (fgets(line, BUFFER_SIZE, file) != NULL) {
                    str_chomp(line);

                    fprintf(stderr, "line: '%s'\n", line);

                    /* These sorts of lines list the instructions on a
                     * tile and make up the actual meat of the
                     * file. */
                    int x, y, count;
                    if (sscanf(line, "TILE @ (%d, %d) NUM_LO_INSTS = %d",
                               &x, &y, &count) == 3) {
                        if (mach == NULL) {
                            fprintf(stderr, "TILE before DIMS\n");
                            abort();
                        }

                        for (int i = 0; i < count; ++i) {
                            if (fgets(line, BUFFER_SIZE, file) == NULL) {
                                fprintf(stderr, "File terminated early\n");
                                abort();
                            }

                            str_chomp(line);
                            auto tile = mach->tile(x, y);
                            if (tile == NULL) {
                                fprintf(stderr, "Unable to find tile\n");
                                abort();
                            }

                            auto ins = std::make_shared<instruction_type>(line);
                            tile->insert_at_pc(ins, i);
                        }

                        continue;
                    }

                    /* There's a special line that describes the size
                     * of the array. */
                    if (sscanf(line, "DIMS (%d, %d)", &x, &y) == 2) {
                        mach = std::make_shared<machine>(x, y);
                        continue;
                    }

                    /* Otherwise we're within the debug info and at
                     * this point require a header file. */
                    char type[BUFFER_SIZE];
                    if (sscanf(line, "%s = %d", type, &count) != 2) {
                        fprintf(stderr, "Malformed HEX file\n");
                        fprintf(stderr, "  Expected section header\n");
                        fprintf(stderr, "  Got '%s'\n", line);
                        abort();
                    }

                    for (int i = 0; i < count; ++i) {
                        if (fgets(line, BUFFER_SIZE, file) == NULL) {
                            fprintf(stderr, "File terminated early\n");
                            abort();
                        }

                        str_chomp(line);
                        debug_info.insert(std::make_pair(type,line));
                    }
                }

                std::vector<instruction_ptr> instructions;
                return std::make_shared<program>(instructions);
            }

    private:
        static void str_chomp(char *line)
            {
                while ((strlen(line) > 0) && isspace(line[strlen(line)-1]))
                    line[strlen(line)-1] = '\0';
            }
    };

    /* The default sort of program, which is probably what you want to
     * use if you're not trying to do anything special at all. */
    class program: public program_t<instruction, machine> {
    };
    extern template class program_t<instruction, machine>;
}

#endif
