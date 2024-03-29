/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    URL.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================


#ifndef _URL_H
#define _URL_H

#include "Mem_Map_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "URL_Addr.h"
#include "URL_Status.h"

#include "ace/SString.h"


// Forward declaration.
class URL_Visitor;

/**
 * @class URL
 *
 * @brief Base class for a URL.
 *
 * This class plays a role in the Visitor pattern.
 */
class URL
{
public:
  /// Destructor.
  virtual ~URL ();

  /**
   * Accept the visitor, which will then perform a particular
   * visitation strategy on the URL.  This method is part of the
   * Visitor pattern.
   */
  virtual int accept (URL_Visitor *visitor) = 0;

  /// Send a <GET> command to fetch the contents in the URI from the
  /// server.
  virtual ssize_t send_request () = 0;

  /// Returns the URL that we represent.
  virtual const ACE_URL_Addr &url_addr () const = 0;

  /// Returns the <Mem_Map_Stream>.
  virtual Mem_Map_Stream &stream ();

  // = Get/set the reply status.
  virtual const URL_Status &reply_status ();
  virtual void reply_status (const URL_Status &);

  // = Get/set the reply status.
  virtual const ACE_CString &content_type ();
  virtual void content_type (const ACE_CString &);


private:
  /// Reply status of the URL.
  URL_Status reply_status_;

  /// Content-type of the URL.
  ACE_CString content_type_;

  /// Contents of the stream.
  Mem_Map_Stream stream_;
};

#endif /* _URL_H */
