// $Id$

//==========================================================================
//
// = LIBRARY
//     TAO/orbsvcs/IFR_Service
//
// = FILENAME
//     Servant_Factory.h
//
// = DESCRIPTION
//     Defines a factory class to create the various forms and types
//     of servants.
//
// = AUTHOR
//     Jeff Parsons
//
//==========================================================================

#ifndef IFR_SERVANT_FACTORY_H
#define IFR_SERVANT_FACTORY_H

#include "tao/PortableServer/PortableServerC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Configuration.h"

class TAO_IDLType_i;
class TAO_Contained_i;
class TAO_Container_i;
class TAO_Repository_i;

class IFR_Servant_Factory
{
  // = TITLE
  //    Servant_Factory
  //
  // = DESCRIPTION
  //    This class is used to create _i classes, _tie classes and
  //    object references.
  //
public:
  IFR_Servant_Factory (TAO_Repository_i *repo);
  // constructor

  TAO_IDLType_i *create_idltype (ACE_Configuration_Section_Key key
                                 ACE_ENV_ARG_DECL);
  // For internal use - faster than creating an object reference and
  // sending a request, and can call utility functions not in IDL.

  TAO_Contained_i *create_contained (ACE_Configuration_Section_Key key
                                     ACE_ENV_ARG_DECL);
  // For internal use - faster than creating an object reference and
  // sending a request, and can call utility functions not in IDL.

  TAO_Container_i *create_container (ACE_Configuration_Section_Key key
                                     ACE_ENV_ARG_DECL);
  // For internal use - faster than creating an object reference and
  // sending a request, and can call utility functions not in IDL.

  PortableServer::Servant create_tie (ACE_Configuration_Section_Key key,
                                      PortableServer::POA_ptr poa
                                      ACE_ENV_ARG_DECL);
  // Used by the servant locator to create a servant.

  CORBA::Object_ptr create_objref (CORBA::DefinitionKind def_kind,
                                   const char *obj_id
                                   ACE_ENV_ARG_DECL);
  // Used to create return values.
private:
  TAO_Repository_i *repo_;
  // Reference to the repository, where most of the state is stored.
};

#endif /* IFR_SERVANT_FACTORY_H */
