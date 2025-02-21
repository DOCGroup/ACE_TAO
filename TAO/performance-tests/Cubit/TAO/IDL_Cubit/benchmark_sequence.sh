#!/bin/sh


# Script to run the IDL_Cubit octet sequence test with different
# sequence sizes, using TAO's UIOP pluggable protocol.
# The -ORBGIOPlite option will be used.
# Client and Server output will be placed in separate files.

# Initially written by Ossama Othman <othman@cs.wustl.edu>

set -e

for bytes in 4 8 16 32 64 128 256 512 1024 2048; do
  echo "Running IDL_Cubit sequence<octet> test with $bytes byte(s) in sequence..."
  ./server -f ior -ORBEndpoint uiop:// -ORBGIOPlite \
	> octet_seq.$bytes.uioplite.server 2>&1 &
  sleep 2
  ./client -f ior -n 1000 -t small_octet_seq -b $bytes -x -ORBGIOPlite \
	> octet_seq.$bytes.uioplite.client 2>&1
  wait
done

echo "Done."

