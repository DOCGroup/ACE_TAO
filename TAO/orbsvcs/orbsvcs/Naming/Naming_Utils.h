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

#ifndef TAO_NAMING_UTILS_H
#define TAO_NAMING_UTILS_H

#include "tao/corba.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/IOR_Multicast.h"
#include "Naming_Context.h"

class TAO_ORBSVCS_Export TAO_Naming_Server
{
  // = TITLE
  //
  //    Defines a wrapper class that holds the root Naming Context.
  //
  // = DESCRIPTION
  //
  //    This class either finds an existing Naming Service or creates
  //    one.  It also defines the operator-> so that <NamingContext>
  //    functions like <bind>, <unbind> .. can be called on a
  //    <NameServer> object.  This class is intended to simplify
  //    programs that want to play the role of a Naming Service
  //    servers.  To simplify programs that want to play the role of
  //    Naming Service clients, use <TAO_Naming_Client>.
public:
  TAO_Naming_Server (void);
  // Default constructor.

  TAO_Naming_Server (CORBA::ORB_ptr orb,
                     PortableServer::POA_ptr poa,
                     size_t context_size = ACE_DEFAULT_MAP_SIZE,
                     ACE_Time_Value *timeout = 0,
                     int resolve_for_existing_naming_service = 1);
  // Either find an existing Naming Service or creates one.  Takes the
  // POA under which to register the Naming Service implementation
  // object.

  int init (CORBA::ORB_ptr orb,
            PortableServer::POA_ptr poa,
            size_t context_size = ACE_DEFAULT_MAP_SIZE,
            ACE_Time_Value *timeout = 0,
            int resolve_for_existing_naming_service = 1);
  // Either find an existing Naming Service or creates one.  Takes the
  // POA under which to register the Naming Service implementation
  // object.

  ~TAO_Naming_Server (void);
  // Destructor.

  TAO_Naming_Context &get_naming_context (void);
  // Returns the "NameService" NamingContext implementation object
  // reference.

  CORBA::String naming_service_ior (void);
  // Returns the IOR of the naming service.

  CosNaming::NamingContext_ptr operator-> (void) const;
  // Returns a <NamingContext_ptr>.

protected:
  int init_new_naming (CORBA::ORB_ptr orb,
                       PortableServer::POA_ptr root_poa,
                       size_t context_size);
  // Initialize a new name server under the given ORB and POA.

  TAO_Naming_Context *naming_context_impl_;
  // Naming context implementation for "NameService".

  CosNaming::NamingContext_var naming_context_;
  // NamingContext ptr.

  TAO_IOR_Multicast *ior_multicast_;
  // The ior_multicast event handler.

  CORBA::String_var naming_service_ior_;
  // The IOR string of the naming service.
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
  //    <resolve>, etc. can be called on a <NameServer> object.  This
  //    class is intended to simplify programs that want to play the
  //    role of a Naming Service clients.
public:
  // = Initialization and termination methods.
  TAO_Naming_Client (void);
  // Default constructor.

  int init (CORBA::ORB_ptr orb,
            ACE_Time_Value *timeout = 0);
  // Initialize the name server.

  ~TAO_Naming_Client (void);
  // Destructor.

  CosNaming::NamingContext_ptr operator-> (void) const;
  // Returns a <NamingContext_ptr>.

  CosNaming::NamingContext_ptr get_context (void) const;
  // Returns the NamingContext

protected:
  CosNaming::NamingContext_var naming_context_;
  // NamingContext ptr.
};

#endif /* TAO_NAMING_UTILS_H */
