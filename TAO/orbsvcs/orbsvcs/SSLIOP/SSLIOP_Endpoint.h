// -*- C++ -*-

//=============================================================================
/**
 *  @file     SSLIOP_Endpoint.h
 *
 *  $Id$
 *
 *  SSLIOP implementation of PP Framework Endpoint interface.
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_SSLIOP_ENDPOINT_H
#define TAO_SSLIOP_ENDPOINT_H

#include "ace/pre.h"

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/INET_Addr.h"

#include "tao/ORB.h"
#include "tao/IIOP_Endpoint.h"
#include "orbsvcs/SSLIOPC.h"

// Tag for storing multiple ssl endpoints within a single profile.
#define TAO_TAG_SSL_ENDPOINTS 0x54414f01U


class TAO_SSLIOP_Export TAO_SSLIOP_Endpoint : public TAO_Endpoint
{
  // = TITLE
  //   TAO_SSLIOP_Endpoint
  //
  // = DESCRIPTION
  //   SSLIOP-specific implementation of PP Framework Endpoint interface.
  //
public:

  friend class TAO_SSLIOP_Profile;

  // = Initialization and termination methods.

  TAO_SSLIOP_Endpoint (const SSLIOP::SSL *ssl_component,
                       TAO_IIOP_Endpoint *iiop_endp);
  // Constructor.

  ~TAO_SSLIOP_Endpoint (void);
  // Destructor.

  // = Implementation of abstract TAO_Endpoint methods.  See
  // Endpoint.h for their documentation.

  virtual TAO_Endpoint *next (void);
  virtual int addr_to_string (char *buffer, size_t length);
  virtual void reset_hint (void);

  // = SSLIOP_Endpoint-specific methods.

  const SSLIOP::SSL &ssl_component (void) const;
  // Return SSL component corresponding to this endpoint.

  CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint);
  // Return true if this endpoint is equivalent to <other_endpoint>.  Two
  // endpoints are equivalent iff their iiop counterparts are
  // equivalent, and, if both have non-zero ssl ports, their ssl ports
  // are the same.

  TAO_IIOP_Endpoint *iiop_endpoint (void) const;
  // Accessor to our IIOP counterpart.

  virtual TAO_Endpoint *duplicate (void);
  // This method returns a copy of the corresponding endpoints by
  // allocation memory

  virtual CORBA::ULong hash (void);
  // Return a hash value for this object.

  /// Return the SSLIOP-specific ACE_INET_Addr.
  const ACE_INET_Addr &object_addr (void) const;

private:

  SSLIOP::SSL ssl_component_;
  // Cache the SSL tagged component in a decoded format. Notice that
  // we do not need to marshal this object!

  /// Cached instance of ACE_INET_Addr for use in making invocations,
  /// etc.
  ACE_INET_Addr object_addr_;

  TAO_SSLIOP_Endpoint *next_;
  // IIOP Endpoints can be stringed into a list.  Return the next
  // endpoint in the list, if any.

  TAO_IIOP_Endpoint *iiop_endpoint_;
  // Our IIOP counterpart.
  // Since SSLIOP is an 'extension' of IIOP, each SSLIOP_Endpoint
  // contains ssl-specific info plus a pointer to the IIOP_Endpoint
  // containing the iiop portion of our address.
};

#if defined (__ACE_INLINE__)
#include "SSLIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* TAO_SSLIOP_ENDPOINT_H */
