/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps/Web
//
// = FILENAME
//    Web_Crawler.h
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef _WEB_CRAWLER_H
#define _WEB_CRAWLER_H

#include "URL_Addr.h"
#include "HTTP_URL.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE 
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration.
class URL_Visitor_Factory;

class Web_Crawler
{
  // = TITLE
  //    An abstraction for a Web Crawler.
  //
  // = DESCRIPTION
  //    This class is a Facade that organizes the other classes in the
  //    solution, which include a factory that creates a visitor,
  //    which in turn embodies the appropriate visitation strategy.
public:
  // = Initialization and termination methods.
  Web_Crawler (void);
  // Constructor.

  ~Web_Crawler (void);
  // Destructor.

  int open (int argc, char *argv[]);
  // Parses the command-line options and initializes the
  // <URL_Visitor_Factory>.

  int run (void);
  // Run the Web Crawler and carries out whatever visitation strategy
  // is configured.  Returns -1 on failure and 0 on success.

private:
  URL_Visitor_Factory *url_visitor_factory_;
  // Pointer to a factory that creates visitors that explore URLs and
  // perform various tasks.  Subclasses of <URL_Visitor_Factory>
  // determine what happens during a visitation.
};

#endif /* _WEB_CRAWLER_H */
