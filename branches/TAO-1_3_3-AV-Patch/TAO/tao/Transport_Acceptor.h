// -*- C++ -*-

//=============================================================================
/**
 *  @file Transport_Acceptor.h
 *
 *  $Id$
 *
 *  Interface for the Acceptor component of the TAO pluggable protocol
 *  framework.
 *
 *  @author  Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================
#ifndef TAO_ACCEPTOR_H
#define TAO_ACCEPTOR_H
#include /**/ "ace/pre.h"
#include "tao/corbafwd.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


// Forward declarations.
class ACE_Addr;
class ACE_Reactor;
class TAO_ORB_Core;
class TAO_MProfile;
class TAO_Endpoint;
class TAO_Transport;

namespace IOP
{
  struct TaggedProfile;
}

namespace TAO
{
  class ObjectKey;
}
// ****************************************************************

/**
 * @class TAO_Acceptor
 *
 * @brief Abstract Acceptor class used for pluggable transports.
 *
 * Base class for the Acceptor bridge class.
 *
 * @@TODO: Need to rename the class as TAO_Transport_Acceptor.
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
   */
  virtual int open_default (TAO_ORB_Core *,
                            ACE_Reactor *reactor,
                            int version_major,
                            int version_minor,
                            const char *options = 0) = 0;

  /// Closes the acceptor
  virtual int close (void) = 0;

  /**
   * Create the corresponding profile for this endpoint.
   * If share_profile is set to true, the pluggable protocol
   * implementation should try to add the endpoint to a profile
   * in the mprofile that is of the same type.  Currently, this
   * is used when RT CORBA is enabled.
   */
  virtual int create_profile (const TAO::ObjectKey &object_key,
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
                          TAO::ObjectKey &key) = 0;

private:

  /// IOP protocol tag.
  CORBA::ULong tag_;

};

#if defined (__ACE_INLINE__)
# include "Transport_Acceptor.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_ACCEPTOR_H */
