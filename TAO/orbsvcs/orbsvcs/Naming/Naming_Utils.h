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
//      Implement wrappers useful to Naming Service clients and servers.
//
// = AUTHORS
//    Nagarajan Surendran (naga@cs.wustl.edu), Matt Braun
//    <mjb2@cs.wustl.edu>, and Douglas C. Schmidt <schmidt@cs.wustl.edu>.
//
// ============================================================================

#ifndef TAO_NAMING_UTILS_H
#include "ace/pre.h"
#define TAO_NAMING_UTILS_H

#include "tao/corba.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/IOR_Multicast.h"
#include "Naming_Context_Interface.h"

// Forward decl;
class TAO_Persistent_Context_Index;

class TAO_Naming_Export TAO_Naming_Server
{
  // = TITLE
  //    Defines a wrapper class that holds the root Naming Context.
  //
  // = DESCRIPTION
  //    This class either finds an existing Naming Service (if the
  //    <resolve_for_existing_naming_service> flag is set) or creates
  //    one (if <resolve_for_existing_naming_service> flag isn't set or
  //    Naming Service was not found).  This class also defines the
  //    operator-> so that  <NamingContext> functions like <bind>,
  //    <unbind> .. can be called directly on a <TAO_Naming_Server>
  //    object, and be forwareded to the root Naming Context.
  //    This class is intended to simplify
  //    programs that want to play the role of a Naming Service
  //    server.  To simplify programs that want to play the role of
  //    Naming Service clients, use <TAO_Naming_Client>.
  //
  //    If a Naming Service is created locally, a TAO_IOR_Multicast
  //    event handler is created and installed on the ORB's reactor.
  //    This event handler allows other clients on the network to
  //    discover and use this Naming Service.
  //    Event handler listens on a multicast port for messages from
  //    clients looking for a Naming Service, and sends back the ior
  //    of the root Naming Context.  For more information on how this
  //    bootstraping through a multicast process works, check out
  //    orbsvcs/orbsvcs/TAO_IOR_Multicast.*, implementation of
  //    <resolve_initial_references>, and orbsvcs/Naming/README.
public:
  TAO_Naming_Server (void);
  // Default constructor.

  TAO_Naming_Server (CORBA::ORB_ptr orb,
                     PortableServer::POA_ptr poa,
                     size_t context_size = ACE_DEFAULT_MAP_SIZE,
                     ACE_Time_Value *timeout = 0,
                     int resolve_for_existing_naming_service = 1,
                     LPCTSTR persistence_location = 0);
  // Constructor.  Attempts to find an existing Naming Service if
  // <resolve_for_existing_naming_service> is set to true.  If it is
  // false, or no Naming Service was found during a <timeout> period,
  // create the Naming Service in this process.  If creating the
  // Naming Service locally, make the root context of size
  // <context_size>, register it under the <poa>, and make the Naming
  // Service persistent if <persistence_location> is not 0.
  // (<persistence_location> specifies name of the file to use for
  // persistent storage).

  int init (CORBA::ORB_ptr orb,
            PortableServer::POA_ptr poa,
            size_t context_size = ACE_DEFAULT_MAP_SIZE,
            ACE_Time_Value *timeout = 0,
            int resolve_for_existing_naming_service = 1,
            LPCTSTR persistence_location = 0);
  // Initializer.  Attempts to find an existing Naming Service if
  // <resolve_for_existing_naming_service> is set to true.  If it is
  // false, or no Naming Service was found during a <timeout> period,
  // create the Naming Service in this process.  If creating the
  // Naming Service locally, make the root context of size
  // <context_size>, register it under the <poa>, and make the Naming
  // Service persistent if <persistence_location> is not 0.
  // (<persistence_location> specifies name of the file to use for
  // persistent storage).

  ~TAO_Naming_Server (void);
  // Destructor.

  char * naming_service_ior (void);
  // Returns the IOR of the naming service.

  CosNaming::NamingContext_ptr operator-> (void) const;
  // Returns a <NamingContext_ptr> for the root Naming Context.

protected:
  int init_new_naming (CORBA::ORB_ptr orb,
                       PortableServer::POA_ptr root_poa,
                       LPCTSTR persistence_location,
                       size_t context_size);
  // Helper method: create Naming Service locally.
  // Make the root context of size
  // <context_size>, register it under the <root_poa>, and make the Naming
  // Service persistent if <persistence_location> is not 0.
  // (<persistence_location> specifies name of the file to use for
  // persistent storage).

  CosNaming::NamingContext_var naming_context_;
  // Root NamingContext_ptr.

  TAO_IOR_Multicast *ior_multicast_;
  // The ior_multicast event handler.

  CORBA::String_var naming_service_ior_;
  // The IOR string of the root naming context.

  TAO_Persistent_Context_Index *context_index_;
  // Pointer to the object used to create/initialize
  // the Naming Service when local persistent Naming Service is
  // desired.
};

class TAO_Naming_Export TAO_Naming_Client
{
  // = TITLE
  //    This class is intended to simplify programs that want to play
  //    the role of Naming Service clients.
  //
  // = DESCRIPTION
  //    Wrapper class that locates the root Naming Context.  It also
  //    defines the operator-> so that <NamingContext> functions like
  //    <resolve>, etc. can be directly called on a
  //    <TAO_Naming_Client> object, and will be forwarded to the root
  //    Naming Context.
public:
  // = Initialization and termination methods.

  TAO_Naming_Client (void);
  // Default constructor.

  int init (CORBA::ORB_ptr orb,
            ACE_Time_Value *timeout = 0);
  // Look for a Naming Service for a period of <timeout> using
  // <resolve_initial_references>.  Return 0 if Naming Service is
  // successfully located, and -1 on failure.

  ~TAO_Naming_Client (void);
  // Destructor.

  CosNaming::NamingContext_ptr operator-> (void) const;
  // Returns a pointer to the root Naming Context.

  CosNaming::NamingContext_ptr get_context (void) const;
  // Returns a pointer to the root Naming Context.  This ptr is
  // duplicated via <_duplicate> so that it can be stored into a
  // <*_var>.

protected:
  CosNaming::NamingContext_var naming_context_;
  // Reference to the root Naming Context.
};

#include "ace/post.h"
#endif /* TAO_NAMING_UTILS_H */
