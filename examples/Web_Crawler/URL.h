/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps/Web
//
// = FILENAME
//    URL.h
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef _URL_H
#define _URL_H

#include "Mem_Map_Stream.h"
#include "URL_Addr.h"
#include "URL_Status.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE 
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration.
class URL_Visitor;

class URL
{
  // = TITLE
  //    Base class for a URL.
  // 
  // = DESCRIPTION
  //    This class plays a role in the Visitor pattern.
public:
  virtual ~URL (void);
  // Destructor.

  virtual int accept (URL_Visitor *visitor) = 0;
  // Accept the visitor, which will then perform a particular
  // visitation strategy on the URL.  This method is part of the
  // Visitor pattern.

  virtual int send_request (void) = 0;
  // Send a <GET> command to fetch the contents in the URI from the
  // server.
  
  virtual const ACE_URL_Addr &url_addr (void) const = 0;
  // Returns the URL that we represent.

  virtual Mem_Map_Stream &stream (void);
  // Returns the <Mem_Map_Stream>.

  // = Get/set the reply status.
  virtual const URL_Status &reply_status (void);
  virtual void reply_status (const URL_Status &);

  // = Get/set the reply status.
  virtual const ACE_CString &content_type (void);
  virtual void content_type (const ACE_CString &);

  

private:
  URL_Status reply_status_;
  // Reply status of the URL.

  ACE_CString content_type_;
  // Content-type of the URL.

  Mem_Map_Stream stream_;
  // Contents of the stream.
};

#endif /* _URL_H */
