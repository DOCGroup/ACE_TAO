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
// = DESCRIPTION
//     UIOP implementation of PP Framework Endpoint interface.
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

#include "strategies_export.h"
#include "ace/UNIX_Addr.h"

class TAO_UIOP_Client_Connection_Handler;

class TAO_Strategies_Export TAO_UIOP_Endpoint : public TAO_Endpoint
{
  // = TITLE
  //   TAO_UIOP_Endpoint
  //
  // = DESCRIPTION
  //   UIOP-specific implementation of PP Framework Endpoint interface.
  //
public:

  friend class TAO_UIOP_Profile;

  // = Initialization and termination methods.

  TAO_UIOP_Endpoint (void);
  // Default constructor.

  TAO_UIOP_Endpoint (const ACE_UNIX_Addr &addr);
  // Constructor.

  ~TAO_UIOP_Endpoint (void);
  // Destructor.

  // = Implementation of abstract TAO_Endpoint methods.  See
  // Endpoint.h for their documentation.

  virtual TAO_Endpoint *next (void);
  virtual int addr_to_string (char *buffer, size_t length);
  virtual void reset_hint (void);

  virtual TAO_Endpoint *duplicate (void);
  // Returns a copy of <this>

  CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint);
  // Return true if this endpoint is equivalent to <other_endpoint>.  Two
  // endpoints are equivalent iff their rendezvous points are the same.

  CORBA::ULong hash (void);
  // Return a hash value for this object.

  // = UIOP_Endpoint-specific methods.

  const ACE_UNIX_Addr &object_addr (void) const;
  // Return a reference to the <object_addr>.

  const char *rendezvous_point (void) const;
  // Return a pointer to the rendezvous point string.
  // This object maintains ownership of the returned string.

  // TAO_UIOP_Client_Connection_Handler *&hint (void);
  // Access to our <hint_>.

private:

  ACE_UNIX_Addr object_addr_;
  // Cached instance of <ACE_UNIX_Addr> for use in making
  // invocations, etc.

  // TAO_UIOP_Client_Connection_Handler *hint_;
  // Hint indicating the last successfully used connection handler for
  // a connection established through this endpoint's acceptor.

  TAO_UIOP_Endpoint *next_;
  // UIOP Endpoints can be stringed into a list.  Return the next
  // endpoint in the list, if any.
};

#if defined (__ACE_INLINE__)
# include "UIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

# endif  /* TAO_HAS_UIOP == 1 */

#include "ace/post.h"
#endif  /* TAO_UIOP_ENDPOINT_H */
