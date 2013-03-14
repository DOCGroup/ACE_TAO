// -*- C++ -*-

//=============================================================================
/**
 *  @file     HTIOP_Endpoint.h
 *
 *  $Id$
 *
 *   HTIOP implementation of PP Framework Endpoint interface.
 *
 *  @author  Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#ifndef HTIOP_ENDPOINT_H
#define HTIOP_ENDPOINT_H
#include /**/ "ace/pre.h"

#include "orbsvcs/HTIOP/HTIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/HTBP/HTBP_Addr.h"

#include "tao/Endpoint.h"
#include "tao/ORB.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace HTIOP
  {

    class Connection_Handler;

    /**
     * @class Endpoint
     *
     * @brief Endpoint
     *
     * HTIOP-specific implementation of PP Framework Endpoint interface.
     */
    class HTIOP_Export Endpoint : public TAO_Endpoint
    {
    public:

      friend class Profile;
      friend class TAO_SSLIOP_Profile;

      // = Initialization and termination methods.

      /// Default constructor.
      Endpoint (void);

      /// Constructor.  This is the most efficient constructor since it
      /// does not require any address resolution processing.
      Endpoint (const char *host,
                CORBA::UShort port,
                const char *htid,
                const ACE::HTBP::Addr &addr);

      /// Constructor.
      Endpoint (const ACE::HTBP::Addr &addr,
                int use_dotted_decimal_addresses);

      /// Constructor.  This constructor is used when decoding endpoints.
      Endpoint (const char *host,
                CORBA::UShort port,
                const char *htid);

      /// Destructor.
      ~Endpoint (void);

      // = Implementation of abstract TAO_Endpoint methods.  See
      // Endpoint.h for their documentation.

      virtual TAO_Endpoint *next (void);
      virtual int addr_to_string (char *buffer, size_t length);
      virtual void reset_hint (void);

      /// Makes a copy of <this>
      virtual TAO_Endpoint *duplicate (void);

      /// Return true if this endpoint is equivalent to @a other_endpoint.  Two
      /// endpoints are equivalent if their port and host are the same.
      CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint);

      /// Return a hash value for this object.
      CORBA::ULong hash (void);

      // Allocates memory and returns a copy of <this>

      // = HTIOP_Endpoint-specific methods.

      /// Return a reference to the <object_addr>.
      const ACE::HTBP::Addr &object_addr (void) const;

      /// Return a pointer to the host string.  This object maintains
      /// ownership of this string.
      const char *host (void) const;

      /// Copy the string @a h into <host_> and return the resulting pointer.
      /// This object maintains ownership of this string.
      const char *host (const char *h);

      /// Return the port number.
      CORBA::UShort port (void) const;

      /// Set the port number.
      CORBA::UShort port (CORBA::UShort p);

      /// Return a pointer to the host string.  This object maintains
      /// ownership of this string.
      const char *htid (void) const;

      /// Copy the string @a h into <host_> and return the resulting pointer.
      /// This object maintains ownership of this string.
      const char *htid (const char *h);

      //Connection_Handler *&hint (void);
      // Access to our <hint_>.

    private:

      /// Helper method for setting INET_Addr.
      int set (const ACE::HTBP::Addr &addr,
               int use_dotted_decimal_addresses);

      /// String representing the host name.
      CORBA::String_var host_;

      /// TCP port number.
      CORBA::UShort port_;

      CORBA::String_var htid_;

      /// Cached instance of <ACE::HTBP::Addr> for use in making
      /// invocations, etc.
      ACE::HTBP::Addr object_addr_;

      /// Flag to indicate if the address has been resolved and set.
      int object_addr_set_;

      /// HTIOP Endpoints can be strung into a list.  Return the next
      /// endpoint in the list, if any.
      Endpoint *next_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "orbsvcs/HTIOP/HTIOP_Endpoint.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif  /* HTIOP_PROFILE_H */
