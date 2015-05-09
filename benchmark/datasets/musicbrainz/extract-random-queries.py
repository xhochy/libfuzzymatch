#!/usr/bin/env python
"""
Script to extract and then generate random queries for fuzzy searching.

Usage:
    ./extract-random-queries.py <infile> <outfile>
"""

import codecs
import os
from random import choice, randint, random
import string
from subprocess import call
import sys
from tempfile import mkstemp

__author__ = "Uwe L. Korn"
__license__ = "MIT"


input_file = sys.argv[1]
output_file = sys.argv[2]

# Randomly select 1000 lines from the input file and store them temporarily.
temp_f, temp_file = mkstemp()
call(['shuf', '-n', '1000', input_file, '-o', temp_file])

# Modifiy these queries so that they have a non-zero edit distance.
with codecs.open(temp_file, 'r', 'utf-8') as f:
    with codecs.open(output_file, 'w', 'utf-8') as out:
        for line in f.readlines():
            if random() > 0.75:
                pos = randint(0, len(line) - 2)
                line = line[0:pos] + choice(string.ascii_lowercase) + line[pos + 1:]
            if random() > 0.25:
                pos = randint(0, len(line) - 2)
                line = line[0:pos] + choice(string.ascii_lowercase) + line[pos + 1:]
            out.write(line)

# Remove the temporary file again.
os.unlink(temp_file)
