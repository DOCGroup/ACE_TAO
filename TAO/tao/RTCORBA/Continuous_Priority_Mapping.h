//=============================================================================
/**
 *  @file    Continuous_Priority_Mapping.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 */
// ===================================================================

#ifndef TAO_CONTINUOUS_PRIORITY_MAPPING_H
#define TAO_CONTINUOUS_PRIORITY_MAPPING_H

#include /**/ "ace/pre.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "rtcorba_export.h"
#include "Priority_Mapping.h"

/**
 * @class TAO_Continuous_Priority_Mapping
 *
 * @brief Maps the first n CORBA priorities to the range of native
 * priorities, where n is the number of native priorities.
 *
 * The lowest native priority is mapped to CORBA priority 0, next
 * higher native priority is mapped to CORBA priority 1, and so
 * on. Since in all the operating systems where TAO is supported the
 * native priority set contains less than 32767 priorities, part of
 * the CORBA priority range is left unused.  Consider NT as an
 * example. NT native priorities -15 -2 -1 0 1 2 15 are mapped to
 * CORBA priorities 0 1 2 3 4 5 6, respectively, and the rest of the
 * CORBA priority range is not used.
 *
 * This class was previously called Direct_Priority_Mapping.
 *
 * \nosubgrouping
 *
 **/
class TAO_RTCORBA_Export TAO_Continuous_Priority_Mapping
  : public TAO_Priority_Mapping
{
public:

  /// Constructor.
  TAO_Continuous_Priority_Mapping (int policy = ACE_SCHED_OTHER);

  /// Destructor.
  ~TAO_Continuous_Priority_Mapping (void);

  /// Convert CORBA priority to native priority.
  CORBA::Boolean to_native (RTCORBA::Priority corba_priority,
                            RTCORBA::NativePriority &native_priority);

  /// Convert native priority to CORBA priority.
  CORBA::Boolean to_CORBA (RTCORBA::NativePriority native_priority,
                           RTCORBA::Priority &corba_priority);

private:
  int policy_;
  int min_;
  int max_;
};

#if defined (__ACE_INLINE__)
# include "Continuous_Priority_Mapping.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_CONTINUOUS_PRIORITY_MAPPING_H */
