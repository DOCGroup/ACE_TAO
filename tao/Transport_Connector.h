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
#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/TAO_Export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Time_Value;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Transport_Descriptor_Interface;
class TAO_InputCDR;
class TAO_Endpoint;
class TAO_Profile;
class TAO_MProfile;
class TAO_ORB_Core;
class TAO_Connect_Strategy;
class TAO_Transport;
class TAO_Connection_Handler;
class TAO_LF_Multi_Event;

namespace TAO
{
  class Profile_Transport_Resolver;
}

/*
 * Hook to add includes and forward declaration
 * to the Connector class.
 */
//@@ TAO_CONNECTOR_SPL_INCLUDE_FORWARD_DECL_ADD_HOOK

/**
 * @class TAO_Connector
 *
 * @brief Generic Connector interface definitions.
 *
 * Base class for connector bridge object.
 *
 * @todo Need to rename the class as TAO_Transport_Connector.
 */
class TAO_Export TAO_Connector
{
public:

  /// Default constructor.
  TAO_Connector (CORBA::ULong tag);

  /// The destructor.
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
  /// MProfile.  Verify that ior is in the correct format.
  int make_mprofile (const char *ior, TAO_MProfile &mprofile);

  /// Helper function to assist corbaloc parsing. The default simply validates
  /// the protocol identifier and scans up to the next comma or slash.
  /// Any protocol that has a specific need, such as uiop, can override this
  /// method to  provide a custom scanner.
  /// The profile returned is either null if this the ior does not match or an
  /// empty profile of the correct type, obtained from make_profile().
  virtual TAO_Profile *corbaloc_scan (const char *ior, size_t &len);

  ///  Initialize object and register with reactor.
  virtual int open (TAO_ORB_Core *orb_core) = 0;

  /// Shutdown Connector bridge and concrete Connector.
  virtual int close (void) = 0;

  /**
   * To support pluggable we need to abstract away the details of the
   * connect ()  method so it can be called from the invocation code
   * independent of the actual transport protocol in use.
   */
  virtual TAO_Transport *connect (TAO::Profile_Transport_Resolver *r,
                                  TAO_Transport_Descriptor_Interface *desc,
                                  ACE_Time_Value *timeout);

  /// A variation on connect that will try simultaneous connections
  /// on all endpoints listed in the desc.
  virtual TAO_Transport *parallel_connect (TAO::Profile_Transport_Resolver *r,
                                           TAO_Transport_Descriptor_Interface
                                           *desc, ACE_Time_Value *timeout);

  /// Create a profile for this protocol and initialize it based on the
  /// encapsulation in @a cdr
  virtual TAO_Profile *create_profile (TAO_InputCDR &cdr) = 0;

  /// Check that the prefix of the provided endpoint is valid for use
  /// with a given pluggable protocol.
  virtual int check_prefix (const char *endpoint) = 0;

  /// Return the object key delimiter to use or expect.
  virtual char object_key_delimiter (void) const = 0;

  //@@ TAO_CONNECTOR_SPL_PUBLIC_METHODS_ADD_HOOK

protected:
  /// A flag indicating the actual connector supports parallel connection
  /// attempts. The base implementation always returns 0. Override to return
  /// non-zero if parallel connection attempts may be tried.
  virtual int supports_parallel_connects (void) const;

  /// Create a profile with a given endpoint.
  virtual TAO_Profile *make_profile (void) = 0;

  /// Set and validate endpoint. We need to do this to initialize our
  /// remote *_Addr's which have not been done during IOR decode.
  virtual int set_validate_endpoint (TAO_Endpoint *endpoint) = 0;

  /// Make a connection
  virtual TAO_Transport *make_connection (TAO::Profile_Transport_Resolver *r,
                                          TAO_Transport_Descriptor_Interface
                                          &desc, ACE_Time_Value *timeout) = 0;

  /// Make a connection using - not a pure virtual since not all
  /// protocols support this.
  virtual TAO_Transport* make_parallel_connection (
      TAO::Profile_Transport_Resolver *r,
      TAO_Transport_Descriptor_Interface &desc,
      ACE_Time_Value *timeout);

  /// Cancel the passed @a svc_handler from the connector
  virtual int cancel_svc_handler (TAO_Connection_Handler *svc_handler) = 0;

  /// Check whether the connection is not closed
  /**
   * @retval 0 The connection happens to be not closed, but is now open
   * because an other thread managed to open the handler
   * @retval -1 The connection is closed
   */
  virtual int check_connection_closure (TAO_Connection_Handler
                                        *connection_handler);

  /**
   * Wait for connection completion. We have a transport that is not
   * connected yet, wait until it is connected.
   * @retval true When we could use @a transport
   * @return false When we can't use the @a transport
   */
  virtual bool wait_for_connection_completion(
      TAO::Profile_Transport_Resolver *r,
      TAO_Transport_Descriptor_Interface &desc,
      TAO_Transport *&transport,
      ACE_Time_Value *timeout);

  /// In the case of a parallel connection attempt, we take an array of
  /// transports, and wait on any of them. When the first one completes,
  /// the rest are closed.
  virtual bool wait_for_connection_completion(
      TAO::Profile_Transport_Resolver *r,
      TAO_Transport *&the_winner,
      TAO_Transport **transport,
      unsigned int count,
      TAO_LF_Multi_Event *mev,
      ACE_Time_Value *timeout);

  /// For a parallel connection attempt, any pending connection attempts
  /// present when the connection completes needs to be cleaned up. In
  /// some cases this happens before wait_for_connection_completion needs
  /// to be called, so the clean up logic is separated out here.
  void cleanup_pending (
      TAO_Transport *&the_winner,
      TAO_Transport **transport,
      unsigned int count);

  /// See if a new connection is allowed
  bool new_connection_is_ok (size_t busy_count);

  /// Wait for a transport to be connected
  /// Note: no longer changes transport reference count
  /// @retval true if wait was uneventful
  /// @retval false if error occurred during wait
  bool wait_for_transport (TAO::Profile_Transport_Resolver *r,
                           TAO_Transport *base_transport,
                           ACE_Time_Value * timeout,
                           bool force_wait);

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
  CORBA::ULong const tag_;

  /// Pointer to our ORB core
  TAO_ORB_Core *orb_core_;

  //@@ TAO_CONNECTOR_SPL_PRIVATE_DATA_ADD_HOOK
};

//@@ TAO_CONNECTOR_SPL_EXTERN_ADD_HOOK

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Transport_Connector.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_CONNECTOR_H*/
