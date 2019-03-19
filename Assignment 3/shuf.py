#!/usr/bin/python

"""
Write a random permutation of the input lines to standard output.

Copyright 2019 Junhong Wang
"""

import random, sys
from optparse import OptionParser

class Shuf:
    def __init__(self):
        version_msg = "%prog 2.0"
        usage_msg = """%prog [OPTION]... FILE
   or: %prog -i LO-HI [OPTION]...
Write a random permutation of the input lines to standard output.

With no FILE, or when FILE is -, read standard input."""

        self.help_msg = "\nTry '{0} --help' for more information.".format(sys.argv[0])

        self.parser = OptionParser(version=version_msg, usage=usage_msg)

        self.parser.add_option("-i", "--input-range",
                          action="store", dest="input_range",
                          help="treat each number LO through HI as an input line")

        self.parser.add_option("-n", "--head-count",
                          action="store", dest="head_count",
                          help="output at most COUNT lines")

        self.parser.add_option("-r", "--repeat",
                          action="store_true", dest="repeat",
                          help="output lines can be repeated")

    def parse_arguments(self, arguments):
        """
        If same options are provided,
        it will be overridden by later option.
        """
        options, args = self.parser.parse_args(arguments)

        return options, args

    def get_lines(self, options, args):
        if options.input_range is not None:
            # read from input_range
            if len(args) != 0:
                sys.exit("{0}: extra operand '{1}' {2}".format(sys.argv[0], args[0], self.help_msg))
            try:
                lo, hi = options.input_range.split("-")
                assert(int(lo) <= int(hi))
                nums = range(int(lo), int(hi) + 1)
                lines = []
                for num in nums:
                    lines.append(str(num) + '\n')
            except:
                sys.exit("{0}: invalid input range '{1}'".format(sys.argv[0], options.input_range))
        else:
            # read from file or stdin                                                                                                                                               
            if len(args) > 1:
                sys.exit("{0}: extra operand '{1}' {2}".format(sys.argv[0], args[1], self.help_msg))
            if len(args) == 0 or args[0] == "-":
                # read from stdin                                                                                                                                                
                lines = sys.stdin.readlines()
            else:
                # read from file
                try:
                    input_file = args[0]
                    f = open(input_file, 'r')
                    lines = f.readlines()
                    f.close()
                except IOError as err:
                    sys.exit("{0}: I/O error({1}) {2} {3}".format(sys.argv[0], err.errno, args[0], err.strerror))

        return lines

    def write(self, lines, options):
        # shuffle the content
        random.shuffle(lines)
        
        # check head count
        if options.head_count is not None:
            try:
                head_count=int(options.head_count)
                assert(head_count >= 0)
            except:
                sys.exit("{0}: invalid head count '{1}'".format(sys.argv[0], options.head_count))

        if options.repeat is None:
            # no repeat
            num_iterations = len(lines)
            if options.head_count is not None and head_count < num_iterations:
                num_iterations = head_count
            for index in range(0, num_iterations):
                sys.stdout.write(str(lines[index]))
        else:
            # repeat
            if len(lines) == 0:
                sys.exit("{0}: no lines to repeat".format(sys.argv[0]))

            if options.head_count is not None:
                # repeat with head count
                for _ in range(0, head_count):
                    sys.stdout.write(str(random.choice(lines)))
            else:
                # repeat forever
                while (True):
                    sys.stdout.write(str(random.choice(lines)))


def main():
    shuf = Shuf()

    options, args = shuf.parse_arguments(sys.argv[1:])

    lines = shuf.get_lines(options, args)

    shuf.write(lines, options)
    
            
if __name__ == "__main__":
    main()