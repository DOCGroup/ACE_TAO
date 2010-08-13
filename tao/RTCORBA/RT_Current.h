// -*- C++ -*-

//=============================================================================
/**
 *  @file   RT_Current.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_RT_CURRENT_H
#define TAO_RT_CURRENT_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/RTCORBA_includeC.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_RT_Current
 *
 * @brief RTCORBA::Current interface iplementation.
 *
 * Allows setting/getting the priority of the current thread.
 */
class TAO_RTCORBA_Export TAO_RT_Current
  : public RTCORBA::Current
  , public ::CORBA::LocalObject
{
public:

  /// Default constructor.
  TAO_RT_Current (TAO_ORB_Core *orb_core);

  virtual RTCORBA::Priority the_priority (void);

  virtual void the_priority (RTCORBA::Priority the_priority);

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_RT_Current (void);

private:

  /// ORB Core that owns us.
  TAO_ORB_Core * const orb_core_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_RT_CURRENT_H */
