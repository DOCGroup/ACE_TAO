// $Id$

// ================================================================
//
// = LIBRARY
//     TAO/tests/POA/Forwarding/Servant_Locator
//
// = FILENAME
//     Servant_Locator.h
//
// = DESCRIPTION
//     Defines a MyFooServantLocator class , used with a POA having
//     a NON_RETAIN policy
//
// = AUTHOR
//     Irfan Pyarali
//
// ================================================================

#ifndef _SERVANT_LOCATOR_H_
#define _SERVANT_LOCATOR_H_

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"

class Servant_Locator : public PortableServer::ServantLocator
{
  // = TITLE
  //   This class is used by a POA with USE_SERVANT_MANAGER and
  //   NON_RETAIN policy.
public:
  Servant_Locator (CORBA::ORB_ptr orb_ptr);

  // constructor

  ~Servant_Locator ();

  virtual PortableServer::Servant preinvoke (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr adapter,
                                             const char *operation,
                                             PortableServer::ServantLocator::Cookie &the_cookie
                                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest));
  // This method is invoked by a POA whenever it receives a request
  // for MyFoo object that is not currently active.

  virtual void postinvoke (const PortableServer::ObjectId &oid,
                           PortableServer::POA_ptr adapter,
                           const char *operation,
                           PortableServer::ServantLocator::Cookie the_cookie,
                           PortableServer::Servant the_servant
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This method is invoked whenever a MyFooServant completes a
  // request.

  void set (CORBA::Object_var obj);
  // To be removed..

 private:
  CORBA::ORB_var orb_var_;

  CORBA::Object_var objref_;
  // To be removed
};

#endif /* _SERVANT_LOCATOR_H */
