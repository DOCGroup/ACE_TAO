// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file     Pluggable.h
 *
 *  $Id$
 *
 *   Interface for the TAO pluggable protocol framework.
 *
 *  @author  Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_PLUGGABLE_H
#define TAO_PLUGGABLE_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Sequence.h"
#include "tao/Typecode.h"
#include "tao/IOPC.h"

// Forward declarations.
class ACE_Addr;
class ACE_Reactor;
class TAO_ORB_Core;

class TAO_Stub;
class TAO_Endpoint;
class TAO_Profile;
class TAO_MProfile;
class TAO_Resource_Factory;

class TAO_Reply_Dispatcher;
class TAO_Transport_Mux_Strategy;
class TAO_Wait_Strategy;

class TAO_GIOP_Invocation;
class TAO_Pluggable_Messaging_Interface;
class TAO_Target_Specification;
class TAO_Operation_Details;
class TAO_Transport_Descriptor_Interface;

class TAO_Transport;

// ****************************************************************

/**
 * @class TAO_Acceptor
 *
 * @brief Abstract Acceptor class used for pluggable protocols.
 *
 * Base class for the Acceptor bridge class.
 */
class TAO_Export TAO_Acceptor
{
public:
  TAO_Acceptor (CORBA::ULong tag);

  /// Destructor
  virtual ~TAO_Acceptor (void);

  /// The tag, each concrete class will have a specific tag value.
  CORBA::ULong tag (void) const;

  /// Method to initialize acceptor for address.
  virtual int open (TAO_ORB_Core *orb_core,
                    ACE_Reactor *reactor,
                    int version_major,
                    int version_minor,
                    const char *address,
                    const char *options = 0) = 0;

  /**
   * Open an acceptor with the given protocol version on a default
   * endpoint
   * @@ This method should be pure virtual, but in order to maintain
   *    some form of backward compatibilty, a default implementation
   *    is provided.  Ideally, that default implementation should be
   *    removed in the near future.
   */
  virtual int open_default (TAO_ORB_Core *,
                            ACE_Reactor *reactor,
                            int version_major,
                            int version_minor,
                            const char *options = 0) = 0;

  /// Closes the acceptor
  virtual int close (void) = 0;

  /** Create the corresponding profile for this endpoint.
   * If share_profile is set to true, the pluggable protocol
   * implementation should try to add the endpoint to a profile
   * in the mprofile that is of the same type.  Currently, this
   * is used when RT CORBA is enabled.
   */
  virtual int create_profile (const TAO_ObjectKey &object_key,
                              TAO_MProfile &mprofile,
                              CORBA::Short priority) = 0;

  /// Return 1 if the <endpoint> has the same address as the acceptor.
  virtual int is_collocated (const TAO_Endpoint* endpoint) = 0;

  /**
   * Returns the number of endpoints this acceptor is listening on.  This
   * is used for determining how many profiles will be generated
   * for this acceptor.
   */
  virtual CORBA::ULong endpoint_count (void) = 0;

  /**
   * This method fetches the <key> from the <profile>. Protocols that
   * are pluggable can send data that are specific in the
   * <profile_data> field encapsulated as a octet stream. This method
   * allows those  protocols to get the object key from the
   * encapsulation.
   */
  virtual int object_key (IOP::TaggedProfile &profile,
                          TAO_ObjectKey &key) = 0;

private:
  /// IOP protocol tag.
  CORBA::ULong tag_;

};

/**
 * @class TAO_Connector
 *
 * @brief Generic Connector interface definitions.
 *
 * Base class for connector bridge object.
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
   * To support pluggable we need to abstract away the connect()
   * method so it can be called from the GIOP code independant of the
   * actual transport protocol in use.
   */
  virtual int connect (TAO_GIOP_Invocation *invocation,
                       TAO_Transport_Descriptor_Interface *desc
                       ACE_ENV_ARG_DECL) = 0;

  /// Initial set of connections to be established.
  virtual int preconnect (const char *preconnections) = 0;

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
# include "tao/Pluggable.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_PLUGGABLE_H */
