// This may look like C, but it's really -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     UIOP_Endpoint.h
//
// = AUTHOR
//     Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_UIOP_ENDPOINT_H
#define TAO_UIOP_ENDPOINT_H
#include "ace/pre.h"

#include "tao/Endpoint.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if TAO_HAS_UIOP == 1

#include "ace/UNIX_Addr.h"

class TAO_UIOP_Client_Connection_Handler;

class TAO_Export TAO_UIOP_Endpoint : public TAO_Endpoint
{
  // = TITLE
  //   This class defines the protocol specific attributes required
  //   for locating ORBs over a TCP/IP network.
  //
  // = DESCRIPTION
  //   This class defines the IIOP profile as specified in the CORBA
  //   specification.
public:

  friend class TAO_UIOP_Profile;

  TAO_UIOP_Endpoint (const ACE_UNIX_Addr &addr);
  // Profile constructor

  TAO_UIOP_Endpoint (void);
  // Endpoint constructor, default.

  ~TAO_UIOP_Endpoint (void);
  // Destructor is to be called only through <_decr_refcnt>.

  CORBA::Boolean is_equivalent (const TAO_UIOP_Endpoint *other_endpoint);
  // Return true if this profile is equivalent to other_profile.  Two
  // profiles are equivalent iff their key, port, host, object_key and
  // version are the same.

  CORBA::ULong hash (void);
  // Return a hash value for this object.

  // = Abstract Endpoint interface methods.

  TAO_Endpoint *next (void);
  // Return the next endpoint in the list.

  virtual int addr_to_string (char *buffer, size_t length);
  // Return a string representation for the address.

  virtual void reset_hint (void);
  //  Reset the hint's value.

  // = UIOP Endpoint methods.

  const char *rendezvous_point (void) const;
  // Return a pointer to the rendezvous point string.
  // This object maintains ownership of the returned string.

  const ACE_UNIX_Addr &object_addr (void) const;
  //  return a reference to the object_addr.

  TAO_UIOP_Client_Connection_Handler *&hint (void);
  //  This is a hint for which connection handler to use.

private:

  ACE_UNIX_Addr object_addr_;
  // Cached instance of <ACE_UNIX_Addr> for use in making
  // invocations, etc.

  TAO_UIOP_Client_Connection_Handler *hint_;
  // Pointer to a connection handler which we successfully used
  // already.

  TAO_UIOP_Endpoint *next_;
  // Next endpoint in the list.
};

#if defined (__ACE_INLINE__)
# include "tao/UIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

# endif  /* TAO_HAS_UIOP == 1 */

#include "ace/post.h"
#endif  /* TAO_UIOP_ENDPOINT_H */
