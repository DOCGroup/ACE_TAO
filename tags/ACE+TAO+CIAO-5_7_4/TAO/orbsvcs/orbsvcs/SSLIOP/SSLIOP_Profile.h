// -*- C++ -*-

//=============================================================================
/**
 *  @file SSLIOP_Profile.h
 *
 *  $Id$
 *
 *  SSLIOP profile specific processing
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_PROFILE_H
#define TAO_SSLIOP_PROFILE_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOP/SSLIOP_Endpoint.h"
#include "tao/IIOP_Profile.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// namespace TAO
// {
//   namespace SSLIOP
//   {

    /**
     * @class Profile
     *
     * @brief This class defines the protocol specific attributes
     *        required for locating ORBs over a TCP/IP network, using
     *        either IIOP or IIOP/SSL for communication.
     *
     * This class extends TAO_IIOP_Profile to support secure
     * communication using SSL.
     */
    class TAO_SSLIOP_Profile : public TAO_IIOP_Profile
    {
    public:
      /// Profile constructor, same as above except the object_key has
      /// already been marshaled.
      TAO_SSLIOP_Profile (const ACE_INET_Addr & addr,
                          const TAO::ObjectKey & object_key,
                          const TAO_GIOP_Message_Version & version,
                          TAO_ORB_Core * orb_core,
                          const ::SSLIOP::SSL * ssl_component);

      ///  Profile constructor, this is the most efficient since it
      /// doesn't require any address resolution processing.
      TAO_SSLIOP_Profile (const char *host,
                          CORBA::UShort port,
                          const TAO::ObjectKey & object_key,
                          const ACE_INET_Addr & addr,
                          const TAO_GIOP_Message_Version & version,
                          TAO_ORB_Core * orb_core,
                          const ::SSLIOP::SSL * ssl_component);

      /// Create profile with the given SSLIOP tagged component.
      TAO_SSLIOP_Profile (TAO_ORB_Core * orb_core,
                          const ::SSLIOP::SSL * ssl_component);

      /// Profile constructor. ssl_only != 0 will force secure
      /// connections, pnly.
      TAO_SSLIOP_Profile (TAO_ORB_Core * orb_core, int ssl_only = 0);


      // = Please see Profile.h for the documentation of these methods.
      virtual int decode (TAO_InputCDR& cdr);
      virtual int encode_endpoints (void);
      virtual TAO_Endpoint *endpoint (void);

      /**
       * Override parse_string() from the base class to update the SSL
       * endpoint's iiop endpoint once the base class has completed
       * parsing the string.
       *@par
       * Initialize this object using the given input string.
       * URL-style string contain only one endpoint.
       */
      virtual void parse_string (const char * string);

      /**
       * Add @a endp to this profile's list of endpoints (it is
       * inserted next to the head of the list).  This profiles takes
       * ownership of @a endp.  If @a endp's @c iiop_endpoint_ member
       * is not 0, it is added to our parent's class endpoint list.
       */
      void add_endpoint (TAO_SSLIOP_Endpoint * endp);


      /**
       * Remove @a endp from this profile's list of endpoints.
       */
      void remove_endpoint (TAO_SSLIOP_Endpoint * endp);

      void remove_generic_endpoint (TAO_Endpoint *ep);

    protected:

      /// Destructor.
      /**
       * Protected destructor to enforce proper memory management
       * through the reference counting mechanism.
       */
      ~TAO_SSLIOP_Profile (void);

      /// Profile equivalence template method.
      /**
       * @see TAO_Profile::do_is_equivalent()
       */
      virtual CORBA::Boolean do_is_equivalent (
        const TAO_Profile * other_profile);

    private:

      /**
       * Helper for @c decode.  Decodes TAO_TAG_SSL_ENDPOINTS from a
       * tagged component. Decode only if RTCORBA is enabled.
       *
       * @return 0 on success and -1 on failure.
       *
       * @note This should be enabled only when RTCORBA is enabled,
       *       but sadly others pay the price (of footprint) under
       *       normal operations.
       */
      int decode_tagged_endpoints (void);

      /**
       * Head of this profile's list of endpoints.  This endpoint is
       * not dynamically allocated because a profile always contains
       * at least one endpoint.
       * @par
       * Currently, a profile contains more than one endpoint, i.e.,
       * list contains more than just the head, only when RTCORBA is
       * enabled.  However, in the near future, this will be used in
       * non-RT mode as well, e.g., to support @c
       * TAG_ALTERNATE_IIOP_ADDRESS feature.
       * @par
       * Since SSLIOP profile is an extension of IIOP profile, its
       * addressing info is contained in two places:  IIOP parent
       * class contains all iiop addressing while this class contains
       * SSL-specific addressing additions to iiop.  This means that
       * there are two lists of endpoints: one maintained in the
       * parent class and one maintained here.  Each ssl endpoint
       * maintains a pointer to its counterpart in the parent class
       * endpoint list.
       * @par
       * For transmission of IIOP addressing information, see
       * @c TAO_IIOP_Profile.  Addressing info of the default SSL
       * endpoint, i.e., head of the list, is transmitted using
       * standard SSLIOP::TAG_SSL_SEC_TRANS tagged component.  See
       * @c encode_endpoints method documentation above for how the
       * rest of the SSL endpoint list is transmitted.
       */
      TAO_SSLIOP_Endpoint ssl_endpoint_;

      /**
       * Allways treat this endpoint as secure, even if the constructor
       * did not explicitely specify a tagged component for SSL.
       * @par
       * Most likely the parse_string() will supply a subset of the
       * attributes - port number, for instance.
       */
      int ssl_only_;

    };

//   }  // End SSLIOP namespace.
// }  // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_PROFILE_H */
