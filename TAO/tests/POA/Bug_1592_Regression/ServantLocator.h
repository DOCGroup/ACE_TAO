// -*- C++ -*-

//=============================================================================
/**
 * @file ServantLocator.h
 *
 * Implementation header for the PortableServer::ServantLocator for
 * the PortableInterceptor / ServantLocator test.
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef SERVANT_LOCATOR_H
#define SERVANT_LOCATOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantLocatorC.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/LocalObject.h"
#include "tao/ORB.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class ServantLocator
 *
 * @brief Test PortableServer::ServantLocator.
 *
 * PortableServer::ServantLocator used for this test.
 */
class ServantLocator
  : public virtual PortableServer::ServantLocator,
    public virtual ::CORBA::LocalObject
{
public:
  explicit ServantLocator (CORBA::ORB_ptr orb);

  ServantLocator (ServantLocator const &) = delete;
  ServantLocator &operator= (ServantLocator const &) = delete;
  ServantLocator (ServantLocator &&) = delete;
  ServantLocator &operator= (ServantLocator &&) = delete;

  PortableServer::Servant preinvoke (
    PortableServer::ObjectId const &oid,
    PortableServer::POA_ptr adapter,
    char const *operation,
    PortableServer::ServantLocator::Cookie &the_cookie) override;

  void postinvoke (
    PortableServer::ObjectId const &oid,
    PortableServer::POA_ptr adapter,
    char const *operation,
    PortableServer::ServantLocator::Cookie the_cookie,
    PortableServer::Servant the_servant) override;

  bool error_status_ {};
protected:
  ~ServantLocator () override = default;

private:
  /// Pseudo-reference to the ORB.
  CORBA::ORB_var orb_;

  /// Pointer to the test servant.
  PortableServer::ServantBase_var servant_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* SERVANT_LOCATOR_H */
