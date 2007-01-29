// -*- C++ -*-
//=============================================================================
/**
 *  @file     Endpoint_Value_Impl.h
 *
 *  $Id$
 *
 *  Implementation of the IIOP-Specific endpoint policy value
 *
 *  @author  Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#ifndef _TAO_ENDPOINT_VALUE_IMPL_H_
#define _TAO_ENDPOINT_VALUE_IMPL_H_

#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"
#include "tao/EndpointPolicy/EndpointPolicy_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Endpoint;
class TAO_Acceptor;

/**
 * @class Endpoint_Value_Impl
 *
 * @brief Abstract base class to be mixed in to servants of protocol
 * specific EndpointValues.
 *
 * This inteface defines the methods necessary for the
 * protocol-specific endpoint value to be used with the endpoint
 * policy
 */

class TAO_EndpointPolicy_Export  TAO_Endpoint_Value_Impl
{
public:
  virtual ~TAO_Endpoint_Value_Impl(void);

  /// This method is used to compare a candidate IOR endpoint with the
  /// endpoint defined by this policy value.
  virtual CORBA::Boolean is_equivalent (const TAO_Endpoint * ) const = 0;

  /// This method is used by the framework to validate that an
  /// acceptor is available to produce an endpoint required by at
  /// least one of the endpoint policies values.
  virtual CORBA::Boolean validate_acceptor (TAO_Acceptor *) const = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* _TAO_Endpoint_Value_Impl_H_ */
