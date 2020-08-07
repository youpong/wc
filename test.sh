#!/bin/bash

function error() {
    echo "$@" >&2
    exit 1
}

cmp <(cat wc.c|./wc) <(cat wc.c|wc) || error "Error: $LINENO"
cmp <(./wc wc.c) <(wc wc.c) || error "Error: $LINENO"

echo "Ok." 
