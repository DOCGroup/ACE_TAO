// This may look like C, but it's really -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     SHMIOP_Endpoint.h
//
// = DESCRIPTION
//     SHMIOP implementation of PP Framework Endpoint interface.
//
// = AUTHOR
//     Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_SHMIOP_ENDPOINT_H
#define TAO_SHMIOP_ENDPOINT_H
#include "ace/pre.h"

#include "tao/Endpoint.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "strategies_export.h"

#include "tao/ORB.h"

#include "ace/INET_Addr.h"
#include "ace/MEM_Addr.h"



class TAO_Strategies_Export TAO_SHMIOP_Endpoint : public TAO_Endpoint
{
  // = TITLE
  //   TAO_SHMIOP_Endpoint
  //
  // = DESCRIPTION
  //   SHMIOP-specific implementation of PP Framework Endpoint interface.
  //
public:

  friend class TAO_SHMIOP_Profile;

  // = Initialization and termination methods.

  TAO_SHMIOP_Endpoint (void);
  // Default constructor.

  TAO_SHMIOP_Endpoint (const char *host,
                       CORBA::UShort port,
                       const ACE_INET_Addr &addr);
  // Constructor.  This is the most efficient constructor since it
  // does not require any address resolution processing.

  TAO_SHMIOP_Endpoint (const ACE_MEM_Addr &addr,
                       int use_dotted_decimal_addresses);
  // Constructor.

  TAO_SHMIOP_Endpoint (const ACE_INET_Addr &addr,
                       int use_dotted_decimal_addresses);
  // Constructor. -- More of a extension of the previous one. One of
  // them need to go. We will decide about that as time goes by.

  TAO_SHMIOP_Endpoint (const char *host,
                       CORBA::UShort port,
                       CORBA::Short priority);
  // Constructor.  This constructor is used when decoding endpoints.

  ~TAO_SHMIOP_Endpoint (void);
  // Destructor.

  // = Implementation of abstract TAO_Endpoint methods.  See
  // Endpoint.h for their documentation.

  virtual TAO_Endpoint *next (void);
  virtual int addr_to_string (char *buffer, size_t length);
  virtual void reset_hint (void);
  virtual TAO_Endpoint *duplicate (void);
  // Return a copy of <this>

  CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint);
  // Return true if this endpoint is equivalent to <other_endpoint>.  Two
  // endpoints are equivalent iff their port and host are the same.

  CORBA::ULong hash (void);
  // Return a hash value for this object.

  // = SHMIOP_Endpoint-specific methods.

  const ACE_INET_Addr &object_addr (void) const;
  // Return a reference to the <object_addr>.

  const char *host (void) const;
  // Return a pointer to the host string.  This object maintains
  // ownership of this string.

  const char *host (const char *h);
  // Copy the string <h> into <host_> and return the resulting pointer.
  // This object maintains ownership of this string.

  CORBA::UShort port (void) const;
  // Return the port number.

  CORBA::UShort port (CORBA::UShort p);
  // Set the port number.

  // TAO_SHMIOP_Client_Connection_Handler *&hint (void);
  // Access to our <hint_>.

private:
  int set (const ACE_INET_Addr &addr,
           int use_dotted_decimal_addresses);
  // Helper method for setting INET_Addr.

  CORBA::String_var host_;
  // String representing the host name.

  CORBA::UShort port_;
  // TCP port number.

  ACE_INET_Addr object_addr_;
  // Cached instance of <ACE_INET_Addr> for use in making
  // invocations, etc.

  // TAO_SHMIOP_Client_Connection_Handler *hint_;
  // Hint indicating the last successfully used connection handler for
  // a connection established through this endpoint's acceptor.

  TAO_SHMIOP_Endpoint *next_;
  // SHMIOP Endpoints can be stringed into a list.  Return the next
  // endpoint in the list, if any.
};

#if defined (__ACE_INLINE__)
# include "SHMIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */

#include "ace/post.h"
#endif  /* TAO_IIOP_PROFILE_H */
