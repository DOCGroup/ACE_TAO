// This may look like C, but it's really -*- C++ -*-
// $Id$

// ================================================================
//
// = LIBRARY
//     TAO/examples/POA/Loader
//
// = FILENAME
//     Servant_Locator.h
//
// = DESCRIPTION
//     Defines a ServantLocator class , used with a POA having
//     a NON_RETAIN policy.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ================================================================

#include "ace/OS.h"

#ifndef SERVANT_LOCATOR_H
#define SERVANT_LOCATOR_H

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/DLL.h"

class ServantLocator : public PortableServer::ServantLocator
{
  // = TITLE
  //   This class is used by a POA with USE_SERVANT_MANAGER and
  //   NON_RETAIN policy.
  //
  // = DESCRIPTION
  //   This class defines the Servant Locator interface of the Servant
  //   Manager. It is invoked when the POA has an USE_SERVANT_MANAGER
  //   policy and a servant_retention policy of NON_RETAIN type.
public:
  typedef PortableServer::Servant
           (*SERVANT_FACTORY) (const PortableServer::ObjectId &oid,
                               PortableServer::POA_ptr poa,
                               CORBA::ORB_ptr orb);
  // This typedef is used to typecast the void* obtained when finding
  // a symbol in the dll. Invoking the function pointer obtained would
  // get a servant.

  typedef void
          (*SERVANT_GARBAGE_COLLECTOR) (const PortableServer::ObjectId &oid,
                                        PortableServer::POA_ptr,
                                        PortableServer::Servant servant);
  // This typedef is used to obtain the garbage_collection_function symbol
  // in the dll. Invoking the function pointer obtained would then destroy
  // the servant.

  ServantLocator (CORBA::ORB_ptr orb,
                    const char *dllname,
                    const char *factory_function,
                    const char *garbage_collection_function);
  // Constructor.

  virtual PortableServer::Servant preinvoke (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr adapter,
                                             const char *operation,
                                             PortableServer::ServantLocator::Cookie &the_cookie
                                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest));
  // This method is invoked by a POA whenever it receives a request
  // for test object that is not currently active.  When the POA is
  // created using the NON_RETAIN policy the Active Object Map is not
  // maintained, in other words, an association between the ObjectId
  // and the servant is not maintained. Hence every client request the
  // servant has to be loaded. Note the operation argument. This
  // argument specifies the operation to be invoked on the
  // servant. The cookie helps in marking the servant. This marking is
  // useful while destroying the servant.

  virtual void postinvoke (const PortableServer::ObjectId &oid,
                           PortableServer::POA_ptr adapter,
                           const char *operation,
                           PortableServer::ServantLocator::Cookie the_cookie,
                           PortableServer::Servant the_servant
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This method is invoked whenever a test servant completes a
  // request. As the Servant Loactor interface is used when the POA
  // doesnt maintain the Active Object Map, its necessary to get rid
  // of the servant after the client request has been processed. The
  // appropriate servant is destroyed by verifying the cookie.Again
  // this method is invoked per client request.

private:
  ACE_CString dllname_;
  // The name of the dll containing the servant.

  ACE_CString create_symbol_;
  // The symbol which on getting invoked will give us the servant
  // pointer.

  ACE_DLL dll_;
  // The ACE_DLL object which performs the task of loading the dll
  // and accessing it.

  SERVANT_FACTORY servant_supplier_;
  // The function pointer of factory_function type.

  SERVANT_GARBAGE_COLLECTOR servant_garbage_collector_;
  // The function pointer of garbage_collection_function type.

  CORBA::ORB_var orb_;
  // A reference to the ORB.
};

#endif /* SERVANT_LOCATOR_H */
