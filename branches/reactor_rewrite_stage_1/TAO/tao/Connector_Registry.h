// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file     Connector_Registry.h
 *
 *  $Id$
 *
 *   Interface for the TAO Connector Registry
 *
 *
 *  @author  Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================


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

class TAO_Transport_Descriptor_Interface;
class TAO_Stub;
class TAO_MProfile;
class TAO_Resource_Factory;
class TAO_GIOP_Invocation;

typedef TAO_Connector** TAO_ConnectorSetIterator;

/**
 * @class TAO_Connector_Registry
 *
 * @brief Per-ORB TAO Connector Registry
 *
 * Connector Registry and Generic Connector interface definitions.
 * All loaded ESIOP or GIOP connector bridges must register with
 * this object.
 * This class is able to dynamically load a set of
 * concrete protocol connectors which have registered with the service
 * configurator and added their Factory name to the Resource_Factory
 * line of the svc.conf file.
 */
class TAO_Export TAO_Connector_Registry
{
public:
  ///  Default constructor.
  TAO_Connector_Registry (void);

  ///  Default destructor.
  ~TAO_Connector_Registry (void);

  /// Return the connector bridges
   TAO_Connector *get_connector (CORBA::ULong tag);

  /// Initialize all registered connectors.
  int open (TAO_ORB_Core *orb_core);

  /// Close all open connectors.
  int close_all (void);

  /// For this list of preconnections call the connector specific
  /// preconnect method for each preconnection.
  int preconnect (TAO_ORB_Core *orb_core,
                  TAO_EndpointSet &preconnections);

  /**
   * This is where the transport protocol is selected based on some
   * policy.  This member will call the connect member of the
   * TAO_Connector class which in turn will call the concrete
   * connector.
   */
  int connect (TAO_GIOP_Invocation *invocation,
               TAO_Transport_Descriptor_Interface *desc
               TAO_ENV_ARG_DECL);

  /// Parse a string containing a URL style IOR and return an
  /// MProfile.
  int make_mprofile (const char *ior,
                     TAO_MProfile &mprofile
                     TAO_ENV_ARG_DECL);

  /// Create a profile based on the contents of <cdr>
  TAO_Profile* create_profile (TAO_InputCDR& cdr);

  /// Obtain the object key delimiter used by the protocol specified in
  /// the provided URL style IOR.
  char object_key_delimiter (const char *ior);

  // = Iterator.
  TAO_ConnectorSetIterator begin (void);
  TAO_ConnectorSetIterator end (void);

private:
  /// Put the preconnects in a form that makes it simple for protocol
  /// implementers to parse.
  int preprocess_preconnects (TAO_ORB_Core *orb_core,
                              TAO_EndpointSet &preconnects);

  // Prohibited
  ACE_UNIMPLEMENTED_FUNC (TAO_Connector_Registry (const TAO_Connector_Registry&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Connector_Registry&))

private:
  /// List of connectors that are currently open.
  TAO_Connector** connectors_;

  /// Number of connectors that are currently open.
  size_t size_;
};

#if defined(__ACE_INLINE__)
#include "tao/Connector_Registry.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CONNECTOR_REGISTRY_H */
