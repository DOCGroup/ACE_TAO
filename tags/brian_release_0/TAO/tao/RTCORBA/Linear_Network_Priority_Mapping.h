// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Linear_Network_Priority_Mapping.h
//
// = DESCRIPTION
//   Declares the Linear_Network_Priority_Mapping interface,
//
// = AUTHOR
//   Yamuna Krishnamurthy (yamuna@oomworks.com)
//
// ============================================================================

#ifndef TAO_LINEAR_NETWORK_PRIORITY_MAPPING_H
#define TAO_LINEAR_NETWORK_PRIORITY_MAPPING_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#include "rtcorba_export.h"
#include "Network_Priority_Mapping.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

class TAO_RTCORBA_Export TAO_Linear_Network_Priority_Mapping : public TAO_Network_Priority_Mapping
{
  //
  // = TITLE
  //   A simple implementation of the Network_Priority_Mapping interface
  //
  // = DESCRIPTION
  //   This implementation uses linear mapping between the range of
  //   priorities for a given scheduling class (ACE_SCHED_OTHER,
  //   ACE_SCHED_FIFO, ACE_SCHED_RR) and the valid range of CORBA
  //   priorities (0...32767)
  //
public:
  TAO_Linear_Network_Priority_Mapping (long policy);
  // Default constructor

  virtual ~TAO_Linear_Network_Priority_Mapping (void);
  // The destructor

  virtual CORBA::Boolean
      to_network (RTCORBA::Priority corba_priority,
                  RTCORBA::NetworkPriority &network_priority);
  virtual CORBA::Boolean
      to_CORBA (RTCORBA::NetworkPriority network_priority,
                RTCORBA::Priority &corba_priority);
 private:
    long policy_;
  // The scheduling policy

  int min_;
  int max_;

  int increment_;

};

#if defined (__ACE_INLINE__)
# include "Linear_Network_Priority_Mapping.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_LINEAR_NETWORK_PRIORITY_MAPPING_H */
