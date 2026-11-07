// -*- C++ -*-

//=============================================================================
/**
 * @file FOO_Client_ORBInitializer.h
 *
 * Implementation header for the "FOO" IOR test interceptor ORB
 * initializer.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef FOO_CLIENT_ORB_INITIALIZER_H
#define FOO_CLIENT_ORB_INITIALIZER_H

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
 * @class FOO_Client_ORBInitializer
 *
 * @brief Simple concrete ORB initializer.
 *
 * This is a test ORB initializer that registers an Client for
 * this test.
 */
class FOO_Client_ORBInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual ::CORBA::LocalObject
{
public:
  FOO_Client_ORBInitializer () = default;
  FOO_Client_ORBInitializer (FOO_Client_ORBInitializer const &) = delete;
  FOO_Client_ORBInitializer (FOO_Client_ORBInitializer &&) = delete;
  FOO_Client_ORBInitializer &operator= (FOO_Client_ORBInitializer const &) = delete;
  FOO_Client_ORBInitializer &operator= (FOO_Client_ORBInitializer &&) = delete;

  /**
   * @name Methods Required by the ORBInitializer Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all ORB initializers.
   */
  //@{
  /// The pre-initialization hook.
  void pre_init (PortableInterceptor::ORBInitInfo_ptr info) override;

  /// The post-initialization hook.
  void post_init (PortableInterceptor::ORBInitInfo_ptr info) override;
  //@}
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* FOO_CLIENT_ORB_INITIALIZER_H */
