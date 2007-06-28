// -*- C++ -*-

// =======================================================================
/**
 *  @file    Direct_Priority_Mapping.h
 *
 *  $Id$
 *
 *  Declares the Direct_Priority_Mapping interface, as defined in the
 *  RT-CORBA spec.
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
// =======================================================================

#ifndef TAO_DIRECT_PRIORITY_MAPPING_H
#define TAO_DIRECT_PRIORITY_MAPPING_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/Priority_Mapping.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Direct_Priority_Mapping
 *
 * @brief A simple implementation of the Priority_Mapping interface
 *
 * This implementation uses direct mapping.
 */
class TAO_RTCORBA_Export TAO_Direct_Priority_Mapping : public TAO_Priority_Mapping
{
public:

  TAO_Direct_Priority_Mapping (long);

  /// The destructor
  virtual ~TAO_Direct_Priority_Mapping (void);

  virtual CORBA::Boolean
      to_native (RTCORBA::Priority corba_priority,
                 RTCORBA::NativePriority &native_priority);
  virtual CORBA::Boolean
      to_CORBA (RTCORBA::NativePriority native_priority,
                RTCORBA::Priority &corba_priority);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_DIRECT_PRIORITY_MAPPING_H */
