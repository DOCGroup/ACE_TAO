
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

#if (TAO_HAS_RT_CORBA == 1)

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/RTCORBAC.h"

/**
 * @class TAO_Priority_Mapping
 *
 * @brief The interface for priority mapping.
 *
 * Check the RT-CORBA spec (orbos/99-02-12) secions 4.5.2
 */
class TAO_Export TAO_Priority_Mapping
{
public:
  /// Default constructor
  TAO_Priority_Mapping (void);

  /// The destructor
  virtual ~TAO_Priority_Mapping (void);

  virtual CORBA::Boolean
      to_native (RTCORBA::Priority corba_priority,
                 RTCORBA::NativePriority &native_priority);
  virtual CORBA::Boolean
      to_CORBA (RTCORBA::NativePriority native_priority,
                RTCORBA::Priority &corba_priority);
};

#if defined (__ACE_INLINE__)
# include "tao/Priority_Mapping.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_RT_CORBA == 1 */

#include "ace/post.h"
#endif /* TAO_PRIORITY_MAPPING_H */
