#!/bin/bash

for file in `ls *.js`; do
    echo "Compressing: $file"
    gzip -kf "$file" && \
    echo "Converting $file to byte array"
    xxd -i -u $file".gz" && \
    echo "Deleting source file"
    rm -f "$file"
done
