// -*- C++ -*-

//=============================================================================
/**
 *  @file     UIPMC_Profile.h
 *
 *  $Id$
 *
 *  UIPMC profile specific processing
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_UIPMC_PROFILE_H
#define TAO_UIPMC_PROFILE_H
#include "ace/pre.h"

#include "tao/Profile.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object_KeyC.h"
#include "tao/target_specification.h"

#include "orbsvcs/PortableGroupC.h"
#include "UIPMC_Endpoint.h"
#include "portablegroup_export.h"

#include "ace/Synch.h"


// TAO UIPMC_Profile concrete Profile definitions
/**
 * @class TAO_UIPMC_Profile
 *
 * @brief This class defines the protocol specific attributes required
 * for locating ORBs over a TCP/IP network.
 *
 * This class defines the UIPMC profile as specified in the CORBA
 * specification.
 */
class TAO_PortableGroup_Export TAO_UIPMC_Profile : public TAO_Profile
{
public:
  /// The object key delimiter that UIPMC uses or expects.
  static const char object_key_delimiter_;
  virtual char object_key_delimiter (void) const;

  /// Return the char string prefix.
  static const char *prefix (void);

  /// Profile constructor, default. Used when the profile contents
  /// are not yet known or when it is being decoded from an IOR.
  TAO_UIPMC_Profile (TAO_ORB_Core *orb_core);

  /// Profile constructor, same as above except the object_key has
  /// already been marshaled.
  TAO_UIPMC_Profile (const ACE_INET_Addr &addr,
                     TAO_ORB_Core *orb_core);

  ///  Profile constructor.
  TAO_UIPMC_Profile (const CORBA::Octet class_d_address[4],
                     CORBA::UShort port,
                     TAO_ORB_Core *orb_core);

  /// Create object using a string ior.
  TAO_UIPMC_Profile (const char *string,
                     TAO_ORB_Core *orb_core
                     TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Destructor is to be called only through <_decr_refcnt>.
  ~TAO_UIPMC_Profile (void);

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
   * Multiple TAG_ALTERNATE_UIPMC_ADDRESS components can be used
   * instead of a single proprietory component to transmit multiple
   * endpoints.  This is somewhat slower and less convenient.  Also,
   * TAG_ALTERNATE_UIPMC_ADDRESS does not provide for transmission of
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
   * Return true if this profile is equivalent to other_profile.  Two
   * profiles are equivalent iff their tag, object_key, version and
   * all endpoints are the same.
   */
  virtual CORBA::Boolean is_equivalent (const TAO_Profile *other_profile);

  /// Return a hash value for this object.
  virtual CORBA::ULong hash (CORBA::ULong max
                             TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Please refer to Profile.h for the documentation of this
  /// function.
  virtual IOP::TaggedProfile &create_tagged_profile (void);


  /// Set the request target specifier to point to a tagged profile
  /// containing the GroupId associated with this profile.
  virtual void request_target_specifier (
                      TAO_Target_Specification &target_spec,
                      TAO_Target_Specification::TAO_Target_Address required_type
                      TAO_ENV_ARG_DECL);

  /// Returns true since this profile can specify multicast endpoints.
  virtual int supports_multicast (void) const;

  static int extract_group_component (const IOP::TaggedProfile &profile,
                                      PortableGroup::TagGroupTaggedComponent &group);

  /// Add the mandatory group component to this profile.
  void set_group_info (const char *domain_id,
                       PortableGroup::ObjectGroupId group_id,
                       PortableGroup::ObjectGroupRefVersion ref_version);

  /// Set and validate the addressing mode if the remote ORB returns an
  /// exception.
  virtual void addressing_mode (CORBA::Short addr_mode
                                TAO_ENV_ARG_DECL);

private:

  /// Creates an encapsulation of the ProfileBody struct in the <cdr>
  void create_profile_body (TAO_OutputCDR &cdr) const;

  /// Update the group component in the cached list of tagged
  /// components.  This needs to be called whenever the group_domain_id,
  /// group_id, or ref_version changes.
  void update_cached_group_component (void);

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
   * is transmitted using standard UIPMC ProfileBody components.  See
   * <encode_endpoints> method documentation above for how the rest of
   * the endpoint list is transmitted.
   */
  TAO_UIPMC_Endpoint endpoint_;

  /// Number of endpoints in the list headed by <endpoint_>.
  size_t count_;

private:

  /// object_key associated with this profile.  This is wasted data for
  /// UIPMC since there is no object key here, but is needed to satisfy
  /// some of the API calls.
  TAO_ObjectKey object_key_;

  /// Cached version of our tagged profile.
  IOP::TaggedProfile tagged_profile_;

  /// Group Domain ID.
  ACE_CString group_domain_id_;

  /// Our group ID within the group domain.
  PortableGroup::ObjectGroupId group_id_;

  /// The group reference's version.
  PortableGroup::ObjectGroupRefVersion ref_version_;

};

#if defined (__ACE_INLINE__)
# include "tao/UIPMC_Profile.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_UIPMC_PROFILE_H */
