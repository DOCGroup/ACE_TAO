// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Endpoint.h
//
// = AUTHOR
//   Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ENDPOINT_H
#define TAO_ENDPOINT_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export TAO_Endpoint
{
  // = TITLE
  //   Defines the Endpoint interface in the Pluggable Protocol
  //   framework. 
  //
  // = DESCRIPTION
  //   A lightweight object encapsulating a addressing information for
  //   a single endpoint.
  //
public:
  TAO_Endpoint (CORBA::ULong tag);
  // Constructor

  virtual ~TAO_Endpoint (void);
  // If you have a virtual method you need a virtual dtor.

  CORBA::ULong tag (void) const;
  // The tag, each concrete class will have a specific tag value.

  void priority (CORBA::Short priority);
  // Sets the <priority_> attribute.

  virtual TAO_Endpoint *next (void) = 0;
  // Return the next endpoint in the list.

  CORBA::Short priority (void) const;
  // Gets the <priority_> attribute.

  virtual int addr_to_string (char *buffer, size_t length) = 0;
  // Return a string representation for the address.  Returns
  // -1 if buffer is too small.  The purpose of this method is to
  // provide a general interface to the underlying address object's
  // addr_to_string method.  This allows the protocol implementor to
  // select the appropriate string format.

  virtual void reset_hint (void) = 0;
  // This method is used with a connection has been reset requiring
  // the hint to be cleaned up and reset to NULL.

private:

  // Profiles should not be copied!
  ACE_UNIMPLEMENTED_FUNC (TAO_Endpoint (const TAO_Endpoint&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Endpoint&))

  CORBA::ULong tag_;
  // IOP protocol tag.

  CORBA::Short priority_;
  // 
};

#if defined (__ACE_INLINE__)
# include "tao/Endpoint.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_PROFILE_H */
