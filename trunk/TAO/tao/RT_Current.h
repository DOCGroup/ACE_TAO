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
#include "ace/pre.h"

#include "tao/orbconf.h"

#if (TAO_HAS_RT_CORBA == 1)

#include "tao/RTCORBAS.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_RT_Current
 *
 * @brief RTCORBA::Current interface iplementation.
 *
 * Allows setting/getting the priority of the current thread.
 */
class TAO_Export TAO_RT_Current
  : public RTCORBA::Current,
    public TAO_Local_RefCounted_Object
{
public:

  /// Default constructor.
  TAO_RT_Current (TAO_ORB_Core *orb_core);

  virtual RTCORBA::Priority the_priority (CORBA::Environment
                                          &ACE_TRY_ENV =
                                          TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void the_priority (RTCORBA::Priority the_priority,
                             CORBA::Environment &ACE_TRY_ENV =
                             TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_RT_Current (void);

private:

  /// ORB Core that owns us.
  TAO_ORB_Core *orb_core_;

};


#if defined (__ACE_INLINE__)
# include "tao/RT_Current.i"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_RT_CORBA == 1 */

#include "ace/post.h"
#endif /* TAO_RT_CURRENT_H */
