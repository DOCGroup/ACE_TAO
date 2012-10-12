
//=============================================================================
/**
 *  @file   Custom_Network_Priority_Mapping.h
 *
 *  $Id$
 *
 * Declares the Custom_Network_Priority_Mapping interface,
 *
 *
 *  @author Yamuna Krishnamurthy (yamuna@oomworks.com)
 */
//=============================================================================


#ifndef CUSTOM_NETWORK_PRIORITY_MAPPING_H
#define CUSTOM_NETWORK_PRIORITY_MAPPING_H

#include /**/ "ace/pre.h"
#include "tao/RTCORBA/Network_Priority_Mapping.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Custom_Network_Priority_Mapping
 *
 * @brief A simple implementation of the Network_Priority_Mapping interface
 *
 * This implementation uses custom mapping between the range of
 * priorities for a given scheduling class (ACE_SCHED_OTHER,
 * ACE_SCHED_FIFO, ACE_SCHED_RR) and the valid range of CORBA
 * priorities (0...32767)
 */
class Custom_Network_Priority_Mapping : public TAO_Network_Priority_Mapping
{
public:
  Custom_Network_Priority_Mapping (void);

  CORBA::Boolean to_network (RTCORBA::Priority corba_priority,
                             RTCORBA::NetworkPriority &network_priority);

  CORBA::Boolean to_CORBA (RTCORBA::NetworkPriority network_priority,
                           RTCORBA::Priority &corba_priority);

  void corba_priority (RTCORBA::Priority corba_priority);

 private:
  RTCORBA::Priority corba_priority_;
};


#include /**/ "ace/post.h"
#endif /* CUSTOM_NETWORK_PRIORITY_MAPPING_H */
