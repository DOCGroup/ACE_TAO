// -*- C++ -*-

//=============================================================================
/**
 *  @file    Connector_Registry.h
 *
 *  $Id$
 *
 *  Interface for the TAO Connector Registry
 *
 *  @author  Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_CONNECTOR_REGISTRY_H
#define TAO_CONNECTOR_REGISTRY_H
#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "ace/Global_Macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations.
class TAO_ORB_Core;
class TAO_Connector;
class TAO_MProfile;
class TAO_Profile;
class TAO_InputCDR;

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
  TAO_Connector *get_connector (CORBA::ULong tag) const;

  /// Initialize all registered connectors.
  int open (TAO_ORB_Core *orb_core);

  /// Close all open connectors.
  int close_all (void);

  /// Parse a string containing a URL style IOR and return an
  /// MProfile.
  int make_mprofile (const char *ior,
                     TAO_MProfile &mprofile
                    );

  /// Create a profile based on the contents of @a cdr
  TAO_Profile* create_profile (TAO_InputCDR& cdr);

  /// Obtain the object key delimiter used by the protocol specified in
  /// the provided URL style IOR.
  char object_key_delimiter (const char *ior);

  // = Iterator.
  TAO_ConnectorSetIterator begin (void) const;
  TAO_ConnectorSetIterator end (void) const;

private:
  // Disallow copying and assignment.
  TAO_Connector_Registry (const TAO_Connector_Registry&);
  void operator= (const TAO_Connector_Registry&);

private:
  /// List of connectors that are currently open.
  TAO_Connector** connectors_;

  /// Number of connectors that are currently open.
  size_t size_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "tao/Connector_Registry.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_CONNECTOR_REGISTRY_H */
