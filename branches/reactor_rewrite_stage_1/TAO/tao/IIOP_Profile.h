// This may look like C, but it's really -*- C++ -*-


//=============================================================================
/**
 *  @file     IIOP_Profile.h
 *
 *  $Id$
 *
 *   IIOP profile specific processing
 *
 *
 *  @author  Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IIOP_PROFILE_H
#define TAO_IIOP_PROFILE_H
#include "ace/pre.h"

#include "tao/Profile.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object_KeyC.h"
#include "tao/IIOP_Endpoint.h"

#include "ace/Synch.h"


// TAO IIOP_Profile concrete Profile definitions
/**
 * @class TAO_IIOP_Profile
 *
 * @brief This class defines the protocol specific attributes required
 * for locating ORBs over a TCP/IP network.
 *
 * This class defines the IIOP profile as specified in the CORBA
 * specification.
 */
class TAO_Export TAO_IIOP_Profile : public TAO_Profile
{
public:
  /// The object key delimiter that IIOP uses or expects.
  static const char object_key_delimiter_;
  virtual char object_key_delimiter (void) const;

  /// Return the char string prefix.
  static const char *prefix (void);

  /// Profile constructor, same as above except the object_key has
  /// already been marshaled.
  TAO_IIOP_Profile (const ACE_INET_Addr &addr,
                    const TAO_ObjectKey &object_key,
                    const TAO_GIOP_Message_Version &version,
                    TAO_ORB_Core *orb_core);

  ///  Profile constructor, this is the most efficient since it
  /// doesn't require any address resolution processing.
  TAO_IIOP_Profile (const char *host,
                    CORBA::UShort port,
                    const TAO_ObjectKey &object_key,
                    const ACE_INET_Addr &addr,
                    const TAO_GIOP_Message_Version &version,
                    TAO_ORB_Core *orb_core);

  /// Profile constructor, default.
  TAO_IIOP_Profile (TAO_ORB_Core *orb_core);

  /// Destructor is to be called only through <_decr_refcnt>.
  ~TAO_IIOP_Profile (void);

  /// Initialize this object using the given input string.
  /// Url-style string contain only one endpoint.
  virtual void parse_string (const char *string
                             TAO_ENV_ARG_DECL);

  /**
   * Return a string representation for this profile.
   * client must deallocate memory.
   * This is used to create url-style reference.  Only one
   * endpoint is included into the string.
   */
  virtual char * to_string (TAO_ENV_SINGLE_ARG_DECL);

  /// Initialize this object using the given CDR octet string.
  virtual int decode (TAO_InputCDR& cdr);

  /// Encode this profile in a stream, i.e. marshal it.
  virtual int encode (TAO_OutputCDR &stream) const;

  /**
   * Encodes this profile's endpoints into a tagged component.
   * This is done only if RTCORBA is enabled, since currently this is
   * the only case when we have more than one endpoint per profile.
   * Returns 0 on success and -1 on failure.
   *
   * Endpoints are transmitted using TAO-proprietory tagged component.
   * Component tag is TAO_TAG_ENDPOINTS and component data is an
   * encapsulation of a sequence of structs, each representing a
   * single endpoint.  Data format is specified in iiop_endpoins.pidl.
   *
   * Multiple TAG_ALTERNATE_IIOP_ADDRESS components can be used
   * instead of a single proprietory component to transmit multiple
   * endpoints.  This is somewhat slower and less convenient.  Also,
   * TAG_ALTERNATE_IIOP_ADDRESS does not provide for transmission of
   * endpoint priorities.
   *
   */
  virtual int encode_endpoints (void);

  /// @@ deprecated. return a reference to the Object Key.
  virtual const TAO_ObjectKey &object_key (void) const;

  /// Return a pointer to the Object Key.  The caller owns the memory
  /// allocated for the returned key.
  virtual TAO_ObjectKey *_key (void) const;

  /// Return pointer to the head of this profile's endpoints list.
  virtual TAO_Endpoint *endpoint (void);

  /// Return how many endpoints this profile contains.
  virtual size_t endpoint_count (void);

  /**
   * Add <endp> to this profile's list of endpoints (it is inserted
   * next to the head of the list).  This profiles takes ownership of
   * <endp>.
   */
  void add_endpoint (TAO_IIOP_Endpoint *endp);

  /**
   * Return true if this profile is equivalent to other_profile.  Two
   * profiles are equivalent iff their tag, object_key, version and
   * all endpoints are the same.
   */
  virtual CORBA::Boolean is_equivalent (const TAO_Profile *other_profile);

  /// Return a hash value for this object.
  virtual CORBA::ULong hash (CORBA::ULong max
                             TAO_ENV_ARG_DECL);

  /// Please refer to Profile.h for the documentation of this
  /// function.
  virtual IOP::TaggedProfile &create_tagged_profile (void);

private:

  /// Creates an encapsulation of the ProfileBody struct in the <cdr>
  void create_profile_body (TAO_OutputCDR &cdr) const;

  /**
   * Helper for <decode>.  Decodes endpoints from a tagged component.
   * Decode only if RTCORBA is enabled.  Furthermore, we may not find
   * TAO_TAG_ENDPOINTS component, e.g., if we are talking to nonRT
   * version of TAO or some other ORB.  This is not an error, and we
   * must proceed.  Return 0 on success and -1 on failure.
   */
  int decode_endpoints (void);

protected:

  /**
   * Head of this profile's list of endpoints.  This endpoint is not
   * dynamically allocated because a profile always contains at least
   * one endpoint.
   *
   * Currently, a profile contains more than one endpoint, i.e.,
   * list contains more than just the head, only when RTCORBA is enabled.
   * However, in the near future, this will be used in nonRT
   * mode as well, e.g., to support TAG_ALTERNATE_IIOP_ADDRESS
   * feature.
   * Addressing info of the default endpoint, i.e., head of the list,
   * is transmitted using standard IIOP ProfileBody components.  See
   * <encode_endpoints> method documentation above for how the rest of
   * the endpoint list is transmitted.
   */
  TAO_IIOP_Endpoint endpoint_;

  /// Number of endpoints in the list headed by <endpoint_>.
  size_t count_;

private:

  /// object_key associated with this profile.
  TAO_ObjectKey object_key_;

  /// Our tagged profile
  IOP::TaggedProfile tagged_profile_;
};

#if defined (__ACE_INLINE__)
# include "tao/IIOP_Profile.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_IIOP_PROFILE_H */
