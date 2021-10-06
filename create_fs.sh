#!/bin/bash
./memfs.py -o sample -d sample -f sample/images/img02.jpg -f sample/images/img03.jpg -d sample/images
ld -r -b binary -o sample_data.o sample_data.bin
