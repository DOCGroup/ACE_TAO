// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file Connector.h
 *
 *  $Id$
 *
 *  Define the interface for the Connector component in TAO's
 *  pluggable protocol framework.
 *
 *  @author  Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================
#ifndef TAO_CONNECTOR_H
#define TAO_CONNECTOR_H
#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class TAO_Transport_Descriptor_Interface;
class TAO_InputCDR;
class TAO_GIOP_Invocation;
class TAO_Profile;
class TAO_MProfile;
class TAO_ORB_Core;
/**
 * @class TAO_Connector
 *
 * @brief Generic Connector interface definitions.
 *
 * Base class for connector bridge object.
 *
 * @@TODO: Need to rename the class as TAO_Transport_Connector.
 */
class TAO_Export TAO_Connector
{
public:

  /// default constructor.
  TAO_Connector (CORBA::ULong tag);

  /// the destructor.
  virtual ~TAO_Connector (void);

  /**
   * The tag identifying the specific ORB transport layer protocol.
   * For example TAO_TAG_IIOP_PROFILE = 0.  The tag is used in the
   * IOR to identify the type of profile included. IOR -> {{tag0,
   * profile0} {tag1, profole1} ...}  GIOP.h defines typedef
   * CORBA::ULong TAO_IOP_Profile_ID;
   */
  CORBA::ULong tag (void) const;

  /// Parse a string containing a URL style IOR and return an
  /// MProfile.
  int make_mprofile (const char *ior,
                     TAO_MProfile &mprofile
                     ACE_ENV_ARG_DECL);

  ///  Initialize object and register with reactor.
  virtual int open (TAO_ORB_Core *orb_core) = 0;

  /// Shutdown Connector bridge and concrete Connector.
  virtual int close (void) = 0;

  /**
   * To support pluggable we need to abstract away the details of the
   * connect ()  method so it can be called from the invocation code
   * independent of the actual transport protocol in use.
   */
  virtual int connect (TAO_GIOP_Invocation *invocation,
                       TAO_Transport_Descriptor_Interface *desc
                       ACE_ENV_ARG_DECL);

  /// Initial set of connections to be established.
  /*
    @@ TODO: Need to be removed from the ORB. People who need this
    could use  _validate_connections () instead.
  virtual int preconnect (const char *preconnections) = 0;
  */

  /// Create a profile for this protocol and initialize it based on the
  /// encapsulation in <cdr>
  virtual TAO_Profile *create_profile (TAO_InputCDR& cdr) = 0;

  /// Check that the prefix of the provided endpoint is valid for use
  /// with a given pluggable protocol.
  virtual int check_prefix (const char *endpoint) = 0;

  /// Return the object key delimiter to use or expect.
  virtual char object_key_delimiter (void) const = 0;

protected:

  /// Create a profile with a given endpoint.
  virtual TAO_Profile *make_profile (ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// Do an actual connect using the underlying transport to make a
  /// connection
  virtual int make_connection (TAO_GIOP_Invocation *invocation,
                               TAO_Transport_Descriptor_Interface *desc) = 0;

  /// Set the ORB Core pointer
  void orb_core (TAO_ORB_Core *orb_core);

  /// Return the TAO_ORB_Core pointer
  TAO_ORB_Core *orb_core (void);

private:

  /// IOP protocol tag.
  CORBA::ULong tag_;

  /// Pointer to our ORB core
  TAO_ORB_Core *orb_core_;

};

#if defined (__ACE_INLINE__)
# include "Transport_Connector.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_CONNECTOR_H*/
