
//=============================================================================
/**
 *  @file   Priority_Mapping.h
 *
 *  $Id$
 *
 * Declares the Priority_Mapping interface, as defined in the
 * RT-CORBA spec.
 *
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_PRIORITY_MAPPING_H
#define TAO_PRIORITY_MAPPING_H
#include "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#define TAO_RTCORBA_SAFE_INCLUDE
#include "RTCORBAC.h"
#undef TAO_RTCORBA_SAFE_INCLUDE

/**
 * @class TAO_Priority_Mapping
 *
 * @brief The interface for priority mapping.
 *
 * Check the RT-CORBA spec (orbos/99-02-12) secions 4.5.2
 */
class TAO_RTCORBA_Export TAO_Priority_Mapping
{
public:
  /// The destructor
  virtual ~TAO_Priority_Mapping (void);

  virtual CORBA::Boolean
      to_native (RTCORBA::Priority corba_priority,
                 RTCORBA::NativePriority &native_priority) = 0;
  virtual CORBA::Boolean
      to_CORBA (RTCORBA::NativePriority native_priority,
                RTCORBA::Priority &corba_priority) = 0;
};

#if defined (__ACE_INLINE__)
# include "Priority_Mapping.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_PRIORITY_MAPPING_H */
