/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Naming_Service
//
// = FILENAME
//    Naming_Utils.h
//
// = DESCRIPTION
//      This class implements a Naming Server wrapper class which holds
//      a Naming Context for TAO NameService.
//
// = AUTHORS
//    Nagarajan Surendran (naga@cs.wustl.edu)
//
// ============================================================================

#if !defined (_NAMING_SERVER_H)
#define _NAMING_SERVER_H

#include "tao/corba.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/IOR_Multicast.h"
#include "CosNaming_i.h"

class TAO_ORBSVCS_Export TAO_Naming_Server
{
  // = TITLE
  //    Defines a wrapper class which holds a Naming Context
  //    implementation for TAO "NamingService" naming context.
  //
  // = DESCRIPTION
  //    This class takes an orb and Poa reference and activates the
  //    NamingService naming context object under that. It also
  //    defines the operator -> so that NamingContext functions like
  //    bind, unbind .. can be called on a NameServer object.
public:
  TAO_Naming_Server (void);
  //Default constructor.

  TAO_Naming_Server (CORBA::ORB_var &orb,
                     PortableServer::POA_var &root_poa,
		     int argc = 0,
                     char **argv = 0);
  // Takes the POA under which to register the Naming Service
  // implementation object.  The <argc> and <argv> commmand-line
  // arguments are parsed to determine if this name server instance is
  // part of a naming tree that resides under the default name server.

  int init (CORBA::ORB_ptr orb,
            PortableServer::POA_ptr root_poa,
	    int argc = 0,
            char **argv = 0);
  // Initialize the name server under the given ORB and POA.  The
  // <argc> and <argv> commmand-line arguments are parsed to determine
  // if this name server instance is part of a naming tree that
  // resides under the default name server.

  NS_NamingContext &GetNamingContext (void);
  // Returns the "NameService" NamingContext implementation object
  // reference.

  CORBA::String naming_service_ior (void);
  // Returns the ior of the naming service.

  CosNaming::NamingContext_ptr operator -> (void) const;
  // Returns a NamingContext_ptr.

  ~TAO_Naming_Server (void);
  // Destructor.

private:
  NS_NamingContext naming_context_impl_;
  // Naming context implemetation for "NameService".

  CosNaming::NamingContext_var naming_context_var_;
  // NamingContext ptr.

  TAO_IOR_Multicast *ior_multicast_;
  // The ior_multicast event handler.

  CORBA::String_var naming_service_ior_;
  // The IOR string of the naming service.

  CORBA::String_var naming_context_name_;
  // Name of the naming context (if different than
  // "NameService").
};

#endif /* _NAMING_SERVER_H */
