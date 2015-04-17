#!/usr/bin/env python
"""
Script to extract the artist names from a MusicBrainz database dump.

Usage:
    ./extract-from-dbdump.py <dump_dir>/artist <outfile>
"""

import pandas as pd
import sys

__author__ = "Uwe L. Korn"
__license__ = "MIT"


input_file = sys.argv[1]
output_file = sys.argv[2]

df = pd.read_csv(input_file, sep='\t', header=None)
df.ix[:, 2].to_csv(output_file, index=False)
