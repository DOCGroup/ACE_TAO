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

#include "tao/Pluggable.h"
#include "tao/Object_KeyC.h"
#include "tao/GIOP.h"
#include "tao/Tagged_Components.h"

#include "ace/Synch.h"
#include "ace/INET_Addr.h"

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
  static const char object_key_delimiter;
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
                    CORBA::Environment &ACE_TRY_ENV);
  // Create object using a string ior.

  TAO_IIOP_Profile (const TAO_IIOP_Profile &pfile);
  // Profile copy constructor

  TAO_IIOP_Profile &operator= (const TAO_IIOP_Profile &src);
  // Copy operator.

  TAO_IIOP_Profile (TAO_ORB_Core *orb_core);
  // Profile constructor, default.

  ~TAO_IIOP_Profile (void);
  // Destructor is to be called only through <_decr_refcnt>.

  int parse_string (const char *string,
                    CORBA::Environment &ACE_TRY_ENV);
  // Initialize this object using the given input string.

  CORBA::String to_string (CORBA::Environment &ACE_TRY_ENV);
  // Return a string representation for this profile.
  // client must deallocate memory.

  int decode (TAO_InputCDR& cdr);
  // Initialize this object using the given CDR octet string.

  virtual int encode (TAO_OutputCDR &stream) const;
  // Encode this profile in a stream, i.e. marshal it.

  const TAO_ObjectKey &object_key (void) const;
  // @@ deprecated, return a reference to the Object Key.

  TAO_ObjectKey &object_key (TAO_ObjectKey& objkey);
  // @@ deprecated. set the Object Key.

  TAO_ObjectKey *_key (void) const;
  //  Return a pointer to the Object Key.

  CORBA::Boolean is_equivalent (const TAO_Profile *other_profile);
  // Return true if this profile is equivalent to other_profile.  Two
  // profiles are equivalent iff their key, port, host, object_key and
  // version are the same.

  CORBA::ULong hash (CORBA::ULong max,
                     CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Return a hash value for this object.

  int addr_to_string (char *buffer, size_t length);
  // Return a string representation for the address.

  const ACE_INET_Addr &object_addr (void) const;
  //  return a reference to the object_addr.

  const char *host (void);
  // Return a pointer to the host string.  This object maintains
  // ownership of this string.

  const char *host (const char *h);
  // Copy the string h into host and return the resulting pointer.
  // This object maintains ownership of this string.

  CORBA::UShort port (void);
  // Return the port number.

  CORBA::UShort port (CORBA::UShort p);
  // Set the port number.

  const TAO_GIOP_Version& version (void) const;
  // Return a pointer to this profile's version.  This object
  // maintains ownership.

  TAO_IIOP_Client_Connection_Handler *&hint (void);
  //  This is a hint for which connection handler to use.

  void reset_hint (void);
  //  Reset the hint's value.

  const TAO_Tagged_Components& tagged_components (void) const;
  TAO_Tagged_Components& tagged_components (void);
  // Access the tagged components, notice that they are empty and
  // ignored for GIOP 1.0

private:
  int set (const ACE_INET_Addr &addr);
  // helper method to set the INET_Addr.

private:

  char *host_;
  // String representing the hosts name.

  CORBA::UShort port_;
  // TCP port number

  TAO_GIOP_Version version_;
  // IIOP version number.

  TAO_ObjectKey object_key_;
  // object_key associated with this profile.

  ACE_INET_Addr object_addr_;
  // Cached instance of <ACE_INET_Addr> for use in making
  // invocations, etc.

  TAO_IIOP_Client_Connection_Handler *hint_;
  // Pointer to a connection handler which we successfully used
  // already.

  TAO_ORB_Core *orb_core_;
  // ORB Core.

  TAO_Tagged_Components tagged_components_;
  // The tagged components
};

#if defined (__ACE_INLINE__)
# include "tao/IIOP_Profile.i"
#endif /* __ACE_INLINE__ */

#endif  /* TAO_IIOP_PROFILE_H */
