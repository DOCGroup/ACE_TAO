// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     UIOP_Profile.h
//
// = DESCRIPTION
//     Unix Domain Socket (UIOP) profile specific processing
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//     Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_UIOP_PROFILE_H
#define TAO_UIOP_PROFILE_H
#include "ace/pre.h"

#include "tao/Profile.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if TAO_HAS_UIOP == 1

#include "strategies_export.h"
#include "tao/Object_KeyC.h"
#include "UIOP_Connect.h"
#include "UIOP_Endpoint.h"

#include "ace/UNIX_Addr.h"
#include "ace/Synch.h"

//class TAO_UIOP_Client_Connection_Handler;

// TAO UIOP_Profile concrete Profile definitions
class TAO_Strategies_Export TAO_UIOP_Profile : public TAO_Profile
{
  // = TITLE
  //   This class defines the protocol specific attributes required
  //   for locating ORBs over local IPC.
  //
  // = DESCRIPTION
  //   This class defines the UIOP profile.
public:
  static const char object_key_delimiter_;
  virtual char object_key_delimiter (void) const;
  // The object key delimiter that UIOP uses or expects.

  static const char *prefix (void);
  // Return the char string prefix.

  TAO_UIOP_Profile (const ACE_UNIX_Addr &addr,
                    const TAO_ObjectKey &object_key,
                    const TAO_GIOP_Version &version,
                    TAO_ORB_Core *orb_core);
  // Profile constructor, same as above except the object_key has
  // already been marshaled.  (actually, no marshalling for this protocol)

  TAO_UIOP_Profile (const char *rendezvous_point,
                    const TAO_ObjectKey &object_key,
                    const ACE_UNIX_Addr &addr,
                    const TAO_GIOP_Version &version,
                    TAO_ORB_Core *orb_core);
  // Profile constructor

  TAO_UIOP_Profile (const char *string,
                    TAO_ORB_Core *orb_core,
                    CORBA::Environment &ACE_TRY_ENV =
                      TAO_default_environment ());
  // Create object using a string ior.

  TAO_UIOP_Profile (TAO_ORB_Core *orb_core);
  // Profile constructor, default.

  ~TAO_UIOP_Profile (void);
  // Destructor is to be called only through <_decr_refcnt>.

  virtual int parse_string (const char *string,
                            CORBA::Environment &ACE_TRY_ENV =
                            TAO_default_environment ());
  // Initialize this object using the given input string.

  virtual char * to_string (CORBA::Environment &ACE_TRY_ENV =
                            TAO_default_environment ());
  // Return a string representation for this profile.
  // client must deallocate memory.

  virtual int decode (TAO_InputCDR& cdr);
  // Initialize this object using the given CDR octet string.

  virtual int encode (TAO_OutputCDR &stream) const;
  // Encode this profile in a stream, i.e. marshal it.

  virtual int encode_endpoints (void);
  // Encodes this profile's endpoints into a tagged component.
  // This is done only if RTCORBA is enabled, since currently this is
  // the only case when we have more than one endpoint per profile.
  //
  // Endpoints are transmitted using TAO-proprietory tagged component.
  // Component tag is TAO_TAG_ENDPOINTS and component data is an
  // encapsulation of a sequence of structs, each representing a
  // single endpoint.  Data format is specified in uiop_endpoins.pidl.
  //
  // Multiple a la TAG_ALTERNATE_IIOP_ADDRESS components can be used
  // instead of a single proprietory component to transmit multiple
  // endpoints.  This is somewhat slower and less convenient.  Also,
  // TAG_ALTERNATE_IIOP_ADDRESS does not provide for transmission of
  // endpoint priorities.
  //

  virtual const TAO_ObjectKey &object_key (void) const;
  // @@ deprecated. return a reference to the Object Key.

  virtual TAO_ObjectKey *_key (void) const;
  // Return a pointer to the Object Key.  The caller owns the memory
  // allocated for the returned key.

  virtual TAO_Endpoint *endpoint (void);
  // Return pointer to the head of this profile's endpoints list.

  virtual size_t endpoint_count (void);
  // Return how many endpoints this profile contains.

  void add_endpoint (TAO_UIOP_Endpoint *endp);
  // Add <endp> to this profile's list of endpoints (it is inserted
  // next to the head of the list).  This profiles takes ownership of
  // <endp>.

  virtual CORBA::Boolean is_equivalent (const TAO_Profile *other_profile);
  // Return true if this profile is equivalent to other_profile.  Two
  // profiles are equivalent iff their tag, object_key, version and
  // all endpoints are the same.

  virtual CORBA::ULong hash (CORBA::ULong max,
                             CORBA::Environment &ACE_TRY_ENV =
                             TAO_default_environment ());
  // Return a hash value for this object.

  virtual IOP::TaggedProfile &create_tagged_profile (void);
  // Please see the Profile.h for the documentation of this method

private:

  void create_profile_body (TAO_OutputCDR &cdr) const;
  // Create an encapsulation of the struct ProfileBody in <cdr>

  int decode_endpoints (void);
  // Helper for <decode>.  Decodes endpoints from a tagged component.
  // Decode only if RTCORBA is enabled.  Furthermore, we may not find
  // TAO_TAG_ENDPOINTS component, e.g., if we are talking to nonRT
  // version of TAO or some other ORB.  This is not an error, and we
  // must proceed.  Return 0 on success and -1 on failure.

  TAO_UIOP_Endpoint endpoint_;
  // Head of this profile's list of endpoints.  This endpoint is not
  // dynamically allocated because a profile always contains at least
  // one endpoint.
  //
  // Currently, a profile contains more than one endpoint, i.e.,
  // list contains more than just the head, only when RTCORBA is enabled.
  // However, in the near future, this will be used in nonRT
  // mode as well, e.g., to support a la TAG_ALTERNATE_IIOP_ADDRESS
  // feature.
  // Addressing info of the default endpoint, i.e., head of the list,
  // is transmitted using standard UIOP ProfileBody components.  See
  // <encode_endpoints> method documentation above for how the rest of
  // the endpoint list is transmitted.

  size_t count_;
  // Number of endpoints in the list headed by <endpoint_>.

  TAO_ObjectKey object_key_;
  // object_key associated with this profile.

  IOP::TaggedProfile tagged_profile_;
  // Our tagged profile info
};

#if defined (__ACE_INLINE__)
# include "UIOP_Profile.i"
#endif /* __ACE_INLINE__ */

# endif  /* TAO_HAS_UIOP == 1 */

#include "ace/post.h"
#endif  /* TAO_UIOP_PROFILE_H */
