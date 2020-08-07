#!/bin/bash

function error() {
    echo "$@" >&2
    exit 1
}

cmp <(cat wc.c|./wc) <(cat wc.c|wc) || error "$LINENO"
cmp <(./wc wc.c) <(wc wc.c) || error "$LINENO"

echo "Ok." 
