// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Acceptor_Registry.h
//
// = DESCRIPTION
//     Interface for the TAO pluggable protocol framework.
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ACCEPTOR_REGISTRY_H
#define TAO_ACCEPTOR_REGISTRY_H
#include "ace/pre.h"

#include "tao/Pluggable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"
#include "tao/Resource_Factory.h"

// Forward declarations.
class ACE_Addr;
class ACE_Reactor;
class TAO_ORB_Core;

class TAO_Stub;
class TAO_Profile;
class TAO_MProfile;


typedef TAO_Acceptor** TAO_AcceptorSetIterator;

class TAO_Export TAO_Acceptor_Registry
{
  // = TITLE
  //   Acceptor Registry and Generic Acceptor interface definitions.
  //   All loaded ESIOP or GIOP acceptor bridges must register with
  //   this object.
  //
  // = DESCRIPTION
  //   This class maintains a list os acceptor factories
  //   for all loaded ORB protocols.
  //   There is one Acceptor_Registry per ORB_Core.
public:
  // = Initialization and termination methods.
  TAO_Acceptor_Registry (void);
  //  Default constructor.

  ~TAO_Acceptor_Registry (void);
  //  Default destructor.

  int open (TAO_ORB_Core *orb_core,
            CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Initialize all registered acceptors.  Return -1 on error.

  int close_all (void);
  // Close all open acceptors.

  size_t endpoint_count (void);
  // Returns the total number of endpoints, i.e., the number of
  // profiles that have been created.

  int make_mprofile (const TAO_ObjectKey& object_key,
                     TAO_MProfile &mprofile);
  // Add profiles to the <mprofile> object for all open endpoints.

  int is_collocated (const TAO_MProfile& mprofile);
  // Check if there is at least one profile in <mprofile> that
  // corresponds to a collocated object.

  TAO_Acceptor *get_acceptor (CORBA::ULong tag);
  // Return the acceptor bridges
  
  // = Iterator.
  TAO_AcceptorSetIterator begin (void);
  TAO_AcceptorSetIterator end (void);

private:
  int open_default (TAO_ORB_Core *orb_core,
                    const char *options);
  // Create a default acceptor for all loaded protocols.

  int open_default (TAO_ORB_Core *orb_core,
                    TAO_ProtocolFactorySetItor &factory,
                    const char *options);
  // Create a default acceptor using the specified protocol factory.

private:
  TAO_Acceptor **acceptors_;
  // List of acceptors that are currently open.

  size_t size_;
  // Number of acceptors that are currently open.
};

#if defined(__ACE_INLINE__)
#include "tao/Acceptor_Registry.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ACCEPTOR_REGISTRY_H */
