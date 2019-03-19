# Make x86-64 random byte generators.

# Copyright 2015 Paul Eggert

# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

# Change the -O2 to -Og or -O0 to ease runtime debugging.
# -O2 is often better for compile-time diagnostics, though.
#OPTIMIZE = -O2

#CC = gcc
#CFLAGS = $(OPTIMIZE) -g3 -Wall -Wextra -march=native -mtune=native -mrdrnd

randmain: randmain.c randcpuid.c randlibhw.so randlibsw.so
	$(CC) $(CFLAGS) -ldl -Wl,-rpath=$PWD randmain.c randcpuid.c -o $@

randlibhw.so: randlibhw.o
	$(CC) $(CFLAGS) -shared -o randlibhw.so randlibhw.o

randlibsw.so: randlibsw.o
	$(CC) $(CFLAGS) -shared -o randlibsw.so randlibsw.o

randlibhw.o: randlibhw.c
	$(CC) $(CFLAGS) -fPIC -c randlibhw.c -o $@

randlibsw.o: randlibsw.c
	$(CC) $(CFLAGS) -fPIC -c randlibsw.c -o $@
