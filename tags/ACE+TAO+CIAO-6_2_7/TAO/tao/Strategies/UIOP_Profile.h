// -*- C++ -*-

//=============================================================================
/**
 *  @file     UIOP_Profile.h
 *
 *  $Id$
 *
 *   Unix Domain Socket (UIOP) profile specific processing
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 *  @author Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_UIOP_PROFILE_H
#define TAO_UIOP_PROFILE_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if TAO_HAS_UIOP == 1

#include "tao/Strategies/strategies_export.h"
#include "tao/Profile.h"
#include "tao/Strategies/UIOP_Connection_Handler.h"
#include "tao/Strategies/UIOP_Endpoint.h"

#include "ace/UNIX_Addr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_UIOP_Profile
 *
 * @brief This class defines the protocol specific attributes required
 * for locating ORBs over local IPC.
 *
 * This class defines the UIOP profile.
 */
class TAO_Strategies_Export TAO_UIOP_Profile : public TAO_Profile
{
public:
  /// The object key delimiter that UIOP uses or expects.
  static const char object_key_delimiter_;
  virtual char object_key_delimiter (void) const;

  /// Return the char string prefix.
  static const char *prefix (void);

  /// Profile constructor, same as above except the object_key has
  /// already been marshaled.  (actually, no marshalling for this protocol)
  TAO_UIOP_Profile (const ACE_UNIX_Addr &addr,
                    const TAO::ObjectKey &object_key,
                    const TAO_GIOP_Message_Version &version,
                    TAO_ORB_Core *orb_core);

  /// Profile constructor
  TAO_UIOP_Profile (const char *rendezvous_point,
                    const TAO::ObjectKey &object_key,
                    const ACE_UNIX_Addr &addr,
                    const TAO_GIOP_Message_Version &version,
                    TAO_ORB_Core *orb_core);

  /// Profile constructor, default.
  TAO_UIOP_Profile (TAO_ORB_Core *orb_core);

  /// Destructor is to be called only through <_decr_refcnt>.
  ~TAO_UIOP_Profile (void);

  /// Template methods. Please see Profile.h for documentation.
  virtual char *to_string (void) const;
  virtual int encode_endpoints (void);
  virtual TAO_Endpoint *endpoint (void);
  virtual CORBA::ULong endpoint_count (void) const;
  virtual CORBA::ULong hash (CORBA::ULong max);
  /**
   * Add @a endp to this profile's list of endpoints (it is inserted
   * next to the head of the list).  This profiles takes ownership of
   * @a endp.
   */
  void add_endpoint (TAO_UIOP_Endpoint *endp);

protected:

  /// Protected template methods. Please see documentation in
  /// Profile.h for details.
  virtual int decode_profile (TAO_InputCDR& cdr);
  virtual void parse_string_i (const char *string);
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
   * mode as well, e.g., to support a la TAG_ALTERNATE_IIOP_ADDRESS
   * feature.
   * Addressing info of the default endpoint, i.e., head of the list,
   * is transmitted using standard UIOP ProfileBody components.  See
   * <encode_endpoints> method documentation above for how the rest of
   * the endpoint list is transmitted.
   */
  TAO_UIOP_Endpoint endpoint_;

  /// Number of endpoints in the list headed by <endpoint_>.
  CORBA::ULong count_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

# endif  /* TAO_HAS_UIOP == 1 */

#include /**/ "ace/post.h"

#endif  /* TAO_UIOP_PROFILE_H */
