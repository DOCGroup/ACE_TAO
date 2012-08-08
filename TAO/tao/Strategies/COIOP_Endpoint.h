// -*- C++ -*-

//=============================================================================
/**
 *  @file     COIOP_Endpoint.h
 *
 *  $Id$
 *
 *   COIOP implementation of PP Framework Endpoint interface.
 *
 *  @author  Johnny Willemsen <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_COIOP_ENDPOINT_H
#define TAO_COIOP_ENDPOINT_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_COIOP) && (TAO_HAS_COIOP != 0)

#include "tao/Strategies/strategies_export.h"
#include "tao/Endpoint.h"
#include "ace/UUID.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_COIOP_Connection_Handler;

/**
 * @class TAO_COIOP_Endpoint
 *
 * @brief TAO_COIOP_Endpoint
 *
 * COIOP-specific implementation of PP Framework Endpoint interface.
 */
class TAO_Strategies_Export TAO_COIOP_Endpoint : public TAO_Endpoint
{
public:

  friend class TAO_COIOP_Profile;
  friend class TAO_SSLIOP_Profile;

  // = Initialization and termination methods.

  /// Default constructor.
  TAO_COIOP_Endpoint (void);

  /// Constructor.  This is the most efficient constructor since it
  /// does not require any address resolution processing.
  TAO_COIOP_Endpoint (const ACE_Utils::UUID& uuid);

  /// Destructor.
  ~TAO_COIOP_Endpoint (void);

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
  /// endpoints are equivalent if their port and host are the same.
  CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint);

  /// Return a hash value for this object.
  CORBA::ULong hash (void);
  //@}

  // = COIOP_Endpoint-specific methods.

  /// Return a pointer to the host string.  This object maintains
  /// ownership of this string.
  const ACE_Utils::UUID& uuid (void) const;

private:
  /// UUID uniquely identifying this COIOP endpoint
  mutable ACE_Utils::UUID uuid_;

  /// COIOP Endpoints can be strung into a list.  Return the next
  /// endpoint in the list, if any.
  TAO_COIOP_Endpoint *next_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Strategies/COIOP_Endpoint.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_COIOP && TAO_HAS_COIOP != 0 */

#include /**/ "ace/post.h"
#endif  /* TAO_COIOP_PROFILE_H */
