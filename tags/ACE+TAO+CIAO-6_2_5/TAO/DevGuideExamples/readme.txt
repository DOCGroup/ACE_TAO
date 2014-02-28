$Id$

"These examples were developed by and are included in this
distribution with the permission of Object Computing, Inc. (OCI)
for the purpose of illustrating the usage of certain features of
The ACE ORB (TAO) as described in OCI's TAO Developer's Guide.
They are provided as is with no warranty, expressed or implied.
Though the examples are believed to illustrate correct usage of
TAO, OCI does not actively maintain them in this distribution and
does not guarantee their accuracy or their suitability for any
particular use.  OCI does provide additional distributions of TAO
and provides commercial support for TAO.  Visit
http://www.theaceorb.com or contact sales@ociweb.com for more
information on OCI's business model with respect to TAO."

--MPC--

Generate GNU Makefiles

$ACE_ROOT/bin/mwc.pl -type gnuace DevGuideExamples.mwc

Generate Visual C++ 7.1 Workspace and Projects
%ACE_ROOT%\bin\mwc.pl -type vc71 DevGuideExamples.mwc

Generate Visual C++ 8 Workspace and Projects
%ACE_ROOT%\bin\mwc.pl -type vc8 DevGuideExamples.mwc


--Windows--
* To run the release version of a test specify the following:
  run_test.pl -ExeSubDir Release
* The BiDirectionalGIOP projects can be referenced as an example of how to use
  Automatic Precompiled Headers
* The GettingStarted projects can be referenced as an example of how to use
  Manual Precompiled Headers

