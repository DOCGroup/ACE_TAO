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
#include "ace/pre.h"

#include "tao/Pluggable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"
#include "tao/params.h"

// Forward declarations.
class ACE_Addr;
class ACE_Reactor;
class TAO_ORB_Core;

class TAO_Stub;
class TAO_MProfile;
class TAO_Resource_Factory;

typedef TAO_Connector** TAO_ConnectorSetIterator;

class TAO_Export TAO_Connector_Registry
{
  // = TITLE
  //   Per ORB TAO Connector Registry
  //
  // = DESCRIPTION
  //   Connector Registry and Generic Connector interface definitions.
  //   All loaded ESIOP or GIOP connector bridges must register with
  //   this object.
  //   This class is able to dynamically load a set of
  //   concrete protocol connectors which have registered with the service
  //   configurator and added their Factory name to the Resource_Factory
  //   line of the svc.comf file.
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

  int preconnect (TAO_ORB_Core *orb_core,
                  TAO_EndpointSet &preconnections);
  // For this list of preconnections call the connector specific
  // preconnect method for each preconnection.

  int connect (TAO_Endpoint *endpoint,
               TAO_Transport *&transport,
               ACE_Time_Value *max_wait_time,
               CORBA::Environment &ACE_TRY_ENV);
  // This is where the transport protocol is selected based on some
  // policy.  This member will call the connect member of the
  // TAO_Connector class which in turn will call the concrete
  // connector.

  int make_mprofile (const char *ior,
                     TAO_MProfile &mprofile,
                     CORBA::Environment &ACE_TRY_ENV);
  // Parse a string containing a URL style IOR and return an
  // MProfile.

  TAO_Profile* create_profile (TAO_InputCDR& cdr);
  // Create a profile based on the contents of <cdr>

  char object_key_delimiter (const char *ior);
  // Obtain the object key delimiter used by the protocol specified in
  // the provided URL style IOR.

  // = Iterator.
  TAO_ConnectorSetIterator begin (void);
  TAO_ConnectorSetIterator end (void);

private:
  int preprocess_preconnects (TAO_ORB_Core *orb_core,
                              TAO_EndpointSet &preconnects);
  // Put the preconnects in a form that makes it simple for protocol
  // implementers to parse.

  // Prohibited
  ACE_UNIMPLEMENTED_FUNC (TAO_Connector_Registry (const TAO_Connector_Registry&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Connector_Registry&))

private:
  TAO_Connector** connectors_;
  // List of connectors that are currently open.

  size_t size_;
  // Number of connectors that are currently open.
};

#if defined(__ACE_INLINE__)
#include "tao/Connector_Registry.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CONNECTOR_REGISTRY_H */
