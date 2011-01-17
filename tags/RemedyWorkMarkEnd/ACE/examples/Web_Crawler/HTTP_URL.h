/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    examples/Web_Crawler
//
// = FILENAME
//    HTTP_URL.h
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef _HTTP_URL_H
#define _HTTP_URL_H

#include "URL_Status.h"
#include "URL.h"
#include "Options.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class HTTP_URL : public URL
{
  // = TITLE
  //    An ADT for an HTTP URL.
  //
  // = DESCRIPTION
  //    This class plays the "element" role in the Visitor pattern.
public:
  HTTP_URL (const ACE_URL_Addr &url_addr,
            HTTP_URL *containing_page = 0);
  // The <url_addr> is the URL that we're going to be visiting.  We
  // also keep track of the containing page, if any, which is used to
  // print out more meaningful messages.

  virtual int accept (URL_Visitor *visitor);
  // Accept the visitor, which will then perform a particular
  // visitation strategy on the URL.  This method is part of the
  // Visitor pattern.

  virtual ssize_t send_request (void);
  // Send a <GET> command to fetch the contents in the URI from the
  // server.

  virtual const ACE_URL_Addr &url_addr (void) const;
  // Returns the URL that we represent.

  int destroy (void);
  // Commit suicide
private:
  ACE_URL_Addr url_addr_;
  // Address of the URL we're connected to.

  HTTP_URL *containing_page_;
  // Page that contained us.
};

#endif /* _HTTP_URL_H */
