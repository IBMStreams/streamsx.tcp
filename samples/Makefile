# Licensed Materials - Property of IBM
# ObjectStorage Toolkit for IBM Streams
# (c) Copyright IBM Corp. 2020
# All rights reserved.                              
.PHONY: all clean

all: 
	for dir in $(shell ls -d -1 */); do\
		make -C $$dir || exit 1; \
	done

clean:
	for dir in $(shell ls -d -1 */); do\
		make -C $$dir clean || exit 1; \
	done

