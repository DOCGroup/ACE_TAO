//-*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    target_identifier.h
//
// = DESCRIPTION
//    A class that encapsulates the target identification details.
//    
//    
// = AUTHOR
//     Balachandran  Natarajan <bala@cs.wustl.edu>
// ============================================================================
#ifndef _TAO_TARGET_SPECIFICATION_H_
#define _TAO_TARGET_SPECIFICATION_H_
#include "tao/Object_KeyC.h"
#include "tao/IOPC.h"


class TAO_Target_Specification
{
  // = TITLE
  //   A class to encapsulate all the ways of specifying targets.
  //
  // = DESCRIPTION
  //   This is a sort of auxillary class. The motivation behind this
  //   is GIOP 1.2 althought I foresee other messaging protocols doing
  //   something similar. The Invocation classes (client side) were
  //   passing the object key that they had extracted from the
  //   profiles with every invocation. This extraction would be done
  //   based on the policies that are specified for the client side
  //   ORB. Further the client side  ORB need not just send the object
  //   key. They can send send the IOP::TaggedProfile or IOP::IOR
  //   profile. So I am putting these possibilites in this class and
  //   pass it to the messaging layer. It would extract what is
  //   required. 
public:
  
  TAO_Target_Specification (void);
  // Ctor
  enum TAO_Target_Address
  {
    // Note that this could be extended for other protocols
    Key_Addr = 0,
    Profile_Addr,
    Reference_Addr
  };

  void target_specifier (const TAO_ObjectKey &key);
  void target_specifier (IOP::TaggedProfile *profile);
  void target_specifier (IOP::IOR *ior, 
                         CORBA::ULong prof_index);
  // Specification of targets

  const TAO_ObjectKey* object_key (void);
  // Returns the object key after a check of the stored specifier. If
  // the stored specifier is not of the right type then this would
  // return a NULL
  
  const IOP::TaggedProfile *profile (void);
  // Returns the IOP::TaggedProfile after a check of the stored specifier. If
  // the stored specifier is not of the right type then this would
  // return a NULL
  
  CORBA::ULong iop_ior (IOP::IOR *ior);
  // Returns a  pointer to IOP::IOR through the parameters and the
  // index of the selected profile as a return parameter after a check
  // of the stored specifier. If the stored specifier is not of the
  // right type then this would return a NULL. 
  
  TAO_Target_Address specifier (void);
  // Access the TArget_Address specifier
  
private:
  union
  {
    TAO_ObjectKey *object_key_;
    IOP::TaggedProfile *profile_;
    IOP::IOR *ior_;
  } u_;
  // The union of all the possibilities
  
  TAO_Target_Address specifier_;
  // The enum identifier

  CORBA::ULong profile_index_;
  // The profile index
};

#if defined (__ACE_INLINE__)
#include "target_identifier.i"
#endif /* defined INLINE */

#endif /*_TAO_TARGET_SPECIFICATION_H_ */
