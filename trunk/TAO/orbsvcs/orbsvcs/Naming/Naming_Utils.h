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
//      This class implements wrapper classes for clients and servers
//      of the Naming Service.
//
// = AUTHORS
//    Nagarajan Surendran (naga@cs.wustl.edu), Matt Braun
//    <mjb2@cs.wustl.edu>, and Douglas C. Schmidt <schmidt@cs.wustl.edu>.
//
// ============================================================================

#if !defined (TAO_NAMING_UTILS_H)
#define TAO_NAMING_UTILS_H

#include "tao/corba.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/IOR_Multicast.h"
#include "CosNaming_i.h"

class TAO_ORBSVCS_Export TAO_Naming_Server
{
  // = TITLE
  //    Defines a wrapper class that holds a Naming Context
  //    implementation for a TAO "NamingService" naming context.
  //
  // = DESCRIPTION
  //    This class takes an ORB and POA reference and activates the
  //    NamingService naming context object under that.  It also
  //    defines the operator-> so that <NamingContext> functions like
  //    <bind>, <unbind> .. can be called on a <NameServer> object.
  //    This class is intended to simplify programs that want to play
  //    the role of a Naming Service servers.  To simplify programs
  //    that want to play the role of Naming Service clients, use
  //    <TAO_Naming_Client>.
public:
  // = Initialization and termination methods.

  TAO_Naming_Server (void);
  //Default constructor.

  TAO_Naming_Server (CORBA::ORB_ptr orb,
                     PortableServer::POA_ptr root_poa,
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

  ~TAO_Naming_Server (void);
  // Destructor.

  NS_NamingContext &get_naming_context (void);
  // Returns the "NameService" NamingContext implementation object
  // reference.

  CORBA::String naming_service_ior (void);
  // Returns the IOR of the naming service.

  CosNaming::NamingContext_ptr operator-> (void) const;
  // Returns a <NamingContext_ptr>.

private:
  NS_NamingContext naming_context_impl_;
  // Naming context implemetation for "NameService".

  CosNaming::NamingContext_var naming_context_;
  // NamingContext ptr.

  TAO_IOR_Multicast *ior_multicast_;
  // The ior_multicast event handler.

  CORBA::String_var naming_service_ior_;
  // The IOR string of the naming service.

  CORBA::String_var naming_context_name_;
  // Name of the naming context (if different than
  // "NameService").
};

class TAO_ORBSVCS_Export TAO_Naming_Client
{
  // = TITLE
  //    Defines a wrapper class that simplifies initialization and
  //    access to a <NamingContext>.
  //
  // = DESCRIPTION
  //    This class takes an ORB reference and contacts the
  //    NamingService naming context object under that.  It also
  //    defines the operator-> so that <NamingContext> functions like
  //    <resolve>, etc. can be called on a <NameServer> object.
  //    This class is intended to simplify programs that want to play
  //    the role of a Naming Service clients.

public:
  // = Initialization and termination methods.

  TAO_Naming_Client (void);
  //Default constructor.

  int init (CORBA::ORB_ptr orb,
	    int argc = 0,
            char **argv = 0);
  // Initialize the name server under the given ORB and POA.  The
  // <argc> and <argv> commmand-line arguments are parsed to determine
  // if this name server instance is part of a naming tree that
  // resides under the default name server.

  ~TAO_Naming_Client (void);
  // Destructor.

  CosNaming::NamingContext_ptr operator-> (void) const;
  // Returns a <NamingContext_ptr>.

private:
  CosNaming::NamingContext_var naming_context_;
  // NamingContext ptr.
};

#endif /* TAO_NAMING_UTILS_H */
