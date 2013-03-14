// -*- C++ -*-

//=============================================================================
/**
 * @file   ServerORBInitializer.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef SERVER_ORB_INITIALIZER_H
#define SERVER_ORB_INITIALIZER_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class ServerORBInitializer
 *
 * @brief Simple concrete ORB initializer.
 *
 * This is a test ORB initializer that registers an IORInterceptor and
 * ServerRequestIntercepor for this ORT test.
 */
class ServerORBInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual ::CORBA::LocalObject
{
public:

  /**
   * @name Methods Required by the ORBInitializer Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all ORB initializers.
   */
  //@{
  /// The pre-initialization hook.
  virtual void pre_init (
      PortableInterceptor::ORBInitInfo_ptr info);

  /// The post-initialization hook.
  virtual void post_init (
      PortableInterceptor::ORBInitInfo_ptr info);
  //@}

};


#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* SERVER_ORB_INITIALIZER_H */
