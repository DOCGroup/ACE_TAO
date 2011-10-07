$Id$

ThreeTier Test.

This test was added in response to the bugs reported in bugzilla entries
  2934: -ORBMuxedConnectionMax didn't work correctly, and
  2935: too many connections to a server are opened in nested-upcall or
        multithreaed situations.


The test consists of three processes, source sends requests,
middle passes the requests on, and sink actually processes the requests.
Because source operates as fast as possible and sink has a delay, the
nested upcall situation happens.  Before the problem was corrected there
were usually five or six connection attempts tried before the first one
had a chance to complete.

The run_test.pl program determines whether the test was successful by
"greppping" the log file produced by sink and counting the number of
peer connections that were logged.   There should be exactly one.

As an aside, these three processes have been very useful in testing various
middle tier issues.  Of course the best way to address this middle tier type
of application is thru a mixture of AMH and AMI, but that's a topic for a
different test.

Dale Wilson <wilsond@ociweb.com>