// -*- C++ -*-

//==========================================================================
/**
 * @file UIOP_Endpoint.h
 *
 * $Id$
 *
 * UIOP implementation of PP Framework Endpoint interface.
 *
 * @author  Marina Spivak <marina@cs.wustl.edu>
 */
//==========================================================================

#ifndef TAO_UIOP_ENDPOINT_H
#define TAO_UIOP_ENDPOINT_H
#include "ace/pre.h"

#include "tao/Endpoint.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if TAO_HAS_UIOP == 1

#include "strategies_export.h"
#include "ace/UNIX_Addr.h"


/**
 * @class TAO_UIOP_Endpoint
 *
 * @brief TAO_UIOP_Endpoint
 *
 * UIOP-specific implementation of PP Framework Endpoint interface.
 */
class TAO_Strategies_Export TAO_UIOP_Endpoint : public TAO_Endpoint
{
public:

  friend class TAO_UIOP_Profile;

  // = Initialization and termination methods.

  /// Default constructor.
  TAO_UIOP_Endpoint (void);

  /// Constructor.
  TAO_UIOP_Endpoint (const ACE_UNIX_Addr &addr);

  /// Destructor.
  ~TAO_UIOP_Endpoint (void);

  // = Implementation of abstract TAO_Endpoint methods.  See
  // Endpoint.h for their documentation.

  virtual TAO_Endpoint *next (void);
  virtual int addr_to_string (char *buffer, size_t length);
  virtual void reset_hint (void);

  /// Returns a copy of <this>
  virtual TAO_Endpoint *duplicate (void);

  /// Return true if this endpoint is equivalent to <other_endpoint>.  Two
  /// endpoints are equivalent iff their rendezvous points are the same.
  CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint);

  /// Return a hash value for this object.
  virtual CORBA::ULong hash (void);

  // = UIOP_Endpoint-specific methods.

  /// Return a reference to the <object_addr>.
  const ACE_UNIX_Addr &object_addr (void) const;

  /// Return a pointer to the rendezvous point string.
  /// This object maintains ownership of the returned string.
  const char *rendezvous_point (void) const;

private:

  /// Cached instance of <ACE_UNIX_Addr> for use in making
  /// invocations, etc.
  ACE_UNIX_Addr object_addr_;

  /// UIOP Endpoints can be stringed into a list.  Return the next
  /// endpoint in the list, if any.
  TAO_UIOP_Endpoint *next_;
};

#if defined (__ACE_INLINE__)
# include "UIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

# endif  /* TAO_HAS_UIOP == 1 */

#include "ace/post.h"
#endif  /* TAO_UIOP_ENDPOINT_H */
