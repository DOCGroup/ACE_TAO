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
// = DESCRIPTION
//   Endpoint interface, part of TAO pluggable protocol framework.
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
  //   Lightweight encapsulation of addressing information for a
  //   single acceptor endpoint.  In other words, Endpoint represents
  //   a single point of contact for the server, and is the smallest
  //   unit of addressing information necessary for a client to connect
  //   to a server.
  //   A Profile contains one or more Endpoints, i.e., knows of
  //   one or more ways to contact server(s).
  //
public:
  TAO_Endpoint (CORBA::ULong tag);
  // Constructor.

  virtual ~TAO_Endpoint (void);
  // Destructor.

  CORBA::ULong tag (void) const;
  // IOP protocol tag accessor.

  void priority (CORBA::Short priority);
  // <priority_> attribute setter.

  CORBA::Short priority (void) const;
  // <priority_> attribute getter.

  // = Abstract methods to be implemented by concrete subclasses.

  virtual TAO_Endpoint *next (void) = 0;
  // Endpoints can be stringed in a list.  Return the next endpoint in
  // the list, if any.

  virtual int addr_to_string (char *buffer, size_t length) = 0;
  // Return a string representation for the address.  Returns
  // -1 if buffer is too small.  The purpose of this method is to
  // provide a general interface to the underlying address object's
  // addr_to_string method.  This allows the protocol implementor to
  // select the appropriate string format.

  virtual void reset_hint (void) = 0;
  // This method is used when a connection has been reset, requiring
  // the hint to be cleaned up and reset to NULL.

  //virtual TAO_Endpoint *duplicate (void) = 0;
  // This method returns a copy of the corresponding endpoints by
  // allocation memory

private:

  // Endpoints should not be copied.
  ACE_UNIMPLEMENTED_FUNC (TAO_Endpoint (const TAO_Endpoint&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Endpoint&))

  CORBA::ULong tag_;
  // IOP tag, identifying the protocol for which this endpoint
  // contains addressing info.

  CORBA::Short priority_;
  // CORBA priority of the acceptor this Endpoint is representing.
  // This is part of TAO 'prioritized endpoints' architecture, and is
  // currently used for RTCORBA only.
};

#if defined (__ACE_INLINE__)
# include "tao/Endpoint.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_PROFILE_H */
