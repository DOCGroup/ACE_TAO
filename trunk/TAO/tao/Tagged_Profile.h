// -*- C++ -*-

//=============================================================================
/**
 *  @file   Tagged_Profile.h
 *
 *  $Id$
 *
 *  @author Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_TAGGED_PROFILE_H
#define TAO_TAGGED_PROFILE_H

#include /**/ "ace/pre.h"

#include "tao/IOPC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object_KeyC.h"
#include /**/ "tao/Versioned_Namespace.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {
    class FW_Server_Request_Wrapper;
  }
}

/**
 * @class TAO_Tagged_Profile
 *
 * @brief This class is used to manipulate and access the target
 *        address field of a GIOP 1.2 request.
 */
class TAO_Export TAO_Tagged_Profile
{
public:

  /// Declare FW_Server_Request_Wrapper a friend
  /// This friendship makes the FW_Server_Request_Wrapper be able to
  /// clone the TAO_Tagged_Profile data member in TAO_ServerRequest.
  friend class TAO::CSD::FW_Server_Request_Wrapper;

  /// Ctor
  TAO_Tagged_Profile (TAO_ORB_Core *orb_core);

  /// Unmarshal the GIOP 1.2 target address field.
  CORBA::Boolean unmarshall_target_address (TAO_InputCDR &cdr);

  /// Unmarshals the received object key for GIOP 1.0/1.1
  CORBA::Boolean unmarshall_object_key (TAO_InputCDR &cdr);

  /// Return the object key
  TAO::ObjectKey &object_key (void);

  /// Save the object key
  void object_key (TAO::ObjectKey &object_key);

  /// Return a const object key
  const TAO::ObjectKey &object_key (void) const;

  /// Get the tagged_profile
  const IOP::TaggedProfile &tagged_profile (void) const;

  /// Get the profile index, that needs to be used in the
  /// sequnce of TaggedProfiles contained  IOP::IOR that is
  /// received from the client.
  CORBA::ULong profile_index (void) const;

  /// Accessor to the type_id contained in the IOP::IOR received from
  /// the client.
  const char* type_id (void) const;

  CORBA::Short discriminator (void) const;

private:
  /// Extract the object key from the TaggedProfile and store it in
  /// <object_key_>
  CORBA::Boolean extract_object_key (IOP::TaggedProfile &profile);

  /// Unmarshals the received object key
  CORBA::Boolean unmarshall_object_key_i (TAO_InputCDR &cdr);

  /// Unmarshall the IOP::TaggedProfile
  CORBA::Boolean unmarshall_iop_profile_i (TAO_InputCDR &cdr);

  /// Unmarshalls the GIOP::IORAddressingInfo
  CORBA::Boolean unmarshall_ref_addr_i (TAO_InputCDR &cdr);

private:
  /// Our ORB Core
  TAO_ORB_Core *orb_core_;

  /// Keep track of which kind of target profile that was extracted.
  CORBA::Short discriminator_;

  /// Flag to denote whether the object key has been extracted yet.
  CORBA::Boolean object_key_extracted_;

  /// The object key
  TAO::ObjectKey object_key_;

  /// The Tagged profile. This class would have the Tagged Profile
  IOP::TaggedProfile profile_;

  /*
   * The GIOP::IORAddressingInfo is defined as follows
   *   struct IORAddressingInfo
   *     {
   *       unsigned long selected_profile_index;
   *       IOP::IOR ior;
   *     };
   *
   * and the IOP::IOR is defined to be
   *   struct IOR
   *      {
   *        string type_id;
   *        sequence<TaggedProfile>   profiles;
   *      };
   * The mapping for the type_id of type string is TAO::String_Manager
   * which does lot of bad things like allocation on construction and
   * a deallocation on destruction. This is bad along the critical
   * path. So we will store this nested structure ripped open with the
   * profile_index and the type_id with the TaggedProfile that is
   * pointed to.
   */

  /// The profile index incase we receive a GIOP::IORAddressingInfo
  /// information
  CORBA::ULong profile_index_;

  /// The type_id in the IOP::IOR in case we receive the
  /// GIOP::IORAddressingInfo information.
  const char* type_id_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Tagged_Profile.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /*TAO_TAGGED_PROFILE_H*/
