// -*- C++ -*-
//=============================================================================
/**
 *  @file     IIOPEndpointValue_i.h
 *
 *  $Id$
 *
 *  Implementation of the IIOP-Specific endpoint policy value
 *
 *  @author  Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#ifndef _TAO_IIOP_ENDPOINT_VALUE_I_H_
#define _TAO_IIOP_ENDPOINT_VALUE_I_H_

#include /**/ "ace/pre.h"

#include "tao/EndpointPolicy/EndpointPolicy_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/EndpointPolicy/IIOPEndpointValueC.h"
#include "tao/EndpointPolicy/Endpoint_Value_Impl.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class IIOPEndpointValue_i
 *
 * @brief Implementation of the IIOP-Specific endpoint policy value
 *
 * This class is used by applications to create an instance of an
 * endpoint value for constructing an EndpointPolicy. While the
 * endpoint policy argument is of type
 * EndpointPolicy::EndpointValueBase, always use instances of this
 * type so that the endpoint policy framework is able to call the
 * equivalency test method.
 */

class TAO_EndpointPolicy_Export  IIOPEndpointValue_i :
  public virtual EndpointPolicy::IIOPEndpointValue,
  public virtual TAO_Endpoint_Value_Impl,
  public virtual TAO_Local_RefCounted_Object
{
public:

  /// Default Constructor. It is acceptable to create an empty value
  /// and modify the state via the attributes.
  IIOPEndpointValue_i ();

  /// Value initializing constructor, this is typically for creation
  /// of one-off values.
  IIOPEndpointValue_i (const char *host,
                       CORBA::UShort port);

  virtual ~IIOPEndpointValue_i (void);

  /// The is_equivalent test is used by the endpoint policy framework
  /// for testing if a target endpoint is the same as the endpoint
  /// defined by this value.
  CORBA::Boolean is_equivalent (const TAO_Endpoint * endpoint) const;

  /// The validate_acceptor method is used during EndpointPolicy
  /// creation to ensure there is an acceptor which provides this
  /// endpoint. Currently the test is limited to only validating that
  /// the acceptor's tag is consistent, as the interfaces do not yet
  /// exist for examining the eventual TAO_Endpoint values the
  /// acceptor would produce.
  CORBA::Boolean validate_acceptor (TAO_Acceptor *) const;

  /// Host attribute get/set operators.
  char * host ( void )
    ACE_THROW_SPEC ((CORBA::SystemException));
  void host (const char *)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Port attribute get/set operators.
  CORBA::UShort port (void)
    ACE_THROW_SPEC ((CORBA::SystemException));
  void port (CORBA::UShort )
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Protocol tag get operator, inherited from EndpointValueBase
  CORBA::ULong protocol_tag (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::Boolean is_equivalent_i (CORBA::UShort port, const char *host) const;

  CORBA::String_var host_;
  CORBA::UShort     port_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */

#include /**/ "ace/post.h"

#endif /* _TAO_IIOP_ENDPOINT_VALUE_I_H_ */
