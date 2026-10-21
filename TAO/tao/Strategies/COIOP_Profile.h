// -*- C++ -*-

//=============================================================================
/**
 *  @file     COIOP_Profile.h
 *
 *   COIOP profile specific processing
 *
 *  @author  Johnny Willemsen <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_COIOP_PROFILE_H
#define TAO_COIOP_PROFILE_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_COIOP) && (TAO_HAS_COIOP != 0)

#include "tao/Strategies/COIOP_Endpoint.h"
#include "tao/Profile.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// TAO COIOP_Profile concrete Profile definitions
/**
 * @class TAO_COIOP_Profile
 *
 * @brief This class defines the protocol specific attributes required
 * for locating ORBs over a TCP/IP network.
 *
 * This class defines the COIOP profile as specified in the CORBA
 * specification.
 */
class TAO_Strategies_Export TAO_COIOP_Profile : public TAO_Profile
{
public:
  /// The object key delimiter that COIOP uses or expects.
  static constexpr char object_key_delimiter_ {'/'};
  char object_key_delimiter () const override;

  /// Return the char string prefix.
  static char const *prefix ();

  /// Profile constructor, this is the most efficient since it
  /// doesn't require any address resolution processing.
  TAO_COIOP_Profile (ACE_Utils::UUID const &uuid,
                     TAO::ObjectKey const &object_key,
                     TAO_GIOP_Message_Version const &version,
                     TAO_ORB_Core *orb_core);

  /// Profile constructor, default.
  explicit TAO_COIOP_Profile (TAO_ORB_Core *orb_core);

  /// Destructor is to be called only through <_decr_refcnt>.
  ~TAO_COIOP_Profile ();

  /// Template methods. Please tao/Profile.h for documentation.
  char *to_string () const override;
  int encode_endpoints () override;
  TAO_Endpoint *endpoint () override;
  CORBA::ULong endpoint_count () const override;
  CORBA::ULong hash (CORBA::ULong max) override;
  /**
   * Add <endp> to this profile's list of endpoints (it is inserted
   * next to the head of the list).  This profiles takes ownership of
   * <endp>.
   */
  void add_endpoint (TAO_COIOP_Endpoint *endp);

protected:
  /// Template methods. Please see tao/Profile.h for documentation.
  int decode_profile (TAO_InputCDR &cdr) override;
  void parse_string_i (char const *string) override;
  void create_profile_body (TAO_OutputCDR &cdr) const override;
  int decode_endpoints () override;
  CORBA::Boolean do_is_equivalent (TAO_Profile const *other_profile) override;

protected:
  TAO_COIOP_Profile (TAO_COIOP_Profile const &) = delete;
  TAO_COIOP_Profile &operator= (TAO_COIOP_Profile const &) = delete;
  TAO_COIOP_Profile (TAO_COIOP_Profile &&) = delete;
  TAO_COIOP_Profile &operator= (TAO_COIOP_Profile &&) = delete;

  /**
   * Head of this profile's list of endpoints.  This endpoint is not
   * dynamically allocated because a profile always contains at least
   * one endpoint.
   *
   * Currently, a profile contains more than one endpoint, i.e.,
   * list contains more than just the head, only when RTCORBA is enabled.
   * However, in the near future, this will be used in nonRT
   * mode as well, e.g., to support TAG_ALTERNATE_COIOP_ADDRESS
   * feature.
   * Addressing info of the default endpoint, i.e., head of the list,
   * is transmitted using standard COIOP ProfileBody components.  See
   * <encode_endpoints> method documentation above for how the rest of
   * the endpoint list is transmitted.
   */
  TAO_COIOP_Endpoint endpoint_;

  /// Number of endpoints in the list headed by <endpoint_>.
  CORBA::ULong count_ {1};
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_COIOP && TAO_HAS_COIOP != 0 */

#include /**/ "ace/post.h"

#endif  /* TAO_COIOP_PROFILE_H */
