/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Web_Crawler.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================


#ifndef _WEB_CRAWLER_H
#define _WEB_CRAWLER_H

#include "URL_Addr.h"
#include "HTTP_URL.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration.
class URL_Visitor_Factory;

/**
 * @class Web_Crawler
 *
 * @brief An abstraction for a Web Crawler.
 *
 * This class is a Facade that organizes the other classes in the
 * solution, which include a factory that creates a visitor,
 * which in turn embodies the appropriate visitation strategy.
 */
class Web_Crawler
{
public:
  /// Constructor.
  Web_Crawler ();

  /// Destructor.
  ~Web_Crawler ();

  /// Parses the command-line options and initializes the
  /// <URL_Visitor_Factory>.
  int open (int argc, ACE_TCHAR *argv[]);

  /// Run the Web Crawler and carries out whatever visitation strategy
  /// is configured.  Returns -1 on failure and 0 on success.
  int run ();

private:
  /**
   * Pointer to a factory that creates visitors that explore URLs and
   * perform various tasks.  Subclasses of <URL_Visitor_Factory>
   * determine what happens during a visitation.
   */
  URL_Visitor_Factory *url_visitor_factory_;
};

#endif /* _WEB_CRAWLER_H */
