// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Naming_Service
//
// = FILENAME
//    Naming_Server.h
//
// = DESCRIPTION
//      This class implements a Naming Server wrapper class which holds
//      a Naming Context for TAO NameService.
//
// = AUTHORS
//    Nagarajan Surendran (naga@cs.wustl.edu)
//
// ============================================================================

#ifndef _NAMING_SERVER_H
#define _NAMING_SERVER_H

#include "tao/corba.h"
#include "orbsvcs/CosNamingC.h"
#include "Naming_Service/CosNaming_i.h"
#include "Naming_Service/Naming_Service.h"

class TAO_Naming_Server
{
  // = TITLE
  //    Defines a wrapper class which holds a Naming Context
  //    implementation for TAO "NamingService" naming context.
  // = DESCRIPTION
  //    This class takes an orb and Poa reference and activates the
  //    NamingService naming context object under that. It also
  //    defines the operator -> so that NamingContext functions like
  //    bind, unbind .. can be called on a NameServer object.
public:
  //Default constructor
  TAO_Naming_Server ();

  // Takes the poa under which to register the Naming Service
  //implementation object
  TAO_Naming_Server (CORBA::ORB_var& orb,PortableServer::POA_var& root_poa);

  // initialize the name server under the given orb and poa.
  int init (CORBA::ORB_var& orb,PortableServer::POA_var& root_poa);

  //Returns the "NameService" NamingContext implementation object reference 
  NS_NamingContext& GetNamingContext ();

  // Returns a NamingContext_ptr
  CosNaming::NamingContext*  operator -> (void) const;

  //Destructor
  ~TAO_Naming_Server ();

private:
  // Naming context implemetation for "NameService"
  NS_NamingContext naming_context_impl_;

  // NamingContext ptr
  CosNaming::NamingContext_ptr naming_context_ptr_;

  // The ior_multicast event handler
  IOR_Multicast *ior_multicast_;
};

#endif
