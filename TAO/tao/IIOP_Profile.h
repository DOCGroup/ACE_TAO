// This may look like C, but it's really -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     IIOP_Profile.h
//
// = DESCRIPTION
//     IIOP profile specific processing
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//
// ============================================================================

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

class TAO_IIOP_Client_Connection_Handler;

// TAO IIOP_Profile concrete Profile definitions
class TAO_Export TAO_IIOP_Profile : public TAO_Profile
{
  // = TITLE
  //   This class defines the protocol specific attributes required
  //   for locating ORBs over a TCP/IP network.
  //
  // = DESCRIPTION
  //   This class defines the IIOP profile as specified in the CORBA
  //   specification.
public:
  static const char object_key_delimiter_;
  virtual char object_key_delimiter (void) const;
  // The object key delimiter that IIOP uses or expects.

  static const char *prefix (void);
  // Return the char string prefix.

  TAO_IIOP_Profile (const ACE_INET_Addr &addr,
                    const TAO_ObjectKey &object_key,
                    const TAO_GIOP_Version &version,
                    TAO_ORB_Core *orb_core);
  // Profile constructor, same as above except the object_key has
  // already been marshaled.

  TAO_IIOP_Profile (const char *host,
                    CORBA::UShort port,
                    const TAO_ObjectKey &object_key,
                    const ACE_INET_Addr &addr,
                    const TAO_GIOP_Version &version,
                    TAO_ORB_Core *orb_core);
  //  Profile constructor, this is the most efficient since it
  // doesn't require any address resolution processing.

  TAO_IIOP_Profile (const char *string,
                    TAO_ORB_Core *orb_core,
                    CORBA::Environment &ACE_TRY_ENV =
                      TAO_default_environment ());
  // Create object using a string ior.

  TAO_IIOP_Profile (TAO_ORB_Core *orb_core);
  // Profile constructor, default.

  ~TAO_IIOP_Profile (void);
  // Destructor is to be called only through <_decr_refcnt>.

  virtual TAO_Endpoint *endpoint (void);
  // Head of the list of endpoints for this profile.

  void add_endpoint (TAO_IIOP_Endpoint *endp);
  //

  int parse_string (const char *string,
                    CORBA::Environment &ACE_TRY_ENV =
                      TAO_default_environment ());
  // Initialize this object using the given input string.
  // Url-style string contain only one endpoint.

  char * to_string (CORBA::Environment &ACE_TRY_ENV =
                      TAO_default_environment ());
  // Return a string representation for this profile.
  // client must deallocate memory.
  // This is used to create url-style reference.  Only one
  // endpoint is included into the string.

  int decode (TAO_InputCDR& cdr);
  // Initialize this object using the given CDR octet string.

  virtual int encode (TAO_OutputCDR &stream) const;
  // Encode this profile in a stream, i.e. marshal it.

  virtual const TAO_ObjectKey &object_key (void) const;
  // @@ deprecated. return a reference to the Object Key.

  TAO_ObjectKey *_key (void) const;
  // Return a pointer to the Object Key.  The caller owns the memory
  // allocated for the returned key.

  CORBA::Boolean is_equivalent (const TAO_Profile *other_profile);
  // Return true if this profile is equivalent to other_profile.  Two
  // profiles are equivalent iff their key, port, host, object_key and
  // version are the same.

  CORBA::ULong hash (CORBA::ULong max,
                     CORBA::Environment &ACE_TRY_ENV =
                     TAO_default_environment ());
  // Return a hash value for this object.

  IOP::TaggedProfile &create_tagged_profile (void);
  // Please refer to Profile.h for the documentation of this
  // function.

private:

  void create_profile_body (TAO_OutputCDR &cdr) const;
  // Creates a encapsultaion of the ProfileBody struct in the <cdr>

  int encode_endpoints (void);
  // Encodes endpoints from this profile into a tagged component.

  int decode_endpoints (void);
  // Decodes endpoints of this profile from a tagged component.

protected:

  TAO_IIOP_Endpoint endpoint_;
  // Head of the list of endpoints for this profile.

  size_t count_;
  // Number of endpoints this profile contains.

  int endpoints_encoded_;
  // Flag indicating whether endpoints have already been encoded,
  // saving us from repeatedly encoding them over and over.

private:

  TAO_ObjectKey object_key_;
  // object_key associated with this profile.

  IOP::TaggedProfile tagged_profile_;
  // Our tagged profile
};

#if defined (__ACE_INLINE__)
# include "tao/IIOP_Profile.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_IIOP_PROFILE_H */
