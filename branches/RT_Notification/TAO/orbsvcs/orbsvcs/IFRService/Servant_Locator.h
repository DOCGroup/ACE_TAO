// $Id$

//==========================================================================
//
// = LIBRARY
//     TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//     Servant_Locator.h
//
// = DESCRIPTION
//     Defines a servant activator subclass, used by the IFR and its POA
//
// = AUTHOR
//     Jeff Parsons
//
//==========================================================================

#ifndef IFR_SERVANT_LOCATOR_H
#define IFR_SERVANT_LOCATOR_H

#include "tao/PortableServer/PortableServerC.h"
#include "ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Repository_i;

class TAO_IFRService_Export IFR_ServantLocator : public PortableServer::ServantLocator
{
  // = TITLE
  //    IFR_ServantLocator
  //
  // = DESCRIPTION
  //    This class is used by the Interface Repository to create
  //    servants on demand
  //
public:
  IFR_ServantLocator (TAO_Repository_i *repo);
  // constructor

  virtual PortableServer::Servant preinvoke (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr adapter,
      const char *operation,
      PortableServer::ServantLocator::Cookie &the_cookie
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest));
  // This method is invoked by the IFR's POA whenever it receives a request
  // for an IR object.

  virtual void postinvoke (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr adapter,
      const char *operation,
      PortableServer::ServantLocator::Cookie the_cookie,
      PortableServer::Servant the_servant
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This method is invoked whenever an IR object servant completes a
  // request.


private:
  TAO_Repository_i *repo_;
  // Passed in to each created servant.
};

#endif /* IFR_SERVANT_LOCATOR_H */
