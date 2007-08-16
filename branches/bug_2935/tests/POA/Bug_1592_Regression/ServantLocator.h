// -*- C++ -*-

//=============================================================================
/**
 * @file ServantLocator.h
 *
 * $Id$
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
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  ServantLocator (CORBA::ORB_ptr orb);

  virtual PortableServer::Servant preinvoke (
      const PortableServer::ObjectId & oid,
      PortableServer::POA_ptr adapter,
      const char * operation,
      PortableServer::ServantLocator::Cookie & the_cookie);

  virtual void postinvoke (
      const PortableServer::ObjectId & oid,
      PortableServer::POA_ptr adapter,
      const char * operation,
      PortableServer::ServantLocator::Cookie the_cookie,
      PortableServer::Servant the_servant);

  bool error_status_;
protected:

  /// Destructor.
  ~ServantLocator (void);

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
