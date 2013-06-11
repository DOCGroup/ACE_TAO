// -*- C++ -*-

//=============================================================================
/**
 *  @file     SHMIOP_Profile.h
 *
 *  $Id$
 *
 *   SHMIOP profile specific processing
 *
 *  @author  Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_SHMIOP_PROFILE_H
#define TAO_SHMIOP_PROFILE_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "tao/Strategies/strategies_export.h"
#include "tao/Profile.h"
#include "tao/Strategies/SHMIOP_Endpoint.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// TAO SHMIOP_Profile concrete Profile definitions
/**
 * @class TAO_SHMIOP_Profile
 *
 * @brief This class defines the protocol specific attributes required
 * for locating local ORBs over local IPC mechanism to use the
 * shared memory transport.
 *
 * This class defines the SHMIOP profile.
 */
class TAO_Strategies_Export TAO_SHMIOP_Profile : public TAO_Profile
{
public:
  /// The object key delimiter that SHMIOP uses or expects.
  static const char object_key_delimiter_;
  virtual char object_key_delimiter (void) const;

  /// Return the char string prefix.
  static const char *prefix (void);

  /// Profile constructor, same as above except the object_key has
  /// already been marshaled.
  TAO_SHMIOP_Profile (const ACE_MEM_Addr &addr,
                      const TAO::ObjectKey &object_key,
                      const TAO_GIOP_Message_Version &version,
                      TAO_ORB_Core *orb_core);

  /// Profile constructor, this is the most efficient since it
  /// doesn't require any address resolution processing.
  TAO_SHMIOP_Profile (const char *host,
                      CORBA::UShort port,
                      const TAO::ObjectKey &object_key,
                      const ACE_INET_Addr &addr,
                      const TAO_GIOP_Message_Version &version,
                      TAO_ORB_Core *orb_core);

  /// Profile constructor, default.
  TAO_SHMIOP_Profile (TAO_ORB_Core *orb_core);

  /// Destructor is to be called only through <_decr_refcnt>.
  ~TAO_SHMIOP_Profile (void);


  /// Template methods, please see Profile.h for documentation.
  virtual char * to_string (void) const;
  virtual int encode_endpoints (void);
  virtual TAO_Endpoint *endpoint (void);
  virtual CORBA::ULong endpoint_count (void) const;
  virtual CORBA::ULong hash (CORBA::ULong max
                            );
  /**
   * Add <endp> to this profile's list of endpoints (it is inserted
   * next to the head of the list).  This profiles takes ownership of
   * <endp>.
   */
  void add_endpoint (TAO_SHMIOP_Endpoint *endp);

protected:

  /// Template methods. Please see tao/Profile.h for documentation.
  virtual int decode_profile (TAO_InputCDR& cdr);
  virtual void parse_string_i (const char *string
                              );
  virtual void create_profile_body (TAO_OutputCDR &cdr) const;
  virtual int decode_endpoints (void);
  virtual CORBA::Boolean do_is_equivalent (const TAO_Profile *other_profile);

private:

  /**
   * Head of this profile's list of endpoints.  This endpoint is not
   * dynamically allocated because a profile always contains at least
   * one endpoint.
   *
   * Currently, a profile contains more than one endpoint, i.e.,
   * list contains more than just the head, only when RTCORBA is enabled.
   * However, in the near future, this will be used in nonRT
   * mode as well, e.g., to support TAG_ALTERNATE_IIOP_ADDRESS-style
   * feature.
   * Addressing info of the default endpoint, i.e., head of the list,
   * is transmitted using standard SHMIOP ProfileBody components.  See
   * <encode_endpoints> method documentation above for how the rest of
   * the endpoint list is transmitted.
   */
  TAO_SHMIOP_Endpoint endpoint_;

  /// Number of endpoints in the list headed by <endpoint_>.
  CORBA::ULong count_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

#include /**/ "ace/post.h"

#endif  /* TAO_SHMIOP_PROFILE_H */
