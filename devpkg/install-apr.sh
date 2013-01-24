#!/bin/sh

set -e
# Go somewhere safe:
cd /tmp
# Get the source to base APR 1.4.6.
curl -L -O http://archive.apache.org/dist/apr/apr-1.4.6.tar.gz
# Extract it and go into the source
tar -xzvf apr-1.4.6.tar.gz
cd apr-1.4.6
# Configure, make, and install:
./configure
make
sudo make install
# Reset and clean up:
cd /tmp
rm -rf apr-1.4.6 apr-1.4.6.tar.gz

# And now the same with apr-util:
curl -L -O http://archive.apache.org/dist/apr/apr-util-1.4.1.tar.gz
tar -xzvf apr-util-1.4.1.gz
cd apr-util-1.4.1
# ./configure requires an extra parameter to make apr-util
# to find apr:
./configure --with-apr=/usr/local/apr
make
sudo make install
cd /tmp
rm -rf apr-util-1.4.1* apr-1.4.6*

