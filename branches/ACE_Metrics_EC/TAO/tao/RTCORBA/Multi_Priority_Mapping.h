// $Id$
// Multi_Priority_Mapping.h,v 1.0

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Multi_Priority_Mapping.h
//
// = DESCRIPTION
//   Declares a priority mapping to support cross platform communication.
//
// = AUTHOR
//   Brian Mendel (brian.r.mendel@boeing.com)
//
// ============================================================================

#ifndef TAO_MULTI_PRIORITY_MAPPING_H
#define TAO_MULTI_PRIORITY_MAPPING_H

#include "tao/orbconf.h"

#include "tao/RTCORBA/Priority_Mapping.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export TAO_Multi_Priority_Mapping : public TAO_Priority_Mapping
{
  //
  // = TITLE
  //   An implementation of the Priority_Mapping interface for communication between different platforms
  //
  // = DESCRIPTION
  //   This implementation uses a custom mapping between the range of
  //   priorities for a given scheduling class (ACE_SCHED_OTHER,
  //   ACE_SCHED_FIFO, ACE_SCHED_RR) and the valid range of CORBA
  //   priorities (0...32767)
  //
public:
  TAO_Multi_Priority_Mapping (int base_native_priority,      // The native priority to use for the highest priority endpoint
                              int base_corba_priority,       // The corba priority to use for the highest priority endpoint
                              int priority_spacing = 1,      // The priority increment to use between endpoints
                              int priorities_contiguous = 1, // Some platforms do use contiguous priorities
                              int policy = ACE_SCHED_FIFO);  // The scheduling policy to use.
  // Default constructor

  virtual ~TAO_Multi_Priority_Mapping (void);
  // The destructor

  virtual CORBA::Boolean
      to_native (RTCORBA::Priority corba_priority,
                 RTCORBA::NativePriority &native_priority);
  virtual CORBA::Boolean
      to_CORBA (RTCORBA::NativePriority native_priority,
                RTCORBA::Priority &corba_priority);

private:

  int base_native_priority_;
  int base_corba_priority_;
  // The base settings

  const int priority_spacing_;
  const int priorities_contiguous_;

  int policy_;
  // The scheduling policy

  int min_;
  int max_;
  // The range
};

#if defined (__ACE_INLINE__)
# include "tao/RTCORBA/Multi_Priority_Mapping.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_MULTI_PRIORITY_MAPPING_H */
