// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file Transport_Connector.h
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
#include /**/ "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CORBA_macros.h"

class ACE_Time_Value;
class TAO_Transport_Descriptor_Interface;
class TAO_InputCDR;
class TAO_Endpoint;
class TAO_GIOP_Invocation;
class TAO_Profile;
class TAO_MProfile;
class TAO_ORB_Core;
class TAO_Connect_Strategy;
class TAO_Transport;

namespace TAO
{
  class Profile_Transport_Resolver;
}
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
   * For example IOP::TAG_INTERNET_IOP == 0.  The tag is used in the
   * IOR to identify the type of profile included. IOR -> {{tag0,
   * profile0} {tag1, profile1} ...}.  The IOP module defines the
   * ProfileId typedef to be a CORBA::ULong.
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

  /**
   * Call is very similar to the previous one but with a timeout.
   */
  virtual int connect (TAO_GIOP_Invocation *invocation,
                       TAO_Transport_Descriptor_Interface *desc,
                       ACE_Time_Value *timeout
                       ACE_ENV_ARG_DECL);

  /**
   * Call is very similar to the previous one but with a timeout.
   */
  virtual TAO_Transport* connect (TAO::Profile_Transport_Resolver *r,
                                  TAO_Endpoint *ep,
                                  ACE_Time_Value *timeout
                                  ACE_ENV_ARG_DECL);

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

  /// Set and validate endpoint. We need to do this to initialize our
  /// remote *_Addr's which have not been done during IOR decode.
  virtual int set_validate_endpoint (TAO_Endpoint *endpoint) = 0;

  /// Do an actual connect using the underlying transport to make a
  /// connection
  virtual int make_connection (TAO_GIOP_Invocation *invocation,
                               TAO_Transport_Descriptor_Interface *desc,
                               ACE_Time_Value *timeout) = 0;

  virtual TAO_Transport* make_connection (TAO::Profile_Transport_Resolver *r,
                                          TAO_Transport_Descriptor_Interface &desc,
                                          ACE_Time_Value *timeout) = 0;

  /// Set the ORB Core pointer
  void orb_core (TAO_ORB_Core *orb_core);

  /// Create a connect strategy
  int create_connect_strategy (void);

  /// Return the TAO_ORB_Core pointer
  TAO_ORB_Core *orb_core (void);

protected:

  /// The (a)synch connect strategy
  TAO_Connect_Strategy *active_connect_strategy_;

private:

  /// IOP protocol tag.
  CORBA::ULong tag_;

  /// Pointer to our ORB core
  TAO_ORB_Core *orb_core_;
};

#if defined (__ACE_INLINE__)
# include "Transport_Connector.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_CONNECTOR_H*/
