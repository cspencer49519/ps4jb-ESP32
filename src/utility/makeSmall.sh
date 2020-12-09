#!/bin/bash

for file in `ls *.js`; do
    echo "Compressing: $file"
    gzip -kf "$file" && \
    echo "Converting $file to byte array"
    xxd -i -u $file".gz" && \
    echo "Deleting source JS file"
    rm -f "$file"
done

for file in `ls *.html`; do
    echo "Compressing: $file"
    gzip -kf "$file" && \
    echo "Deleting source HTML file"
    rm -f "$file"
done

for file in `ls *.bin`; do
    echo "Compressing: $file"
    gzip -kf "$file" && \
    echo "Deleting source BIN file"
    rm -f "$file"
done