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
#include "ace/pre.h"

#include "tao/IOPC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/GIOPC.h"


/**
 * @class TAO_Tagged_Profile
 *
 * @brief Implementation of one of the GIOP 1.2 Target Address
 * recognition mode
 *
 * This class is used to manipulate and access the profile_data
 * field of an IIOP profile (and other GIOP profiles).
 * The definition for that field is simply a sequence of the
 * following structures:
 * struct ProfileBody
 * {
 * Version iiop_version;
 * string host;
 * unsigned short port;
 * sequence<octet> object_key;
 * sequence <IOP::TaggedComponent> components;
 * };
 * the real motivation is to store the details sent by a client
 * and use only the object key which is relevant to a TAO server
 * as on date.
 */
class TAO_Export TAO_Tagged_Profile
{
public:
  /// Ctor
  TAO_Tagged_Profile (TAO_ORB_Core *orb_core);

  /// Return the object key
  TAO_ObjectKey &object_key (void);

  /// Save  the object key
  void object_key (TAO_ObjectKey &object_key);

  /// Return a const object key
  const TAO_ObjectKey &object_key (void) const;

  /// get the tagged_profile
  IOP::TaggedProfile &tagged_profile (void);

  /// get the addressing info
  GIOP::IORAddressingInfo &addressing_info (void);

  /// Extract the object key from the TaggedProfile and store it in
  /// <object_key_>
  CORBA::Boolean extract_object_key (IOP::TaggedProfile &profile);

private:
  /// Our ORB Core
  TAO_ORB_Core *orb_core_;

  /// The object key
  TAO_ObjectKey object_key_;

  /// The Tagged profile. This class would have the Tagged Profile
  IOP::TaggedProfile profile_;

  /// The addressing info
  GIOP::IORAddressingInfo addr_info_;

  // The above two declarations are not used in TAO as on date. It is
  // here so that we can use this anyday. The object_key extracted
  // from these would still be available in <object_key_>.
};

#if defined (__ACE_INLINE__)
# include "tao/Tagged_Profile.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_TAGGED_PROFILE_H*/
