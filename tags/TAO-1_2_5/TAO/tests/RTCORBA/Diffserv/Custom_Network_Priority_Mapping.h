// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Custom_Network_Priority_Mapping.h
//
// = DESCRIPTION
//   Declares the Custom_Network_Priority_Mapping interface, 
//
// = AUTHOR
//   Yamuna Krishnamurthy (yamuna@oomworks.com)
//
// ============================================================================

#ifndef TAO_CUSTOM_NETWORK_PRIORITY_MAPPING_H
#define TAO_CUSTOM_NETWORK_PRIORITY_MAPPING_H
#include "ace/pre.h"

#include "tao/orbconf.h"

#include "tao/RTCORBA/rtcorba_export.h"
#include "tao/RTCORBA/Network_Priority_Mapping.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_RTCORBA_Export TAO_Custom_Network_Priority_Mapping : public TAO_Network_Priority_Mapping
{
  //
  // = TITLE
  //   A simple implementation of the Network_Priority_Mapping interface
  //
  // = DESCRIPTION
  //   This implementation uses custom mapping between the range of
  //   priorities for a given scheduling class (ACE_SCHED_OTHER,
  //   ACE_SCHED_FIFO, ACE_SCHED_RR) and the valid range of CORBA
  //   priorities (0...32767)
  //
public:
  TAO_Custom_Network_Priority_Mapping (void);
  // Default constructor

  virtual ~TAO_Custom_Network_Priority_Mapping (void);
  // The destructor

  virtual CORBA::Boolean
      to_network (RTCORBA::Priority corba_priority,
                 RTCORBA::NetworkPriority &network_priority);
  virtual CORBA::Boolean
      to_CORBA (RTCORBA::NetworkPriority network_priority,
                RTCORBA::Priority &corba_priority);

  void desired_priority (int priority);

 private:
  int corba_priority_;
  int max_;
  int min_;
  int increment_;
};


#include "ace/post.h"
#endif /* TAO_CUSTOM_NETWORK_PRIORITY_MAPPING_H */
