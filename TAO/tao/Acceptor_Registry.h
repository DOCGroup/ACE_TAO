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

#include "tao/Pluggable.h"
#include "tao/corbafwd.h"
#include "tao/Typecode.h"
#include "ace/Containers_T.h"

// Forward declarations.
class ACE_Addr;
class ACE_Reactor;
class TAO_ORB_Core;

class TAO_Stub;
class TAO_Profile;
class TAO_MProfile;
class TAO_Resource_Factory;

typedef ACE_Unbounded_Set<TAO_Acceptor*>
        TAO_AcceptorSet;

// @@ Fred&Ossama: Even though ye typing fingers may ache the DOC
//    group style (or lack of it) is to call this stuff
//    TAO_AcceptorSetIterator (not Itor).
//    You may want to define this typedef inside the Acceptor_Registry
//    to avoid namespace pollution. This is specially true since you
//    don't export the Iterator (or the set) in the class interface,
//    and it is only used in the implementation.
//
typedef ACE_Unbounded_Set_Iterator<TAO_Acceptor*>
        TAO_AcceptorSetItor;

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
public:
  TAO_Acceptor_Registry (void);
  //  Default constructor.

  ~TAO_Acceptor_Registry (void);
  //  Default destructor.

  // virtual TAO_Acceptor* acceptor (const char* endpoint) = 0;
  // Create a new acceptor from an endpoint specification, if
  // the specification is not recognized then we return 0.
  // Each protocol will be characterized by a prefix, so
  // endpoints will be specified as:
  // "iiop:macarena:0" - IIOP on macarena, port <any>
  // "uiop:/foo/bar"   - GIOP over unix domain, file /foo/bar
  // "iiop:localhost:0" - IIOP, sap_any [any better idea?]
  // "aal5giop:......"   - GIOP over AAL5, how to specify the
  //                       end point?
  //
  // this format is extensible, for example:
  // "rtiiop:macarena:0,15" - real-time IIOP, host, port, prio.
  //

  TAO_Acceptor *get_acceptor (CORBA::ULong tag);
  // Return the acceptor bridge corresponding to tag (IOP).

  int open (TAO_ORB_Core *orb_core);
  // Initialize all registered acceptors.  Return -1 on error.
  // @@ Fred&Ossama: What is the relationship between the ORB_Core and
  //    the Acceptor_Registry? Is there just one per orb core? Should
  //    tbe acceptor registry know which ORB_Core it belongs to?


  int is_collocated (const TAO_MProfile& mprofile);
  // Check if there is at least one profile in <mprofile> that
  // corresponds to a collocated object.

private:

  TAO_AcceptorSet acceptors_;
  // list of acceptors currently open.
};

#endif TAO_ACCEPTOR_REGISTRY_H
