// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     SHMIOP_Profile.h
//
// = DESCRIPTION
//     SHMIOP profile specific processing
//
// = AUTHOR
//     Nanbor Wang <nanbor@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_SHMIOP_PROFILE_H
#define TAO_SHMIOP_PROFILE_H

#include "tao/Profile.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"
#include "tao/Object_KeyC.h"
#include "tao/GIOP.h"

#include "ace/Synch.h"
#include "ace/INET_Addr.h"

class TAO_SHMIOP_Client_Connection_Handler;

// TAO SHMIOP_Profile concrete Profile definitions
class TAO_Export TAO_SHMIOP_Profile : public TAO_Profile
{
  // = TITLE
  //     This class defines the protocol specific attributes required
  //     for locating local ORBs over local IPC mechanism to use the
  //     shared memory transport.
  // = DESCRIPTION
  //     This class defines the SHMIOP profile.
public:
  static const char object_key_delimiter;
  virtual const char get_object_key_delimiter (void);
  // The object key delimiter that SHMIOP uses or expects.

  static const char *prefix (void);
  // Return the char string prefix.

  TAO_SHMIOP_Profile (const ACE_INET_Addr &addr,
                      const TAO_ObjectKey &object_key,
                      const TAO_GIOP_Version &version,
                      TAO_ORB_Core *orb_core);
  // Profile constructor, same as above except the object_key has
  // already been marshaled.

  TAO_SHMIOP_Profile (const char *host,
                      CORBA::UShort port,
                      const TAO_ObjectKey &object_key,
                      const ACE_INET_Addr &addr,
                      const TAO_GIOP_Version &version,
                      TAO_ORB_Core *orb_core);
  //  Profile constructor, this is the most efficient since it
  // doesn't require any address resolution processing.

  TAO_SHMIOP_Profile (const char *string,
                      TAO_ORB_Core *orb_core,
                      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Create object using a string ior.

  TAO_SHMIOP_Profile (const TAO_SHMIOP_Profile &pfile);
  // Profile copy constructor

  TAO_SHMIOP_Profile &operator= (const TAO_SHMIOP_Profile &src);
  // Copy operator.

  TAO_SHMIOP_Profile (TAO_ORB_Core *orb_core);
  // Profile constructor, default.

  ~TAO_SHMIOP_Profile (void);
  // Destructor is to be called only through <_decr_refcnt>.

  int parse_string (const char *string,
                    CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Initialize this object using the given input string.

  char * to_string (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
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

  TAO_SHMIOP_Client_Connection_Handler *&hint (void);
  //  This is a hint for which connection handler to use.

  void reset_hint (void);
  //  Reset the hint's value.

private:
  int set (const ACE_INET_Addr &addr);
  // helper method to set the INET_Addr.

private:

  CORBA::String_var host_;
  // String representing the hosts name.

  CORBA::UShort port_;
  // TCP port number

  TAO_GIOP_Version version_;
  // GIOP version number.

  TAO_ObjectKey object_key_;
  // object_key associated with this profile.

  ACE_INET_Addr object_addr_;
  // Cached instance of <ACE_INET_Addr> for use in making
  // invocations, etc.

  TAO_SHMIOP_Client_Connection_Handler *hint_;
  // Pointer to a connection handler which we successfully used
  // already.

  TAO_ORB_Core *orb_core_;
  // ORB Core.
};

#if defined (__ACE_INLINE__)
# include "tao/SHMIOP_Profile.i"
#endif /* __ACE_INLINE__ */

#endif  /* TAO_SHMIOP_PROFILE_H */
