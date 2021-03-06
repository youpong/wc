#!/bin/bash

LANG=C

function error() {
    echo "$@" >&2
    exit 1
}

# stdin
cmp <(cat wc.c|./wc) <(cat wc.c|wc) || error "Error: $LINENO"

# single file
cmp <(./wc wc.c) <(wc wc.c) || error "Error: $LINENO"

# multiple file
cmp <(./wc wc.c Makefile) <(wc wc.c Makefile) || error "Error: $LINENO"

# catching error 
./wc '' 2>/dev/null && error "Error: $LINENO: file not found"
./wc .  2>/dev/null && error "Error: $LINENO: directory"

echo "Ok." 
