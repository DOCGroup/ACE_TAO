/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    HTTP_URL.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#ifndef _HTTP_URL_H
#define _HTTP_URL_H

#include "URL_Status.h"
#include "URL.h"
#include "Options.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class HTTP_URL
 *
 * @brief An ADT for an HTTP URL.
 *
 * This class plays the "element" role in the Visitor pattern.
 */
class HTTP_URL : public URL
{
public:
  /**
   * The <url_addr> is the URL that we're going to be visiting.  We
   * also keep track of the containing page, if any, which is used to
   * print out more meaningful messages.
   */
  HTTP_URL (const ACE_URL_Addr &url_addr,
            HTTP_URL *containing_page = 0);

  /**
   * Accept the visitor, which will then perform a particular
   * visitation strategy on the URL.  This method is part of the
   * Visitor pattern.
   */
  virtual int accept (URL_Visitor *visitor);

  /// Send a <GET> command to fetch the contents in the URI from the
  /// server.
  virtual ssize_t send_request (void);

  /// Returns the URL that we represent.
  virtual const ACE_URL_Addr &url_addr (void) const;

  /// Commit suicide
  int destroy (void);
private:
  /// Address of the URL we're connected to.
  ACE_URL_Addr url_addr_;

  /// Page that contained us.
  HTTP_URL *containing_page_;
};

#endif /* _HTTP_URL_H */
