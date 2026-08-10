#!/bin/bash
cc test_facile.c -o test_facile 
cc test_moins_facile.c -o test_moins_facile
cc -m32 test_moins_facile.c -o test_moins_facile_32-bit

