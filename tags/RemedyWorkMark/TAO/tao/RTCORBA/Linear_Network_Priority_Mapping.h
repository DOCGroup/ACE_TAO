// -*- C++ -*-


//=============================================================================
/**
 *  @file   Linear_Network_Priority_Mapping.h
 *
 *  $Id$
 *
 * Declares the Linear_Network_Priority_Mapping interface,
 *
 *
 *  @author Yamuna Krishnamurthy (yamuna@oomworks.com)
 */
//=============================================================================


#ifndef TAO_LINEAR_NETWORK_PRIORITY_MAPPING_H
#define TAO_LINEAR_NETWORK_PRIORITY_MAPPING_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/rtcorba_export.h"
#include "tao/RTCORBA/Network_Priority_Mapping.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Linear_Network_Priority_Mapping
 *
 * @brief A simple implementation of the Network_Priority_Mapping interface
 *
 * This implementation uses linear mapping between the range of
 * DiffServ priorities and CORBA priorities.
 */
class TAO_RTCORBA_Export TAO_Linear_Network_Priority_Mapping : public TAO_Network_Priority_Mapping
{
public:

  TAO_Linear_Network_Priority_Mapping (long);

  CORBA::Boolean to_network (RTCORBA::Priority corba_priority,
                             RTCORBA::NetworkPriority &network_priority);

  CORBA::Boolean to_CORBA (RTCORBA::NetworkPriority network_priority,
                           RTCORBA::Priority &corba_priority);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_LINEAR_NETWORK_PRIORITY_MAPPING_H */
