#!/bin/sh


# Script to run all of the IDL_Cubit type tests for IIOP, UIOP and IIOP/UIOP
# with the -ORBGIOPlite option.
# Client and Server output will be placed in separate files.

# Initially written by Ossama Othman <othman@cs.wustl.edu>

set -e

for protocol in iiop uiop; do
   for test in void octet short long struct union \
               small_octet_seq small_long_seq small_struct_seq \
               large_octet_seq large_long_seq large_struct_seq \
               mixin rti_data one_way; do
	echo "Running IDL_Cubit $test test for $protocol..."
	./server -f ior -ORBEndpoint $protocol:// \
		> $test.$protocol.server 2>&1 &
	sleep 2
	./client -f ior -n 1000 -t $test -x \
		> $test.$protocol.client 2>&1
	wait
	echo "Running IDL_Cubit $test for $protocol with -ORBGIOPlite..."
	./server -f ior -ORBEndpoint $protocol:// -ORBGIOPlite \
		> $test.$protocol.lite.server 2>&1 &
	sleep 2
	./client -f ior -n 1000 -t $test -x -ORBGIOPlite \
		> $test.$protocol.lite.client 2>&1
	wait
   done
done
echo "Done."

