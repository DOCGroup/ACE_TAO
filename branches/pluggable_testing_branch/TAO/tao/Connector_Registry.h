// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Connector_Registry.h
//
// = DESCRIPTION
//     Interface for the TAO Connector Registry
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_CONNECTOR_REGISTRY_H
#define TAO_CONNECTOR_REGISTRY_H

#include "tao/Pluggable.h"
#include "tao/corbafwd.h"
#include "tao/Typecode.h"
#include "tao/params.h"

#include "ace/Containers_T.h"

// Forward declarations.
class ACE_Addr;
class ACE_Reactor;
class TAO_ORB_Core;

class TAO_Stub;
class TAO_MProfile;
class TAO_Resource_Factory;

typedef ACE_Unbounded_Set<TAO_Connector*>
        TAO_ConnectorSet;

typedef ACE_Unbounded_Set_Iterator<TAO_Connector*>
        TAO_ConnectorSetItor;


class TAO_Export TAO_Connector_Registry
{
  // = TITLE
  //   Connector Registry and Generic Connector interface definitions.
  //   All loaded ESIOP or GIOP connector bridges must register with
  //   this object.
  //
  // = DESCRIPTION
  //   @@ Fred: this class should be able to dynamically load a set of
  //            connectors using the service configurator.
public:
  TAO_Connector_Registry (void);
  //  Default constructor.

  ~TAO_Connector_Registry (void);
  //  Default destructor.

   TAO_Connector *get_connector (CORBA::ULong tag);
  // Return the connector bridges

  int open (TAO_ORB_Core *orb_core);
  // Initialize all registered connectors.

  int close_all (void);
  // Close all open connectors.

  int preconnect (TAO_EndpointSet &preconnections);
  // For this list of preconnections call the connector specific
  // preconnect method for each preconnection.

  int connect (TAO_Stub *&obj, TAO_Transport *&);
  // This is where the transport protocol is selected based on some
  // policy.  This member will call the connect member of the
  // TAO_Connector class which in turn will call the concrete
  // connector.

  int make_mprofile (const char *ior,
                     TAO_MProfile &mprofile,
                     CORBA::Environment &ACE_TRY_ENV);
  // Parse a string containing a URL style IOR and return an
  // MProfile.

private:
  TAO_ConnectorSet connectors_;
  // list of connectors currently open.
};

#endif TAO_CONNECTOR_REGISTRY_H
