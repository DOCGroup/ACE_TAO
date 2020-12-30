# TAO GIOP Fragmentation Bug
## Reproduces a bug when GIOP fragmentation is used 

The server returns a string sequence with two elements, the first is a
long 4kB string with repeating character 'A', the second element is
the string "Hello World". If one removes the command line parameters
`-ORBMaxMessageSize 1024` from `run_test.pl` everything
works as expected, but with these settings, which cause GIOP
fragmentation, the client receives an empty string as the second
element of the sequence.
