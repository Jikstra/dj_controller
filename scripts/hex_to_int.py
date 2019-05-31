#!/usr/bin/python

import sys

if len(sys.argv) != 2:
    print("Usage: hex_to_int HEXSTRING")
    sys.exit(0)

print(int(sys.argv[1], 0))
