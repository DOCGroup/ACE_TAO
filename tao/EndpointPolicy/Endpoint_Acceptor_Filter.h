// -*- C++ -*-

//=============================================================================
/**
 *  @file    Endpoint_Acceptor_Filter.h
 *
 *  $Id$
 *
 *  Subclass of TAO_Default_Acceptor_Filter that populates mprofile and filters
 *  mprofile with the endpoint policy in TAO_POA_Manager.
 *
 *  @author  Yan Dai <dai_y@ociweb.com>
 */
//=============================================================================


#ifndef TAO_ENDPOINT_ACCEPTOR_FILTER_H
#define TAO_ENDPOINT_ACCEPTOR_FILTER_H

#include /**/ "ace/pre.h"

#include "tao/EndpointPolicy/EndpointPolicy_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Default_Acceptor_Filter.h"
#include "tao/EndpointPolicy/EndpointPolicyC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_POA_Manager;

//=============================================================================
/**
 * @class TAO_Endpoint_Acceptor_Filter
 *
 * @brief Subclass of default Acceptor_Filter.
 *
 * Endpoint strategy for populating mprofile: only the endpoints match
 * the POAManager endpoint policy are included.
 */
//=============================================================================
class TAO_EndpointPolicy_Export TAO_Endpoint_Acceptor_Filter
  : public TAO_Default_Acceptor_Filter
{
public:
  TAO_Endpoint_Acceptor_Filter (const EndpointPolicy::EndpointList & eps);

  /// Populate @a mprofile with endpoints match the POAManager endpoint
  /// policy.
  int fill_profile (const TAO::ObjectKey &object_key,
                    TAO_MProfile &mprofile,
                    TAO_Acceptor **acceptors_begin,
                    TAO_Acceptor **acceptors_end,
                    CORBA::Short priority = TAO_INVALID_PRIORITY);

private:
  // this is the list of endpoints as obtained from the policy value supplied
  // to the constructor.
  EndpointPolicy::EndpointList endpoints_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ENDPOINT_ACCEPTOR_FILTER_H */
