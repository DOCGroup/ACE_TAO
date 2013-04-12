// -*- C++ -*-

//=============================================================================
/**
 *  @file     HTIOP_Profile.h
 *
 *  $Id$
 *
 *   HTIOP profile specific processing
 *
 *
 *  @author  Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================


#ifndef HTIOP_PROFILE_H
#define HTIOP_PROFILE_H
#include /**/ "ace/pre.h"

#include "tao/Profile.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object_KeyC.h"
#include "orbsvcs/HTIOP/HTIOP_Endpoint.h"

#include "ace/Synch.h"

#if !defined (OCI_TAG_HTIOP_PROFILE)
# define OCI_TAG_HTIOP_PROFILE 0x4f434902U
#endif

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace HTIOP
  {

    // TAO HTIOP_Profile concrete Profile definitions
    /**
     * @class HTIOP_Profile
     *
     * @brief This class defines the protocol specific attributes required
     * for locating ORBs over a TCP/IP network.
     *
     * This class defines the HTIOP profile as specified in the CORBA
     * specification.
     */
    class HTIOP_Export Profile : public TAO_Profile
    {
    public:
      /// The object key delimiter that HTIOP uses or expects.
      static const char object_key_delimiter_;
      virtual char object_key_delimiter (void) const;

      /// Return the char string prefix.
      static const char *prefix (void);

      /// Profile constructor, same as above except the object_key has
      /// already been marshaled.
      Profile (const ACE::HTBP::Addr &addr,
               const TAO::ObjectKey &object_key,
               const TAO_GIOP_Message_Version &version,
               TAO_ORB_Core *orb_core);

      ///  Profile constructor, this is the most efficient since it
      /// doesn't require any address resolution processing.
      Profile (const char *host,
               CORBA::UShort port,
               const char *htid,
               const TAO::ObjectKey &object_key,
               const ACE::HTBP::Addr &addr,
               const TAO_GIOP_Message_Version &version,
               TAO_ORB_Core *orb_core);

      /// Profile constructor, default.
      Profile (TAO_ORB_Core *orb_core);

      /// Destructor is to be called only through <_decr_refcnt>.
      ~Profile (void);

      /**
       * Return a string representation for this profile.
       * client must deallocate memory.
       * This is used to create url-style reference.  Only one
       * endpoint is included into the string.
       */
      virtual char * to_string (void) const;

      /**
       * Endpoints are transmitted using TAO-proprietory tagged component.
       * Component tag is TAO_TAG_ENDPOINTS and component data is an
       * encapsulation of a sequence of structs, each representing a
       * single endpoint.  Data format is specified in iiop_endpoins.pidl.
       */
      virtual int encode_endpoints (void);

      /// Return pointer to the head of this profile's endpoints list.
      virtual TAO_Endpoint *endpoint (void);

      /// Return how many endpoints this profile contains.
      virtual CORBA::ULong endpoint_count (void) const;

      /**
       * Add <endp> to this profile's list of endpoints (it is inserted
       * next to the head of the list).  This profiles takes ownership of
       * <endp>.
       */
      void add_endpoint (Endpoint *endp);

      /// Return a hash value for this object.
      virtual CORBA::ULong hash (CORBA::ULong max);

    protected:

      /// Template methods. Please see Profile.h for the documentation.
      virtual int decode_profile (TAO_InputCDR &cdr);
      virtual int decode_endpoints (void);
      virtual void parse_string_i (const char *string);
      virtual void create_profile_body (TAO_OutputCDR &cdr) const;
      virtual CORBA::Boolean do_is_equivalent (const TAO_Profile *other_profile);

    protected:

      /**
       * Head of this profile's list of endpoints.  This endpoint is not
       * dynamically allocated because a profile always contains at least
       * one endpoint.
       *
       * Addressing info of the default endpoint, i.e., head of the list,
       * is transmitted using standard HTIOP ProfileBody components.  See
       * <encode_endpoints> method documentation above for how the rest of
       * the endpoint list is transmitted.
       */
      Endpoint endpoint_;

      /// Number of endpoints in the list headed by <endpoint_>.
      CORBA::ULong count_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif  /* HTIOP_PROFILE_H */
