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
#include "ace/pre.h"

#include "tao/Object_KeyC.h"
#include "tao/IOPC.h"


/**
 * @class TAO_Target_Specification
 *
 * @brief A class to encapsulate all the ways of specifying targets.
 *
 * @@ Bala: do we have examples of how other protocols map object
 * keys?
 * @@ Carlos: The way HTTP-NG does is not quite intuitive. But
 * they too have a sequnce of Octet which more or less fits this
 * model. You are also allowed to specify is a Cache Index (14
 * bits). I think that can also be worked out and shouldn't be
 * a big deal.
 * @@ Bala:What if they pass something around that does not fit
 * this model?
 * @@ Carlos:As long as we dont know it is ok. But then if we get
 * to some point where we have something floating around,
 * obviously we would have well defined data structure in
 * TAO. BTW, in IMHO it is not possible for me to think the
 * myriad data structures that a designer can come up with. So,
 * I can look ahead possibily a couple of days but not a life
 * time  :-) But you have a good question though. Please sont
 * remove these discussions. It could be useful for someone
 * someday.
 */
class TAO_Export TAO_Target_Specification
{

  //   @@ Bala: i hate to be picky on these matters, but you are not
  //   writing a novel or your memoirs, 'I foresee' does not look like
  //   the right kind of comment in a class description.
  //
  //   The motivation behind this is GIOP 1.2 althought I foresee
  //   other messaging protocols doing something similar.
  //   The Invocation classes (client side) were
  //   passing the object key that they had extracted from the
  //   profiles with every invocation. This extraction would be done
  //   based on the policies that are specified for the client side
  //   ORB. Further the client side  ORB need not just send the object
  //   key. They can send send the IOP::TaggedProfile or IOP::IOR
  //   profile. So I am putting these possibilites in this class and
  //   pass it to the messaging layer. It would extract what is
  //   required.
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
  void target_specifier (const TAO_ObjectKey &key);


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
  const TAO_ObjectKey* object_key (void);

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
    TAO_ObjectKey *object_key_;
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

#include "ace/post.h"
#endif /*TAO_TARGET_SPECIFICATION_H*/
