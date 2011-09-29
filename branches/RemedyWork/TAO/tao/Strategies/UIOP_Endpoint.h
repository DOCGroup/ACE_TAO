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
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if TAO_HAS_UIOP == 1

#include "tao/Strategies/strategies_export.h"
#include "tao/Endpoint.h"
#include "ace/UNIX_Addr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  TAO_UIOP_Endpoint (const ACE_UNIX_Addr &addr,
                     CORBA::Short priority = TAO_INVALID_PRIORITY);

  /// Destructor.
  ~TAO_UIOP_Endpoint (void);

  /**
   * @name TAO_Endpoint Methods
   *
   * Please check the documentation in Endpoint.h for details.
   */
  //@{
  virtual TAO_Endpoint *next (void);
  virtual int addr_to_string (char *buffer, size_t length);
  virtual TAO_Endpoint *duplicate (void);

  /// Return true if this endpoint is equivalent to @a other_endpoint.  Two
  /// endpoints are equivalent if their rendezvous points are the same.
  CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint);

  /// Return a hash value for this object.
  virtual CORBA::ULong hash (void);
  //@}

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

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Strategies/UIOP_Endpoint.inl"
#endif /* __ACE_INLINE__ */

# endif  /* TAO_HAS_UIOP == 1 */

#include /**/ "ace/post.h"
#endif  /* TAO_UIOP_ENDPOINT_H */
