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
  // = Initialization and termination methods.
  TAO_Acceptor_Registry (void);
  //  Default constructor.

  ~TAO_Acceptor_Registry (void);
  //  Default destructor.

  // @@ Fred, should this method be removed?  If so, where should the
  // comments be put?

  // virtual TAO_Acceptor *acceptor (const char *endpoint) = 0;
  // Create a new acceptor from an endpoint specification, if
  // the specification is not recognized then we return 0.
  // Each protocol will be characterized by a prefix, so
  // endpoints will be specified as:
  // "iiop://macarena:0" - IIOP on macarena, port <any>
  // "uiop:///foo/bar"   - GIOP over unix domain, file /foo/bar
  // "iiop://localhost:0" - IIOP, sap_any [any better idea?]
  // "aal5giop:......"   - GIOP over AAL5, how to specify the
  //                       end point?
  // this format is extensible, for example:
  // "rtiiop://macarena:0,15" - real-time IIOP, host, port, prio.

  int open (TAO_ORB_Core *orb_core);
  // Initialize all registered acceptors.  Return -1 on error.
  // @@ Fred&Ossama: What is the relationship between the ORB_Core and
  // the Acceptor_Registry? Is there just one per orb core? Should tbe
  // acceptor registry know which ORB_Core it belongs to?

  int close_all (void);
  // Close all open acceptors.

  size_t endpoint_count (void);
  // Returns the total number of endpoints, i.e., the number of
  // profiles that will be created.

  int make_mprofile (const TAO_ObjectKey& object_key,
                     TAO_MProfile &mprofile);
  // Add profiles to the <mprofile> object for all open endpoints.

  int is_collocated (const TAO_MProfile& mprofile);
  // Check if there is at least one profile in <mprofile> that
  // corresponds to a collocated object.

  // = Iterator.
  TAO_AcceptorSetItor begin (void);
  TAO_AcceptorSetItor end (void);

private:
  int open_default (TAO_ORB_Core *orb_core,
                    ACE_CString *protocol_prefix = 0);
  // Create an acceptor with a default endpoint.

private:
  TAO_AcceptorSet acceptors_;
  // List of acceptors that are currently open.
};

#if defined(__ACE_INLINE__)
#include "tao/Acceptor_Registry.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_ACCEPTOR_REGISTRY_H */
