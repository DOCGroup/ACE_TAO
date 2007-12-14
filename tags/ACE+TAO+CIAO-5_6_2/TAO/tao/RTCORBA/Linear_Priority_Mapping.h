// -*- C++ -*-


//=============================================================================
/**
 *  @file   Linear_Priority_Mapping.h
 *
 *  $Id$
 *
 * Declares the Linear_Priority_Mapping interface, as defined in the
 * RT-CORBA spec.
 *
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_LINEAR_PRIORITY_MAPPING_H
#define TAO_LINEAR_PRIORITY_MAPPING_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/rtcorba_export.h"
#include "tao/RTCORBA/Priority_Mapping.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Linear_Priority_Mapping
 *
 * @brief A simple implementation of the Priority_Mapping interface.
 *
 * This implementation uses linear mapping between the range of
 * priorities for a given scheduling class (ACE_SCHED_OTHER,
 * ACE_SCHED_FIFO, ACE_SCHED_RR) and the valid range of CORBA
 * priorities (0...32767)
 */
class TAO_RTCORBA_Export TAO_Linear_Priority_Mapping : public TAO_Priority_Mapping
{
public:
  /// Default constructor
  TAO_Linear_Priority_Mapping (long policy);

  /// The destructor
  virtual ~TAO_Linear_Priority_Mapping (void);

  virtual CORBA::Boolean to_native (RTCORBA::Priority corba_priority,
                                    RTCORBA::NativePriority &native_priority);
  virtual CORBA::Boolean to_CORBA (RTCORBA::NativePriority native_priority,
                                   RTCORBA::Priority &corba_priority);

private:
  /// The scheduling policy
  long policy_;

  // The range
  int const min_;
  int const max_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_LINEAR_PRIORITY_MAPPING_H */
