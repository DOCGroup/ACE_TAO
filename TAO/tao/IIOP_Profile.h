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

#include /**/ "ace/pre.h"

#include "tao/Profile.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IIOP_Endpoint.h"

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
                    const TAO::ObjectKey &object_key,
                    const TAO_GIOP_Message_Version &version,
                    TAO_ORB_Core *orb_core);

  /// Profile constructor, this is the most efficient since it
  /// doesn't require any address resolution processing.
  TAO_IIOP_Profile (const char *host,
                    CORBA::UShort port,
                    const TAO::ObjectKey &object_key,
                    const ACE_INET_Addr &addr,
                    const TAO_GIOP_Message_Version &version,
                    TAO_ORB_Core *orb_core);

  /// Profile constructor, default.
  TAO_IIOP_Profile (TAO_ORB_Core *orb_core);

  /// Destructor is to be called only through <_decr_refcnt>.
  ~TAO_IIOP_Profile (void);

  /// Template methods. Please see Profile.h for documentation.
  virtual char * to_string (ACE_ENV_SINGLE_ARG_DECL);
  virtual int encode_endpoints (void);
  virtual TAO_Endpoint *endpoint (void);
  virtual CORBA::ULong endpoint_count (void) const;
  virtual CORBA::ULong hash (CORBA::ULong max
                             ACE_ENV_ARG_DECL);
  /**
   * Add @a endp to this profile's list of endpoints (it is inserted
   * next to the head of the list).  This profiles takes ownership of
   * @a endp.
   */
  void add_endpoint (TAO_IIOP_Endpoint *endp);

protected:

  /// Template methods. Please see Profile.h for the documentation.
  virtual int decode_profile (TAO_InputCDR &cdr);
  virtual int decode_endpoints (void);
  virtual void parse_string_i (const char *string
                               ACE_ENV_ARG_DECL);
  virtual void create_profile_body (TAO_OutputCDR &cdr) const;
  virtual CORBA::Boolean do_is_equivalent (const TAO_Profile *other_profile);

protected:

  /**
   * Head of this profile's list of endpoints.  This endpoint is not
   * dynamically allocated because a profile always contains at least
   * one endpoint.
   *
   * Currently, a profile contains more than one endpoint, i.e.,
   * list contains more than just the head, only for two cases
   * (1) when RTCORBA is enabled and
   * (2) the ORB is initialized with -ORBPreferredInterfaces option.
   * However, in the near future, this will be used in for mode as
   * well, e.g., to support TAG_ALTERNATE_IIOP_ADDRESS  feature.
   *
   * This is probably as good a place to discuss how the list of
   * endpoints is used for #2. If the ORB is configured to use
   * preferred interfaces for invocation, TAO creates an endpoint per
   * preferred interface. To be clear, every tuple
   * <destination:target> will have an endpoint. What TAO essentially
   * does is that creates it multiple endpoints so that the invocation
   * code path can use existing iterating techniques to try one
   * preferred interface after another (if the first did not work). If
   * the ORB is configured with -ORBEnforcePreferredInterface set to
   * false in addition to the ORBPreferredInterfaces option , TAO
   * creates another endpoint with the preferred bit set to null, so
   * that the invocation code can fall back to a TCP stack returned
   * local address.
   * Addressing info of the default endpoint, i.e., head of the list,
   * is transmitted using standard IIOP ProfileBody components.  See
   * <encode_endpoints> method documentation above for how the rest of
   * the endpoint list is transmitted.
   */
  TAO_IIOP_Endpoint endpoint_;

  /// Number of endpoints in the list headed by <endpoint_>.
  CORBA::ULong count_;
};

#if defined (__ACE_INLINE__)
# include "tao/IIOP_Profile.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif  /* TAO_IIOP_PROFILE_H */
