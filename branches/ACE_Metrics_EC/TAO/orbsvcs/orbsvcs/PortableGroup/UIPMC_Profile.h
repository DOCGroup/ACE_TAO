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
#include "ace/SString.h"


// TAO UIPMC_Profile concrete Profile definitions
/**
 * @class TAO_UIPMC_Profile
 *
 * @brief This class defines the protocol specific attributes required
 * for locating ORBs over a TCP/IP network.
 *
 * This class defines the UIPMC profile as specified in the CORBA
 * specification.
 *
 * @NOTE: This class inherits the ObjectKey from TAO_Profile which may
 *  not be needed at all! But lets punt on this for the timebeing.
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
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Destructor is to be called only through <_decr_refcnt>.
  ~TAO_UIPMC_Profile (void);

  /// Template methods. Please see tao/Profile.h for documentation.
  virtual void parse_string (const char *string
                             ACE_ENV_ARG_DECL);
  virtual char * to_string (ACE_ENV_SINGLE_ARG_DECL);
  virtual int encode_endpoints (void);
  virtual TAO_Endpoint *endpoint (void);
  virtual size_t endpoint_count (void);
  virtual CORBA::Boolean is_equivalent (const TAO_Profile *other_profile);
  virtual CORBA::ULong hash (CORBA::ULong max
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  virtual IOP::TaggedProfile &create_tagged_profile (void);
  virtual void request_target_specifier (
                      TAO_Target_Specification &target_spec,
                      TAO_Target_Specification::TAO_Target_Address r
                      ACE_ENV_ARG_DECL);
  virtual int supports_multicast (void) const;
  virtual void addressing_mode (CORBA::Short addr_mode
                                ACE_ENV_ARG_DECL);
  static int extract_group_component (const IOP::TaggedProfile &profile,
                                      PortableGroup::TagGroupTaggedComponent &group);

  /// Add the mandatory group component to this profile.
  void set_group_info (const char *domain_id,
                       PortableGroup::ObjectGroupId group_id,
                       PortableGroup::ObjectGroupRefVersion ref_version);


protected:
  /// Template methods, please see documentation in tao/Profile.h
  virtual int decode_profile (TAO_InputCDR& cdr);
  virtual int decode_endpoints (void);
  virtual void parse_string_i (const char *
                               ACE_ENV_ARG_DECL);
  virtual void create_profile_body (TAO_OutputCDR &cdr) const;
  virtual void update_cached_group_component (void);

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
# include "UIPMC_Profile.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_UIPMC_PROFILE_H */
