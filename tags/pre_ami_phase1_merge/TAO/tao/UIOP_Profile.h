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

# if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "ace/Synch.h"
#include "ace/UNIX_Addr.h"

#include "tao/Pluggable.h"
#include "tao/Object_KeyC.h"
#include "tao/UIOP_Connect.h"

//class TAO_UIOP_Client_Connection_Handler;

// TAO UIOP_Profile concrete Profile definitions
class TAO_Export TAO_UIOP_Profile : public TAO_Profile
{
  // = TITLE
  //   This class defines the protocol specific attributes required
  //   for locating ORBs over Unix Domain Sockets.
  //
  // = DESCRIPTION
  //   This class defines the UIOP profile.
public:
  // = Currently, TAO supports UIOP 1.0.
  enum
    {
      DEF_UIOP_MAJOR = 1,  // FIXME:  Version numbers?
      DEF_UIOP_MINOR = 0
    };

  static const char object_key_delimiter;
  // The object key delimiter that UIOP uses or expects.

  static const char *prefix (void);
  // Return the char string prefix.

  TAO_UIOP_Profile (const ACE_UNIX_Addr &addr,
                    const char *object_key);
  // Profile constructor, the rendezvous_point field derived derived from
  // addr.

  TAO_UIOP_Profile (const ACE_UNIX_Addr &addr,
                    const TAO_ObjectKey &object_key);
  // Profile constructor, same as above except the object_key has
  // already been marshaled.  (actually, no marshalling for this protocol)

  TAO_UIOP_Profile (const ACE_UNIX_Addr &addr,
                    const TAO_IOP_Version &version,
                    const char *object_key);
  // Profile constructor, explicitly define the protocol version.

  TAO_UIOP_Profile (const ACE_UNIX_Addr &addr,
                    const TAO_IOP_Version &version,
                    const TAO_ObjectKey &object_key);
  // Profile constructor, explicitly define the protocol version.

  TAO_UIOP_Profile (const char *rendezvous_point,
                    const TAO_ObjectKey &object_key,
                    const ACE_UNIX_Addr &addr);
  // Profile constructor

  TAO_UIOP_Profile (const char *rendevous_point,
                    const TAO_ObjectKey &object_key);
  // Profile constructor

  TAO_UIOP_Profile (const char *rendezvous_point,
                    const TAO_IOP_Version &version,
                    const TAO_ObjectKey &object_key);
  // Profile constructor, explicitly define the protocol version
  // FIXME:  What do we do about versions?

  TAO_UIOP_Profile (const char *string,
                    CORBA::Environment &env);
  // Create object using a string ior.

  TAO_UIOP_Profile (const TAO_UIOP_Profile *pfile);
  // Profile copy constructor

  TAO_UIOP_Profile (const TAO_UIOP_Profile &pfile);
  // Profile copy constructor

  TAO_UIOP_Profile (const TAO_IOP_Version &version);
  // Profile constructor, explicitly define the version.

  TAO_UIOP_Profile (void);
  // Profile constructor, default.

  ~TAO_UIOP_Profile (void);
  // Destructor is to be called only through <_decr_refcnt>.

  int parse_string (const char *string,
                    CORBA::Environment &env);
  // Initialize this object using the given input string.

  CORBA::String to_string (CORBA::Environment &env);
  // Return a string representation for this profile.
  // client must deallocate memory.

  const TAO_opaque& body (void) const;
  // Create UIOP_Profile Object from marshalled data.

  int decode (TAO_InputCDR& cdr);
  // Initialize this object using the given CDR octet string.

  virtual int encode (TAO_OutputCDR &stream) const;
  // Encode this profile in a stream, i.e. marshal it.
  // FIXME:  NO MARSHALING for Unix Domain Sockets is needing

  const TAO_ObjectKey &object_key (void) const;
  // @@ deprecated, return a reference to the Object Key.

  TAO_ObjectKey &object_key (TAO_ObjectKey& objkey);
  // @@ deprecated. set the Object Key.

  TAO_ObjectKey *_key (CORBA::Environment &env) const;
  // Return a pointer to the Object Key.

  CORBA::Boolean is_equivalent (TAO_Profile *other_profile,
                                CORBA::Environment &env);
  // Return true if this profile is equivalent to other_profile.  Two
  // profiles are equivalent iff their key, rendezvous point, object_key
  // and version are the same.

  CORBA::ULong hash (CORBA::ULong max,
                     CORBA::Environment &env);
  // Return a hash value for this object.

  char *addr_to_string (void);
  // Return a string representation for the address.

  const ACE_UNIX_Addr &object_addr (void) const;
  // return a reference to the object_addr.

  const char *rendezvous_point (void);
  // Return a pointer to the rendezvous point string.  This object maintains
  // ownership of this string.

  const char *rendezvous_point (const char *r);
  // Copy the string r into rendezvous_point_ and return the
  // resulting pointer.
  // This object maintains ownership of this string.

  const TAO_IOP_Version *version (void);
  // Return a pointer to this profile's version.  This object
  // maintains ownership.

  const TAO_IOP_Version *version (TAO_IOP_Version *v);
  // First set the version then return a pointer to it.  This object
  // maintains ownership.

  TAO_UIOP_Client_Connection_Handler *&hint (void);
  // This is a hint for which connection handler to use.

  void reset_hint (void);
  // Reset the hint's value.

  TAO_Profile *_nil (void);
  // Return a null object pointer.

  TAO_UIOP_Profile & operator= (const TAO_UIOP_Profile &src);
  // Assignment operator

private:
  int set (const ACE_UNIX_Addr &addr);
  // helper method to set the UNIX_Addr.

  void create_body (void);
  // Does the work for <add_profile>.

private:

  char *rendezvous_point_;
  // String representing the rendezvous point.

  TAO_opaque body_;
  // Marshaled profile (CDR).

  TAO_IOP_Version version_;
  // UIOP version number.

  TAO_ObjectKey object_key_;
  // object_key associated with this profile.

  ACE_UNIX_Addr object_addr_;
  // Cached instance of <ACE_UNIX_Addr> for use in making
  // invocations, etc.

  TAO_UIOP_Client_Connection_Handler *hint_;
  // Pointer to a connection handler which we successfully used
  // already.

  TAO_MProfile *forward_to_;
  // list of profiles which we should try forwarding on.
};

#if defined (__ACE_INLINE__)
# include "tao/UIOP_Profile.i"
#endif /* __ACE_INLINE__ */

# endif  /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */

#endif  /* TAO_UIOP_PROFILE_H */
