// -*- C++ -*-

//=============================================================================
/**
 *  @file   Network_Priority_Mapping.h
 *
 *  $Id$
 *
 *  Class that declares the Network Priority Mapping
 *  that can be overriden to provide different mappings
 *
 *
 *  @author Yamuna Krishnamurthy (yamuna@oomworks.com)
 */
//=============================================================================


#ifndef TAO_NETWORK_PRIORITY_MAPPING_H
#define TAO_NETWORK_PRIORITY_MAPPING_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/RTCORBA_includeC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Network_Priority_Mapping
 *
 * @brief The interface for priority mapping.
 *
 * Check the RT-CORBA spec (orbos/99-02-12) secions 4.5.2
 */
class TAO_RTCORBA_Export TAO_Network_Priority_Mapping
{
public:
  /// The destructor
  virtual ~TAO_Network_Priority_Mapping (void);

  virtual CORBA::Boolean
    to_network (RTCORBA::Priority corba_priority,
                RTCORBA::NetworkPriority &network_priority) = 0;
  virtual CORBA::Boolean
    to_CORBA (RTCORBA::NetworkPriority network_priority,
              RTCORBA::Priority &corba_priority) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_NETWORK_PRIORITY_MAPPING_H */




