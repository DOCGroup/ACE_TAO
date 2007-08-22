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

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

// Forward declarations.
ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Addr;
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

//@@ TAO_ACCEPTOR_SPL_INCLUDE_FORWARD_DECL_ADD_HOOK

// ****************************************************************

/// The TAO-specific OMG assigned value for the TAG_ORB_TYPE tagged
/// component.
/**
 * This number was assigned by the OMG.  Do *NOT* change.  The ASCII
 * representation is "TA\x00".  If necessary, we can request more ORB
 * types later.
 */
const CORBA::ULong TAO_ORB_TYPE = 0x54414f00U;

// ****************************************************************

/**
 * @class TAO_Acceptor
 *
 * @brief Abstract Acceptor class used for pluggable transports.
 *
 * Base class for the Acceptor bridge class.
 *
 * @todo Need to rename the class as TAO_Transport_Acceptor.
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

  /// Return 1 if the @a endpoint has the same address as the acceptor.
  virtual int is_collocated (const TAO_Endpoint *endpoint) = 0;

  /**
   * Returns the number of endpoints this acceptor is listening on.  This
   * is used for determining how many profiles will be generated
   * for this acceptor.
   */
  virtual CORBA::ULong endpoint_count (void) = 0;

  /**
   * This method fetches the @a key from the @a profile. Protocols that
   * are pluggable can send data that are specific in the
   * @c profile_data field encapsulated as a octet stream. This method
   * allows those  protocols to get the object key from the
   * encapsulation.
   */
  virtual int object_key (IOP::TaggedProfile &profile,
                          TAO::ObjectKey &key) = 0;

  /*
   * Hook to add public methods from derived acceptor classes onto
   * this class.
   */
  //@@ TAO_ACCEPTOR_SPL_PUBLIC_METHODS_ADD_HOOK

private:
  /// IOP protocol tag.
  CORBA::ULong const tag_;

  /*
   * Hook to add data members from concrete acceptor implementations onto
   * the base class.
   */
  //@@ TAO_ACCEPTOR_SPL_DATA_MEMBERS_ADD_HOOK
};

//@@ TAO_ACCEPTOR_SPL_EXTERN_ADD_HOOK

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Transport_Acceptor.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_ACCEPTOR_H */
