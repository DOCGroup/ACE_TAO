//-*- C++ -*-

//=============================================================================
/**
 *  @file    target_specification.h
 *
 *  $Id$
 *
 *  A class that encapsulates the target identification details.
 *
 *
 *
 *  @author  Balachandran  Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_TARGET_SPECIFICATION_H
#define TAO_TARGET_SPECIFICATION_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IOP_IORC.h"
#include "tao/Object_KeyC.h"

/**
 * @class TAO_Target_Specification
 *
 * @brief A class to encapsulate all the ways of specifying targets.
 *
 */
class TAO_Export TAO_Target_Specification
{
public:

  /// Ctor
  TAO_Target_Specification (void);
  enum TAO_Target_Address
  {
    // Note that this could be extended for other protocols
    Key_Addr = 0,
    Profile_Addr,
    Reference_Addr
  };

  // Note: Please do not pass in a identifiers that is allocated on
  // stack. These methods does not make a copy but holds the pointer
  // passed in.
  /// Set the target specification by giving the object key.
  void target_specifier (const TAO::ObjectKey &key);


  /// Set the target specification by passing in an IOP::TaggedProfile.
  void target_specifier (IOP::TaggedProfile &profile);

  /**
   * Specify the target by passing in the IOP::IOR with a profile
   * index. Please see the header file IOPC.h on why a profile index
   * is required.
   */
  void target_specifier (IOP::IOR &ior,
                         CORBA::ULong prof_index);


  /**
   * Returns the object key after a check of the stored specifier. If
   * the stored specifier is not of the right type then this would
   * return a NULL
   */
  const TAO::ObjectKey* object_key (void);

  /**
   * Returns the IOP::TaggedProfile after a check of the stored specifier. If
   * the stored specifier is not of the right type then this would
   * return a NULL
   */
  const IOP::TaggedProfile *profile (void);

  /**
   * Returns a  pointer to IOP::IOR through the parameters and the
   * index of the selected profile as a return parameter after a check
   * of the stored specifier. If the stored specifier is not of the
   * right type then this would return a NULL.
   */
  CORBA::ULong iop_ior (IOP::IOR *&ior);

  /// Access the TArget_Address specifier
  TAO_Target_Address specifier (void);

private:
  union
  {
    TAO::ObjectKey *object_key_;
    IOP::TaggedProfile *profile_;
    IOP::IOR *ior_;
  } u_;
  // The union of all the possibilities

  /// The enum identifier
  TAO_Target_Address specifier_;

  /// The profile index
  CORBA::ULong profile_index_;
};

#if defined (__ACE_INLINE__)
#include "target_specification.i"
#endif /* defined INLINE */

#include /**/ "ace/post.h"
#endif /*TAO_TARGET_SPECIFICATION_H*/
