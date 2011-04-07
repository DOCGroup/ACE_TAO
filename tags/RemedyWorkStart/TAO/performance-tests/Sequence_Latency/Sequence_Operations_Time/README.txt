// $Id$

This test is used to validate the peformance of sequence copying.

The motivation of this test is that a comparison between DOC & OCI's
release of TAO versus a different ORB showed TAO had a performance
deficit. The performance was improved by using shallow copys for some
sequences, and this test can be used to measure the performance
improvements.

Output is written to stderr, and can be either easy to read text, or
CSV format for import into a spreadsheet.

To run the test, use the command line:

./test

for CSV:

./test -csv
