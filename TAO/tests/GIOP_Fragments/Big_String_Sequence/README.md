# TAO GIOP Fragmentation Bug
## Reproduces a bug when GIOP fragmentation is used 

The server returns a string sequence with two elements, the first is a
long 4MB string with repeating character 'A', the second element is
the string "Hello World". If one removes the command line parameters
`-ORBMaxMessageSize 1048576` from `run_test.pl` everything
works as expected, but with these settings, which cause GIOP
fragmentation, the client receives an empty string as the second
element of the sequence.

We discovered problems in a more complicated application when big
sequences are returned, often the client encountering
`CORBA::MARSHAL` or `CORBA::COMM_FAILURE` exceptions. In one
case, it worked if the sequence contained a single element, even if
big, but a small second element caused client exceptions. The complex
application encountered problems with big sequences even without the
`-ORBMaxMessageSize` parameter, but problems start at smaller
sizes if it is used.