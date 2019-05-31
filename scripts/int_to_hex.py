#!/usr/bin/python

import sys

if len(sys.argv) != 2:
    print("Usage: int_to_hex INTEGER")
    sys.exit(0)

print(hex(int(sys.argv[1])))
