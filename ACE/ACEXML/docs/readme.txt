/**  -*- HTML-Helper -*- $Id$

$Id$

@mainpage ACEXML - A Small and Portable XML Parser

@section acexml_synopsis Synopsis
ACEXML is a small footprint and portable framework for integrating XML
parsing ability into user applications.  The framework is based on the
 <a href="http://www.saxproject.org/"> Simple API for XML
(SAX 2.0) </A> by David Megginson.  A simple non-conformant XML parser
is also included in the framework.  Since our original motivation was
to use the parser for specifying software composition and
configuration descriptors, at the current stage, the parser does not
fully support all the features specified in the XML specification.

We do, however, plan to add more features to the parser and will
either release a more versatile parser or allow the current parser to
dynamically load in the extra features in the future.

@section acexml_features Features of ACEXML
<ul>
  <li> ACEXML only recognize UNICODE documents, although they can be
  in various different encoding, such as UTF-8, UTF-16, or UCS-4.
  Therefore, it might be a problem to handle document containing
  multi-byte charatersets.  They can, however, be translated into
  UNICODE before being parsed by ACEXML parser.
</ul>


@section acexml_others  Other Topics
<ol>
  <li>@ref acexml_parser_features
  <li>@ref acexml_guides
  <li>@ref acexml_bugs
  <li>@ref acexml_todo
</ol>

*/
